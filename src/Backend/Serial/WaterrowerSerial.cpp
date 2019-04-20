

#include "SerialAdapter.h"
#include "WaterrowerProtocol.h"
#include "IProtocolHandler.h"
#include "WaterrowerSerial.h"

#include "../System/Logging.h"
#include "../System/Parsing.h"

#include <QDebug>
#include <QDateTime>
#include <QMessageLogger>



BOOST_FUSION_ADAPT_STRUCT
(
    Waterrower::Serial::WaterrowerVersion,
    (int, model)
    (int, major)
    (int, minor)
)


namespace Waterrower {
namespace Serial {


    template<typename T> QString toCategory();
    template<> QString toCategory<BasicCommand>()       { return "[   BASIC]"; }
    template<> QString toCategory<Register>()           { return "[REGISTER]"; }
    template<> QString toCategory<KeyPadInteraction>()  { return "[  KEYPAD]"; }
    template<> QString toCategory<WaterrowerVersion>()  { return "[ VERSION]"; }


    void traceMessage( BasicCommand cmd )
    {
        qUSB.info( QString(
                          toCategory<BasicCommand>() +
                          QString(" ") +
                          toDescription( cmd ) ) );
    }

    void traceMessageBasic( BasicCommand cmd, int value )
    {
        qUSB.info( QString(
                          toCategory<BasicCommand>() +
                          QString(" ") +
                          toDescription( cmd ) +
                          QString(" : ") +
                          QString::number(value) ) );
    }

    void traceMessageRegister( Register cmd, int value )
    {
        qUSB.info( QString(
                          toCategory<Register>() +
                          QString(" ") +
                          toDescription( cmd ) +
                          QString(" : ") +
                          QString::number(value) ), Logger::LogLevel_Extended );
    }




    void traceMessageKeyPadInteraction( KeyPadInteraction cmd )
    {
         qUSB.info( QString(
                              toCategory<KeyPadInteraction>() +
                              QString(" ") +
                              toDescription( cmd ) ) );
    }


    void traceVersionMessage( const class WaterrowerVersion & version )
    {
        qUSB.info( QString(
                          toCategory<WaterrowerVersion>() +
                          QString("        Model: ") +
                          QString::number( version.model ) +
                          QString("  S/W v") +
                          QString::number( version.major ) +
                          QString(".") +
                          QString::number( version.minor) )
                      );
    }




    /** boost::spirit::qi parser rule for matching Waterrower Monitor's InformationRequestPackets with the given register. On parser match, the given functor will be called passing exactly ONE INTEGER argument into the callback */
    GrammarRule makeRule( enum Register address, IProtocolHandler & handler, pFnArgumentInt callback )
    {
        return      char_('I')                                  // 'I' for Information Request Packet
                >>  ( char_('D') | char_('R') )                 // 'D' for Data Measurement, 'R' for RequestValue
                >>  ( char_('S')  | char_('D') | char_('T') )   // 'S' for Single-Byte, 'D' for Double-Byte, 'T' for Triple-Byte
                >>  string( translateAddress( address) )        // Ascii-coded hexadecimal register address from the Lookup-Table
                >>  int_parser<int, 16>()                       // Ascii-coded hexadecimal integer value with data length of 8/16/24 Bits as given by S|D|T rule
                    [ boost::bind( callback, &handler, _1) ]
                    [ boost::bind( traceMessageRegister, address, _1  ) ]
                ;
    }


    /** boost::spirit::qi parser rule for matching Waterrower Monitor's basic command set. On Parser match, the given functor will be calling a member function with NO ARGUMENTS */
    GrammarRule makeRule( enum BasicCommand command, IProtocolHandler & handler, pFnArgumentVoid callback )
    {
        return  string( translateCommand( command ) )[ boost::bind( callback, &handler ) ][ boost::bind( traceMessage, command ) ];
    }


    /** boost::spirit::qi parser rule for matching Waterrower Monitor's basic command set. On Parser macth, the given functor will be calling a member function with ONE INTEGER argument */
    GrammarRule makeRule( enum BasicCommand command, IProtocolHandler & handler, pFnArgumentInt callback )
    {
        return string( translateCommand( command ) )  >> int_parser<int, 16>()[ boost::bind( callback, &handler, _1) ][ boost::bind( traceMessageBasic, command, _1 ) ];
    }


    /** boost::spirit::qi parser rule for matching Waterrower Monitor's InteractiveKeyPad press messages */
    GrammarRule makeKeyPadRule(  LexerSymbols< KeyPadInteraction > & keys, IProtocolHandler & handler, pFnArgumentKeyPad callback )
    {
        return string("AK") >> keys[ boost::bind( callback, &handler, _1) ][ boost::bind( traceMessageKeyPadInteraction, _1 ) ];
    }


    /** creates the lexer symbols for @ref{KeyPadInteraction} enum */
    LexerSymbols< KeyPadInteraction > makeKeyPadSymbols()
    {
        LexerSymbols< KeyPadInteraction > symbols;

        symbols.add ( "R", KeyPad_RESET )
                    ( "1", KeyPad_Units )
                    ( "2", KeyPad_Zones )
                    ( "3", KeyPad_WorkoutPrograms )
                    ( "4", KeyPad_UpArrow )
                    ( "5", KeyPad_Ok )
                    ( "6", KeyPad_DownArrow )
                    ( "7", KeyPad_Advanced )
                    ( "8", KeyPad_StoredPrograms )
                    ( "9", KeyPad_HoldCancel );

        return symbols;
    }



    /** context-free grammar for parsing the model/software version message
        @todo not sure how to express this as a simple grammar rule with correct argument passing to the IProtocol functor
    */
    struct VersionParser : Grammar
    {
        VersionParser(IProtocolHandler & protocolHandler)
            : VersionParser::base_type( start )
        {
            using qi::string;
            using qi::int_parser;

            model %= int_parser<unsigned int, 10, 1, 1>();
            major %= int_parser<unsigned int, 10, 2, 2>();
            minor %= int_parser<unsigned int, 10, 2, 2>();

            versionnumbers %= model >> major >> minor;

            start %= string("IV") >> versionnumbers[ boost::bind( &Waterrower::Serial::IProtocolHandler::onVersionInfo, &protocolHandler, _1 ) ]
                                                   [ boost::bind( traceVersionMessage, _1  ) ];

            //BOOST_SPIRIT_DEBUG_NODE(versionnumbers);
        }

        GrammarRuleData<int>                model;
        GrammarRuleData<int>                major;
        GrammarRuleData<int>                minor;
        GrammarRuleData<WaterrowerVersion>  versionnumbers;

        GrammarRule   start;
    };





    /** context-free grammar for parsing live Waterrower v4/5 serial data using boost::spirit::qi parser generator
     */

    struct WaterrowerParser : Grammar
    {
       WaterrowerParser( IProtocolHandler & protocolHandler )
            : WaterrowerParser::base_type(start),
              version( protocolHandler)
        {
            ping            = makeRule( BasicCommand_PING,        protocolHandler, &IProtocolHandler::onPing );
            ss              = makeRule( BasicCommand_StrokeStart, protocolHandler, &IProtocolHandler::onStrokeStart );
            se              = makeRule( BasicCommand_StrokeEnd,   protocolHandler, &IProtocolHandler::onStrokeEnd );
            _wr_            = makeRule( BasicCommand_USB_ACK,     protocolHandler, &IProtocolHandler::onWaterrowerConnected );

            pulsecount      = makeRule( BasicCommand_PulsesPer25msec, protocolHandler, &IProtocolHandler::onPulseCount );

            heartrate       = makeRule( Register_HeartRate,    protocolHandler, &IProtocolHandler::onHeartrateReceived );
            distance        = makeRule( Register_Distance,     protocolHandler, &IProtocolHandler::onDistanceReceived );
            averageSpeed    = makeRule( Register_AverageSpeed, protocolHandler, &IProtocolHandler::onAverageSpeedReceived );
            tankVolume      = makeRule( Register_TankVolume,   protocolHandler, &IProtocolHandler::onTankVolumeReceived );

            strokes         = makeRule( Register_Strokes,       protocolHandler, &IProtocolHandler::onStrokesReceived );

            calibration_pinsperxxcm  = makeRule( Register_CalibrationPinsPerXXcm,   protocolHandler, &IProtocolHandler::onCalibrationPinsPerXXcm );
            calibration_distancexxcm = makeRule( Register_CalibrationDistanceXXcm,  protocolHandler, &IProtocolHandler::onCalibrationDistanceXXcm );

            totalCalories = makeRule( Register_Calories, protocolHandler, &IProtocolHandler::onTotalCaloriesReceived );

            keypadSymbols   = makeKeyPadSymbols();
            keypad          = makeKeyPadRule( keypadSymbols, protocolHandler, &IProtocolHandler::onKeyPadInteraction );


            start %= version | ping | ss | se |_wr_ | pulsecount | heartrate | distance | averageSpeed | tankVolume | keypad
                             | calibration_distancexxcm | calibration_pinsperxxcm |totalCalories | strokes
                    ;
        }

        VersionParser   version;                            /**< sub-grammar defining the decomposition of the version message */

        LexerSymbols< KeyPadInteraction > keypadSymbols;    /**< defines the lexer symbols for keypad events */
        GrammarRule     keypad;                             /**< grammar rule defining the keypad event message parsing */

        GrammarRule     calibration_pinsperxxcm;            /**< grammar rule defining calibration info message */
        GrammarRule     calibration_distancexxcm;           /**< grammar rule defining calibration info message */

        GrammarRule     _wr_;                               /**< grammar rule defining the 'USB connection acknowledge' message */
        GrammarRule     ss;                                 /**< grammar rule defining the 'stroke start' message */
        GrammarRule     se;                                 /**< grammar rule defining the 'stroke complete' message */
        GrammarRule     ping;                               /**< grammar rule defining the keep-alive message */
        GrammarRule     pulsecount;                         /**< grammar rule defining the 'pulses-per-25msec' message */
        GrammarRule     heartrate;                          /**< grammar rule defining the heartrate message */
        GrammarRule     tankVolume;                         /**< grammar rule defining the tankvolume message */
        GrammarRule     distance;                           /**< grammar rule defining the distance message */
        GrammarRule     averageSpeed;                       /**< grammar rule defining the averageSpeed message */
        GrammarRule     totalCalories;                      /**< grammar rule defining the totalCalories message */
        GrammarRule     strokes;                            /**< grammar rule defining the strokes message */

        GrammarRule     start;                              /**< start rule for the grammar */
    };



    void receive(class IProtocolHandler & protocolHandler, const QString &line )
    {
        GrammarString text( line.toStdString() );

        Iterator iter( text.begin() );
        Iterator end( text.end() );

        WaterrowerParser parser( protocolHandler );

        bool success( phrase_parse(iter, end, parser, boost::spirit::ascii::space) && iter == end );

        qUSB.info( QString("<RX> ") + line, Logger::LogLevel_All );

        if( !success )
        {
            qUSB.warning( QString( QString("<ERR> ") + line) );
        }
    }


    void sendRequestsHighPriority( SerialAdapter & serialPort )
    {
        using namespace Waterrower::Serial;

        serialPort.write( buildDataCommand( Register_HeartRate,  DLC_1_Byte ) );
        serialPort.write( buildDataCommand( Register_Distance, DLC_2_Byte ) );
        serialPort.write( buildDataCommand( Register_AverageSpeed, DLC_2_Byte ) );
        serialPort.write( buildDataCommand( Register_Calories, DLC_3_Byte ));
        serialPort.write( buildDataCommand( Register_Strokes, DLC_2_Byte) );
    }


    void sendRequestsLowPriority( SerialAdapter & serialPort )
    {
        serialPort.write( buildDataCommand( Register_TankVolume, DLC_1_Byte ) );
        serialPort.write( buildDataCommand( Register_CalibrationPinsPerXXcm, DLC_1_Byte ) );
        serialPort.write( buildDataCommand( Register_CalibrationDistanceXXcm, DLC_1_Byte ) );
    }


    void sendRequestVersion( SerialAdapter & serialPort )
    {
        serialPort.write( "IV?\r\n" ); // request firmware version and rowing computer model
    }

} // namespace Serial
} // namespace Waterrower

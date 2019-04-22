#include "Parser.h"


#include "../IProtocolHandler.h"

#include "../ProtocolV4.h"
#include "../Logging/Logging.h"


#include "SystemLogging.h"
#include "SystemParsing.h"


BOOST_FUSION_ADAPT_STRUCT
(
	WaterrowerVersion,
	(int, model)
	(int, major)
	(int, minor)
)




/** boost::spirit::qi parser rule for matching Waterrower Monitor's InformationRequestPackets with the given register. On parser match, the given functor will be called passing exactly ONE INTEGER argument into the callback */
GrammarRule makeRule(enum Register address, IProtocolHandler & handler, pFnArgumentInt callback)
{
	return      char_('I')                                  // 'I' for Information Request Packet
		>> (char_('D') | char_('R'))                 // 'D' for Data Measurement, 'R' for RequestValue
		>> (char_('S') | char_('D') | char_('T'))   // 'S' for Single-Byte, 'D' for Double-Byte, 'T' for Triple-Byte
		>> string(translateAddress(address))        // Ascii-coded hexadecimal register address from the Lookup-Table
		>> int_parser<int, 16>()                       // Ascii-coded hexadecimal integer value with data length of 8/16/24 Bits as given by S|D|T rule
		[boost::bind(callback, &handler, _1)]
	[boost::bind(traceMessage<Register, int>, address, _1)]
	;
}


/** boost::spirit::qi parser rule for matching Waterrower Monitor's basic command set. On Parser match, the given functor will be calling a member function with NO ARGUMENTS */
GrammarRule makeRule(enum BasicCommand command, IProtocolHandler & handler, pFnArgumentVoid callback)
{
	return  string(translateCommand(command))[boost::bind(callback, &handler)][boost::bind(traceMessage<BasicCommand>, command)];
}


/** boost::spirit::qi parser rule for matching Waterrower Monitor's basic command set. On Parser macth, the given functor will be calling a member function with ONE INTEGER argument */
GrammarRule makeRule(enum BasicCommand command, IProtocolHandler & handler, pFnArgumentInt callback)
{
	return string(translateCommand(command)) >> int_parser<int, 16>()[boost::bind(callback, &handler, _1)][boost::bind(traceMessage<BasicCommand, int>, command, _1)];
}


/** boost::spirit::qi parser rule for matching Waterrower Monitor's InteractiveKeyPad press messages */
GrammarRule makeKeyPadRule(LexerSymbols< KeyPadInteraction > & keys, IProtocolHandler & handler, pFnArgumentKeyPad callback)
{
	return string("AK") >> keys[boost::bind(callback, &handler, _1)][boost::bind(traceMessage<KeyPadInteraction>, _1)];
}


/** creates the lexer symbols for @ref{KeyPadInteraction} enum */
LexerSymbols< KeyPadInteraction > makeKeyPadSymbols()
{
	LexerSymbols< KeyPadInteraction > symbols;

	symbols.add("R", KeyPad_RESET)
		("1", KeyPad_Units)
		("2", KeyPad_Zones)
		("3", KeyPad_WorkoutPrograms)
		("4", KeyPad_UpArrow)
		("5", KeyPad_Ok)
		("6", KeyPad_DownArrow)
		("7", KeyPad_Advanced)
		("8", KeyPad_StoredPrograms)
		("9", KeyPad_HoldCancel);

	return symbols;
}



/** context-free grammar for parsing the model/software version message
@todo not sure how to express this as a simple grammar rule with correct argument passing to the IProtocol functor
*/
struct VersionParser : Grammar
{
	VersionParser(IProtocolHandler & protocolHandler)
		: VersionParser::base_type(start)
	{
		using qi::string;
		using qi::int_parser;

		model %= int_parser<unsigned int, 10, 1, 1>();
		major %= int_parser<unsigned int, 10, 2, 2>();
		minor %= int_parser<unsigned int, 10, 2, 2>();

		versionnumbers %= model >> major >> minor;

		start %= string("IV") >> versionnumbers[boost::bind(&IProtocolHandler::onVersionInfo, &protocolHandler, _1)]
			[boost::bind(traceVersionMessage, _1)];

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

struct Waterrower_Grammar : Grammar
{
	Waterrower_Grammar(IProtocolHandler & protocolHandler)
		:	Waterrower_Grammar::base_type(start),
			version(protocolHandler)
	{
		ping = makeRule(BasicCommand_PING, protocolHandler, &IProtocolHandler::onPing);
		ss = makeRule(BasicCommand_StrokeStart, protocolHandler, &IProtocolHandler::onStrokeStart);
		se = makeRule(BasicCommand_StrokeEnd, protocolHandler, &IProtocolHandler::onStrokeEnd);
		_wr_ = makeRule(BasicCommand_USB_ACK, protocolHandler, &IProtocolHandler::onWaterrowerConnected);

		pulsecount = makeRule(BasicCommand_PulsesPer25msec, protocolHandler, &IProtocolHandler::onPulseCount);

		heartrate = makeRule(Register_HeartRate, protocolHandler, &IProtocolHandler::onHeartrateReceived);
		distance = makeRule(Register_Distance, protocolHandler, &IProtocolHandler::onDistanceReceived);
		averageSpeed = makeRule(Register_AverageSpeed, protocolHandler, &IProtocolHandler::onAverageSpeedReceived);
		tankVolume = makeRule(Register_TankVolume, protocolHandler, &IProtocolHandler::onTankVolumeReceived);

		strokes = makeRule(Register_Strokes, protocolHandler, &IProtocolHandler::onStrokesReceived);

		calibration_pinsperxxcm = makeRule(Register_CalibrationPinsPerXXcm, protocolHandler, &IProtocolHandler::onCalibrationPinsPerXXcm);
		calibration_distancexxcm = makeRule(Register_CalibrationDistanceXXcm, protocolHandler, &IProtocolHandler::onCalibrationDistanceXXcm);

		totalCalories = makeRule(Register_Calories, protocolHandler, &IProtocolHandler::onTotalCaloriesReceived);

		keypadSymbols = makeKeyPadSymbols();
		keypad = makeKeyPadRule(keypadSymbols, protocolHandler, &IProtocolHandler::onKeyPadInteraction);


		start %= version | ping | ss | se | _wr_ | pulsecount | heartrate | distance | averageSpeed | tankVolume | keypad
			| calibration_distancexxcm | calibration_pinsperxxcm | totalCalories | strokes
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



Parser::Parser( IProtocolHandler & protocolHandler )
	: m_grammar( new Waterrower_Grammar( protocolHandler ) )
{
}

Parser::~Parser() 
{
	delete m_grammar;
}


bool Parser::parse(const QString & line)
{
	String text(line.toStdString());

	Iterator iter(text.begin());
	Iterator end(text.end());

	return phrase_parse(iter, end, *m_grammar, boost::spirit::ascii::space) && iter == end;
}


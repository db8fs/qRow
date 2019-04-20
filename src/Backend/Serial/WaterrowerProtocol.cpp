#include "WaterrowerProtocol.h"
#include "IProtocolHandler.h"

namespace Waterrower {
namespace Serial {

    /** lookup-table columns for waterrower registers */
    enum LUTIndex
    {
        LUT_Address = 0,
        LUT_Description
    };


    /** lookup-table of waterrower registers, ordered by @ref{Register} numbering */
    static const char* s_registerAddresses[][2] = {
        { "1A0",  "                 HeartRate" },
        { "057",  "                  Distance" },
        { "14A",  "              AverageSpeed" },
        { "0A9",  "                TankVolume" },
        { "083",  "    CalibrationPinsPerXXcm" },
        { "084",  "   CalibrationDistanceXXcm" },
        { "08A",  "             TotalCalories" },
        { "140",  "                   Strokes" }
    };


    /** lookup-table of waterrower basic commands, ordered by @ref{BasicCommand} */
    static const char* s_basicCommandLUT[][2] = {
        { "PING", " ==================== PING" },
        { "SS",   " ------------  StrokeStart" },
        { "SE",   " ------------    StrokeEnd" },
        { "_WR_", " ********          USB_ACK" },
        { "P",    " >>>       PulsesPer25msec" }
    };


    /** lookup-table of waterrower keypad events, ordered by @ref{KeyPadInteraction} */
    static const char* s_keypadLUT[] = {
                  " +++                 RESET",
                  " +++                 Units",
                  " +++                 Zones",
                  " +++       WorkoutPrograms",
                  " +++               UpArrow",
                  " +++                    Ok",
                  " +++             DownArrow",
                  " +++              Advanced",
                  " +++            HoldCancel"
    };


    QByteArray toDescription( enum BasicCommand command )      {   return s_basicCommandLUT[ command ][ LUT_Description];      }
    QByteArray toDescription( enum Register address )          {   return s_registerAddresses[ address ][ LUT_Description];    }
    QByteArray toDescription( enum KeyPadInteraction button )  {   return s_keypadLUT[button];                                 }

    const char* const translateCommand( enum BasicCommand command )  {   return s_basicCommandLUT[ command ][ LUT_Address ];     }
    const char* const translateAddress( Register address )           {   return s_registerAddresses[ address ][ LUT_Address ];   }


    QByteArray buildDataCommand( enum Register address, enum DLC dlc, enum WR_Command command )
    {
        QByteArray result( "I" );

        switch( command )
        {
        case WR_Command_Value:
            result += "D";
            break;
        case WR_Command_RequestValue:
            result += "R";
        default:
            break;
        }

        switch( dlc )
        {
        case DLC_2_Byte:
            result += "D";
            break;

        case DLC_3_Byte:
            result += "T";
            break;

        case DLC_1_Byte:
        default:
            result += "S";
            break;
        }

        result += translateAddress( address );
        result += "\r\n";

        return result;
    }



} // namespace Serial
} // namespace Waterrower

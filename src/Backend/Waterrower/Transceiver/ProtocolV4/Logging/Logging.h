#ifndef WATERROWERPROTOCOL_H
#define WATERROWERPROTOCOL_H

#include <QString>
#include "../ProtocolV4.h"

    /** creates a data command string for the given register address.
        @detail Use @ref{command} to indicate if command is used for requesting register values from the waterrower. Registers may hold 1, 2 or 3 Bytes of data as indicated by @ref{dlc}.
     */
    QByteArray buildDataCommand( enum Register address, enum DLC dlc = DLC_1_Byte, enum WR_Command command = WR_Command_RequestValue );

    /** returns a string description of the given command */
    QByteArray toDescription( enum BasicCommand command );

    /** returns a string description of the given register */
    QByteArray toDescription( enum Register address );

    /** returns a string description of the given Keypad button */
    QByteArray toDescription( enum KeyPadInteraction button );

    /** returns the waterrower protocol string for the given command  */
    const char* const translateCommand( enum BasicCommand command );

    /** returns the ascii-coded-hexadecimal address for the given register */
    const char* const translateAddress( Register address );


	template<typename T, typename Val> void traceMessage(T cmd, Val value);

	template<typename T> void traceMessage(T cmd);


	void traceVersionMessage(const struct WaterrowerVersion & version);



#endif // WATERROWERPROTOCOL_H

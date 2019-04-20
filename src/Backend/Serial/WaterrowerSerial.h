#ifndef WATERROWERGRAMMAR_H
#define WATERROWERGRAMMAR_H

#include <QString>

#include "IProtocolHandler.h"

namespace Waterrower {
namespace Serial {

    /** parses the given text line and calls @ref{IProtocolHandler} members on success. */
    void receive(class IProtocolHandler & protocolHandler, const QString & line );

    /** sends the data requests for high update rate */
    void sendRequestsHighPriority( class SerialAdapter & serialPort );

    /** sends the data requests for low update rate */
    void sendRequestsLowPriority ( class SerialAdapter & serialPort );

    /** requests the model and software version of the waterrower */
    void sendRequestVersion( class SerialAdapter & serialPort );

} // namespace Serial
} // namespace Waterrower


#endif // WATERROWERGRAMMAR_H

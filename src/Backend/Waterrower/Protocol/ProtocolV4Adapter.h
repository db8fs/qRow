
#include "ProtocolV4/Parser/IProtocolHandler.h"

#include <QString>

/** Waterrower V4/V5 protocol controller */
class ProtocolV4Adapter 
{
	struct ProtocolV4Adapter_Private* const m_private;

public:
	ProtocolV4Adapter(class WaterrowerModelV4 & model);

	~ProtocolV4Adapter();
	
	void txVersionRequest( class SerialAdapter & serialAdapter );
	void txHighPrioRequests( class SerialAdapter & serialAdapter );
	void txLowPrioRequests( class SerialAdapter & serialAdapter );
	bool rxReceive( const QString & line );

};


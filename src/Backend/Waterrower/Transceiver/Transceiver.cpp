#include "Transceiver.h"

#include "SystemLogging.h"

#include "Protocol/ProtocolV4Adapter.h"
#include "Serial/SerialAdapter.h"

#include <QTimer>


	/////

	/** defines transceiver state */
	struct Transceiver_Private
	{
		Transceiver_Private( ProtocolV4Adapter & protocol)
			:	m_serialAdapter(),
				m_highPriorityTimer(),
				m_lowPriorityTimer(),
				m_protocol( protocol )
		{
		}

		// request firmware version and rowing computer model
		void txVersionRequest()
		{
			m_protocol.txVersionRequest( m_serialAdapter );
		}

		// sends the data requests with high update rate	
		void txHighPrioPollRequest()
		{
			m_protocol.txHighPrioRequests( m_serialAdapter );
		}

		// sends the data requests with low update rate
		void txLowPrioPollRequest()
		{
			m_protocol.txLowPrioRequests( m_serialAdapter );
		}

		bool rxReceive( const QString & line )
		{
			return m_protocol.rxReceive(line);
		}


		SerialAdapter		m_serialAdapter;
		QTimer				m_highPriorityTimer;		/**< timer for requesting extended ergometer computer data with higher priority  */
		QTimer				m_lowPriorityTimer;			/**< timer for requesting low priority ergometer monitor data */
		ProtocolV4Adapter &	m_protocol;					/**< the serial protocol object to be used for Waterrower v4/v5 */
	};



	//////


	Transceiver::Transceiver( class ProtocolV4Adapter & protocol )
		:	QObject(), 
			m_private( new Transceiver_Private( protocol ) )
	{
		connect( &m_private->m_serialAdapter,		&SerialAdapter::waterrowerDisconnected, this, &Transceiver::onWaterrowerDisconnected );
		connect( &m_private->m_serialAdapter,		&SerialAdapter::waterrowerLineReceived, this, &Transceiver::onWaterrowerLineReceived);
		connect( &m_private->m_lowPriorityTimer,	&QTimer::timeout, this, &Transceiver::onLowPriorityPollRequest );
		connect( &m_private->m_highPriorityTimer,	&QTimer::timeout, this, &Transceiver::onHighPriorityPollRequest );
		
		Q_ASSERT( m_private != nullptr );
	}

	Transceiver::~Transceiver()
	{
		delete m_private;
	}



	void Transceiver::onHighPriorityPollRequest()
	{
		m_private->txHighPrioPollRequest();
	}


	void Transceiver::onLowPriorityPollRequest()
	{
		m_private->txLowPrioPollRequest();
	}


	void Transceiver::onWaterrowerDisconnected()
	{
		m_private->m_highPriorityTimer.stop();
		m_private->m_lowPriorityTimer.stop();
		Q_EMIT waterrowerDisconnected();
	}

	void Transceiver::onWaterrowerConnected()
	{	
		m_private->txVersionRequest();


		// initialize the process variables of this data model by querying initial values on connection and updating them onChange
		m_private->txHighPrioPollRequest();
		m_private->txLowPrioPollRequest();

		m_private->m_lowPriorityTimer.start(15000);
		m_private->m_highPriorityTimer.start(500);
	}



	void Transceiver::onWaterrowerLineReceived(const QString & line)
	{
		qUSB.info(QString("<RX> ") + line, SystemLogging::LogLevel_All);

		if( !m_private->rxReceive( line ) )
		{
			qUSB.warning( QString(QString("<ERR> ") + line) );
		}
	}

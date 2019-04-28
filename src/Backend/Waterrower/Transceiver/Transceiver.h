#ifndef WATERROWERGRAMMAR_H
#define WATERROWERGRAMMAR_H

#include <QString>
#include <QObject>

class Transceiver : public QObject
{
	Q_OBJECT

public:
	Transceiver( class ProtocolV4Adapter & protocolHandler );
	~Transceiver();

signals:
	/** signals that a waterrower rowing ergometer has disconnected from the USB port */
	void waterrowerDisconnected();

public slots:
	
	/** handles connect events (TODO: make private) */
	void onWaterrowerConnected();


private slots:
	/** polls the waterrower monitor only few times per sec */
	void onLowPriorityPollRequest();

	/** polls the waterrower ergometer for extended data packets */
	void onHighPriorityPollRequest();


	/** line received from the waterrower ergometer via the USB-UART */
	void onWaterrowerLineReceived(const QString &);

	/** handles disconnect events */
	void onWaterrowerDisconnected();


private:
	struct Transceiver_Private* const	m_private;

};


#endif // WATERROWERGRAMMAR_H

#ifndef SERIALADAPTER_H
#define SERIALADAPTER_H

#include <QObject>
#include <QTimer>
#include <QSharedPointer>



/** adapts to the Waterrower USB UART for receiving status updates via serial communication */
class SerialAdapter : public QObject
{
    Q_OBJECT
public:
    explicit SerialAdapter(QObject *parent = nullptr);

    ~SerialAdapter();

    /** sends the given serial command to the waterrower ergometer */
    void write(const QByteArray & );

signals:

    /** line received from the waterrower ergometer via the USB-UART */
    void waterrowerLineReceived( const QString & );

    /** signals that a waterrower rowing ergometer has connected to the USB port */
    void waterrowerConnected();

    /** signals that a waterrower rowing ergometer has disconnected from the USB port */
    void waterrowerDisconnected();

private slots:

    /** checks the available serial ports for the waterrower ergometer. returns true if ergometer has been found. */
    void update();


private:

    QSharedPointer<class SerialAdapter_Private> m_private;

    QTimer      m_readTimer;        /**< timer for reading serial update from the UART */

};



#endif // SERIALADAPTER_H


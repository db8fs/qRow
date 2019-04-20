#include "SerialAdapter.h"

#include "../System/Logging.h"

#include "WaterrowerProtocol.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QRegularExpression>

#include <QDebug>
#include <QException>
#include <QThread>
#include <QQueue>

namespace Waterrower {
namespace Serial {


class SerialAdapter_Private
{
public:
    SerialAdapter_Private( const QSerialPortInfo & serialPortInfo )
        : m_serialPort( serialPortInfo )
    {
        m_serialPort.setBaudRate( 115200 );
        m_serialPort.setFlowControl( QSerialPort::NoFlowControl );
        m_serialPort.setDataBits( QSerialPort::Data8 );
        m_serialPort.setParity( QSerialPort::NoParity );
        m_serialPort.setStopBits( QSerialPort::OneStop );

        if( !m_serialPort.open( QIODevice::ReadWrite ) )
            throw QException();

        write( "USB\r\n" );
    }

    ~SerialAdapter_Private()
    {
        m_serialPort.write( "EXIT\r\n" );
    }

    void write( const char* text )
    {
        m_outputQueue.push_back( text );
    }

    /** serializes writing of output commands. Waterrower monitor otherwise seems to eat strings up */
    void update()
    {
        if( !m_outputQueue.empty() )
        {
            m_serialPort.write( m_outputQueue.front() );
            m_serialPort.waitForBytesWritten( -1 );

            QString text( m_outputQueue.front() );

            text.replace( "\n", "");
            text.replace( "\r", "");

            qUSB.info( QString("<TX> ") + text, Logger::LogLevel_All );

            m_outputQueue.pop_front();
        }
    }


    QVector<QString> readLines()
    {
        QVector<QString> result;

        while( m_serialPort.isOpen() && m_serialPort.bytesAvailable() )
        {
            QByteArray array( m_serialPort.readLine() );

            array.replace( "\n", "");
            array.replace( "\r", "");

            result.append( array );
        }

        return result;
    }

private:
    QQueue<QByteArray>      m_outputQueue;
    QSerialPort             m_serialPort;
};


SerialAdapter::SerialAdapter(QObject *parent)
    : QObject(parent),
      m_readTimer(),
      m_private()
{
    connect( &m_readTimer, &QTimer::timeout, this, &SerialAdapter::update );

    m_readTimer.start( 20 );
}

SerialAdapter::~SerialAdapter()
{
    m_readTimer.stop();
}

void SerialAdapter::update()
{
    QList<QSerialPortInfo> listOfPorts( QSerialPortInfo::availablePorts() );

    QRegularExpression regEx( "WR-S\[45\]" );
    QRegularExpression calloutUnit( "cu\." );

    for( auto i : listOfPorts )
    {
        if( regEx.match( i.description() ).hasMatch() ) // checkout for Waterrower ergometer version 4 or 5
        {
            if( !calloutUnit.match( i.portName() ).hasMatch() ) //  skip /dev/cu.* devices for macOS/Linux - use tty instead
            {
                if( m_private.isNull() )
                {
                    try
                    {
                        m_private = QSharedPointer<SerialAdapter_Private>( new SerialAdapter_Private( i ) );

                        qUSB.info( QString("SerialAdapter: found Waterrower device at ") + i.systemLocation() );

                        Q_EMIT waterrowerConnected();

                    }
                    catch( const QException & exception )
                    {
                        qUSB.warning( QString("SerialAdapter: Could not open serial port for writing!") + i.systemLocation() );
                    }
                }
                else
                {
                    for( auto i : m_private->readLines() )
                    {
                        Q_EMIT waterrowerLineReceived(i);
                    }

                    m_private->update();
                }

                return;
            }
        }
    }

    if( !m_private.isNull() )
    {
        qUSB.warning( QString("SerialAdapter: waterrowerDisconnected()!") );

        Q_EMIT waterrowerDisconnected();

        m_private.reset();
        return;
    }
}

void SerialAdapter::write(const QByteArray & text)
{
    if( !m_private.isNull() )
    {
        m_private->write( text );
    }
}



} // namespace Serial
} // namespace Waterrower

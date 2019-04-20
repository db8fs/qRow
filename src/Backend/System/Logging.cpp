#include "Logging.h"

#include <QDateTime>


namespace Waterrower {

inline QString traceHeader( const QString & subsystem )
{
    return  QString( QDateTime::currentDateTime().toString( Qt::ISODateWithMs) + QString(" {") + subsystem + QString("} ") );
}


Logger::Logger( const QString & string )
    : m_logger(),
      m_subsystem( string ),
      m_loglevel( Waterrower::Logger::LogLevel_NoLogging )
{
}

void Logger::setLogLevel(Logger::LogLevel actualLogLevel)
{
    m_loglevel = actualLogLevel;
}

void Logger::info(const QString &line, Logger::LogLevel minimumLogLevel)
{
    if( (int) m_loglevel >= minimumLogLevel )
    {
        m_logger.info( "%s %s", traceHeader( m_subsystem ).toUtf8().constData(), line.toUtf8().constData() );
    }
}

void Logger::warning(const QString &line, Logger::LogLevel minimumLogLevel)
{
    if( (int) m_loglevel >= minimumLogLevel )
    {
        m_logger.warning( "%s %s", traceHeader( m_subsystem ).toUtf8().constData(), line.toUtf8().constData() );
    }
}

Logger &Logger::instance(Subsystem subsystem )
{
    static Logger s_default( "General" );
    static Logger s_serial( "USB" );
    static Logger s_audio( "Audio" );
    static Logger s_video( "Video" );

    if( subsystem == Logger::USB )
    {
        return s_serial;
    }

    if( subsystem == Logger::Audio )
    {
        return s_audio;
    }

    if( subsystem == Logger::Video )
    {
        return s_video;
    }

    return s_default;
}


} // namespace Waterrower

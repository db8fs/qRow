#ifndef LOGGING_H
#define LOGGING_H

#include <QString>
#include <QMessageLogger>

namespace Waterrower {

/** */
class Logger
{
public:

    /** enumerates the subsystems of the application */
    enum Subsystem
    {
        Default = 0,        /**< default place for log messages */
        USB,                /**< serial subsystem log messages  */
        Audio,              /**< audio subsystem log messages   */
        Video               /**< video subsystem log messages   */
    };


    /** defines the loglevels */
    enum LogLevel
    {
        LogLevel_NoLogging      = 0,   /**< disables logging (default setting)                  */
        LogLevel_Basic          = 1,   /**< enables logging of basic parser rules               */
        LogLevel_Extended       = 2,   /**< enables logging of basic + extended parser rules    */
        LogLevel_All            = 4    /**< enables logging of everything, even UART outputs    */
    };


    /** defines the actual loglevel for the logging subsystem */
    void setLogLevel( LogLevel actualLogLevel);

    /** */
    void info( const QString & line, LogLevel minimumLogLevel=LogLevel_Basic );

    /** */
    void warning( const QString & line, LogLevel minimumLogLevel=LogLevel_Basic );

    /** */
    static Logger & instance( Subsystem );


private:
    Logger( const QString & subsystem );

    /** */
    QMessageLogger      m_logger;

    /** subsystem to use for logging */
    QString             m_subsystem;

    /** defines the actual loglevel for this subsystem */
    LogLevel            m_loglevel;

};

} // namespace Waterrower


#ifndef qGeneral
# define qGeneral   Waterrower::Logger::instance( Waterrower::Logger::Default )
#endif


#ifndef qSerial
# define qUSB       Waterrower::Logger::instance( Waterrower::Logger::USB )
#endif


#ifndef qVideo
# define qVideo     Waterrower::Logger::instance( Waterrower::Logger::Video )
#endif


#ifndef qAudio
# define qAudio     Waterrower::Logger::instance( Waterrower::Logger::Audio )
#endif

#endif // LOGGING_H

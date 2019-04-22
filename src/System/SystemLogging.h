#ifndef SYSTEMLOGGING_H
#define SYSTEMLOGGING_H

#include <QString>
#include <QMessageLogger>

	/** */
	class SystemLogging
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
			LogLevel_NoLogging = 0,		/**< disables logging (default setting)                  */
			LogLevel_Basic = 1,			/**< enables logging of basic parser rules               */
			LogLevel_Extended = 2,		/**< enables logging of basic + extended parser rules    */
			LogLevel_All = 4			/**< enables logging of everything, even UART outputs    */
		};


		/** defines the actual loglevel for the logging subsystem */
		void setLogLevel(LogLevel actualLogLevel);

		/** */
		void info(const QString & line, LogLevel minimumLogLevel = LogLevel_Basic);

		/** */
		void warning(const QString & line, LogLevel minimumLogLevel = LogLevel_Basic);

		/** */
		static SystemLogging & instance(Subsystem);


	private:
		SystemLogging(const QString & subsystem);

		/** */
		QMessageLogger      m_logger;

		/** subsystem to use for logging */
		QString             m_subsystem;

		/** defines the actual loglevel for this subsystem */
		LogLevel            m_loglevel;

	};



#ifndef qGeneral
# define qGeneral   SystemLogging::instance( SystemLogging::Default )
#endif				
					
					
#ifndef qSerial		
# define qUSB       SystemLogging::instance( SystemLogging::USB )
#endif				
					
					
#ifndef qVideo		
# define qVideo     SystemLogging::instance( SystemLogging::Video )
#endif				
					
					
#ifndef qAudio		
# define qAudio     SystemLogging::instance( SystemLogging::Audio )
#endif

#endif // SYSTEMLOGGING_H
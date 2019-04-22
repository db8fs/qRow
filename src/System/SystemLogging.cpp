#include "SystemLogging.h"

#include <QDateTime>


	inline QString traceHeader(const QString & subsystem)
	{
		return  QString(QDateTime::currentDateTime().toString(Qt::ISODateWithMs) + QString(" {") + subsystem + QString("} "));
	}


	SystemLogging::SystemLogging(const QString & string)
		: m_logger(),
		m_subsystem(string),
		m_loglevel( SystemLogging::LogLevel_NoLogging )
	{
	}

	void SystemLogging::setLogLevel( SystemLogging::LogLevel actualLogLevel)
	{
		m_loglevel = actualLogLevel;
	}

	void SystemLogging::info(const QString &line, SystemLogging::LogLevel minimumLogLevel)
	{
		if ((int)m_loglevel >= minimumLogLevel)
		{
			m_logger.info("%s %s", traceHeader(m_subsystem).toUtf8().constData(), line.toUtf8().constData());
		}
	}

	void SystemLogging::warning(const QString &line, SystemLogging::LogLevel minimumLogLevel)
	{
		if ((int)m_loglevel >= minimumLogLevel)
		{
			m_logger.warning("%s %s", traceHeader(m_subsystem).toUtf8().constData(), line.toUtf8().constData());
		}
	}

	SystemLogging & SystemLogging::instance(Subsystem subsystem)
	{
		static SystemLogging s_default("General");
		static SystemLogging s_serial("USB");
		static SystemLogging s_audio("Audio");
		static SystemLogging s_video("Video");

		if (subsystem == SystemLogging::USB)
		{
			return s_serial;
		}

		if (subsystem == SystemLogging::Audio)
		{
			return s_audio;
		}

		if (subsystem == SystemLogging::Video)
		{
			return s_video;
		}

		return s_default;
	}


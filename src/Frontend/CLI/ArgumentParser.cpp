

#include "ArgumentParser.h"

#include "SystemParsing.h"
#include <QDebug>


/** enumerates possible subsystem symbols for being parsed */
enum Subsystem
{
	Subsystem_all = 0,
	Subsystem_General,
	Subsystem_USB,
	Subsystem_Audio,
	Subsystem_Video
};


void enableLoggingSubsystem(Subsystem subsystem, SystemLogging::LogLevel loglevel)
{
	switch (subsystem)
	{
	case Subsystem_General:
		qGeneral.setLogLevel(loglevel);
		break;

	case Subsystem_USB:
		qUSB.setLogLevel(loglevel);
		break;

	case Subsystem_Audio:
		qAudio.setLogLevel(loglevel);
		break;

	case Subsystem_Video:
		qVideo.setLogLevel(loglevel);
		break;

	case Subsystem_all:
		qUSB.setLogLevel(loglevel);
		qAudio.setLogLevel(loglevel);
		qVideo.setLogLevel(loglevel);
		qGeneral.setLogLevel(loglevel);
	default:
		break;
	}
}


/** defines the grammar in EBNF approximate syntax for subsystem string parsing
@example usb,audio,video
@example all
@example USB,audio,Video
*/
struct SubsystemParser : Grammar
{
	SubsystemParser( SystemLogging::LogLevel loglevel)
		: SubsystemParser::base_type(start)
	{
		using qi::no_case;

		symbols.add("all", Subsystem_all);
		symbols.add("general", Subsystem_General);
		symbols.add("usb", Subsystem_USB);
		symbols.add("audio", Subsystem_Audio);
		symbols.add("video", Subsystem_Video);

		subsystem %= no_case[symbols[boost::bind(enableLoggingSubsystem, _1, loglevel)]];

		start %= subsystem >> *("," >> subsystem);
	}

	LexerSymbols<Subsystem>  symbols;        /**< grammar rule defining the symbols */
	GrammarRule              subsystem;      /**< grammar rule defining the subsystem keywords */
	GrammarRule              start;          /**< grammar rule defining the start message parsing */
};


/** parses the subsystem commandline string */
QStringList parseSubsystems(const QString & line, SystemLogging::LogLevel loglevel)
{
	String text(line.toStdString());

	Iterator iter(text.begin());
	Iterator end(text.end());

	SubsystemParser parser(loglevel);

	bool success(qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space) && iter == end);

	if (success)
	{
		//return result;
	}
	else
	{
		qCritical() << (QString("Invalid subsystem arguments given: ")) << line;
		exit(1);
	}

	return QStringList();
}


ArgumentParser::ArgumentParser()
	: QCommandLineParser()
{
	QCommandLineParser::setApplicationDescription("qRow - Waterrower ergometer software");
	QCommandLineParser::addHelpOption();
	QCommandLineParser::addVersionOption();
}

void ArgumentParser::process(const QCoreApplication & app)
{
	QCommandLineOption traces(QStringList() << "trace", QCoreApplication::translate("main", "Enables debug traces: 1=Basic (default), 2=Extended, 3=All"), "trace", "1");
	QCommandLineOption subsystems(QStringList() << "subsystems", QCoreApplication::translate("main", "Filters subsystems for tracing, comma-separated. Possible values: all|general|audio|video|usb. All enabled by default. e.g. --subsystems audio,usb"), "subsystems", "all");

	QCommandLineParser::addOption(traces);
	QCommandLineParser::addOption(subsystems);

	QCommandLineParser::process(app);

	SystemLogging::LogLevel loglevel(ArgumentParser::handleTraces(traces));
	ArgumentParser::handleSubsystems(subsystems, loglevel);
}

void ArgumentParser::handleSubsystems(QCommandLineOption & subsystemOption, SystemLogging::LogLevel loglevel)
{
	if (QCommandLineParser::isSet(subsystemOption))
	{
		QString line(QCommandLineParser::value(subsystemOption));

		parseSubsystems(line, loglevel);
	}
	else
	{
		enableLoggingSubsystem(Subsystem_all, loglevel);
	}
}

SystemLogging::LogLevel ArgumentParser::handleTraces(QCommandLineOption & traces)
{
	SystemLogging::LogLevel loglevel( SystemLogging::LogLevel_NoLogging);

	if (QCommandLineParser::isSet(traces))
	{
		QString level(QCommandLineParser::value(traces));
		int debugLevel(0);

		if (!level.isEmpty())
		{
			debugLevel = level.toInt();
		}

		switch (debugLevel)
		{
		case 1:
			loglevel = SystemLogging::LogLevel_Basic;
			break;
		case 2:
			loglevel = SystemLogging::LogLevel_Extended;
			break;
		case 3:
			loglevel = SystemLogging::LogLevel_All;
			break;
		default:
			fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "Error: Invalid trace argument. Must be 1, 2 or 3.")));
			QCommandLineParser::showHelp(1);
			exit(1);
		}
	}

	return loglevel;
}


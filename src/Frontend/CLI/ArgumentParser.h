#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include "SystemLogging.h"

#include <QCommandLineParser>


/** extracts command line arguments for application parametrization */
class ArgumentParser : QCommandLineParser
{
public:
	ArgumentParser();

	/** performs the extraction */
	void process(const QCoreApplication & app);

private:

	/** */
	void handleSubsystems(QCommandLineOption & subsystem, SystemLogging::LogLevel loglevel);

	/** */
	SystemLogging::LogLevel handleTraces(QCommandLineOption & traces);

};

#endif // ARGUMENTPARSER_H


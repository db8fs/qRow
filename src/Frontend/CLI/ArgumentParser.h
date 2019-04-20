#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include "../Backend/System/Logging.h"
#include "../Backend/System/Parsing.h"

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
    void handleSubsystems(QCommandLineOption & subsystem, Waterrower::Logger::LogLevel loglevel);

    /** */
    Waterrower::Logger::LogLevel handleTraces(QCommandLineOption & traces );

};


#endif // ARGUMENTPARSER_H

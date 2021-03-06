
#include "LogReader.h"
#include <QFile>

#include "LogEntry.h"
#include <QVector>

#include "SystemLogging.h"
#include "SystemParsing.h"

BOOST_FUSION_ADAPT_STRUCT
(
    TimeStamp,
    (int, year)
    (int, month)
    (int, day)
    (int, hour)
    (int, minute)
    (int, second)
    (int, microseconds)
)

BOOST_FUSION_ADAPT_STRUCT
(
    DisplayTime,
    (int, hour)
    (int, minute)
    (int, second)
    (int, decisecond)
)

BOOST_FUSION_ADAPT_STRUCT
(
    Measurement,
    (int, heartRateBPM)
    (int, distanceMeters)
    (int, strokeCount)
    (int, averageSpeedCentimetersPerSecond)
    (int, totalSpeed)
)

BOOST_FUSION_ADAPT_STRUCT
(
    Calories,
    (int, caloriesWatts)
    (int, caloriesTotal)
    (int, caloriesUp)
)

BOOST_FUSION_ADAPT_STRUCT
(
    LogEntry,
    (TimeStamp,    timeStamp)
    (DisplayTime,  displayTime)
    (bool, strokeActive)
    (Measurement,  measurement)
    (Calories,     calories)
)


/** context-free grammar for parsing a waterrower.py logfile line using boost::spirit::qi parser generator
    */

template <typename Iterator>
struct LogParser : qi::grammar<Iterator, LogEntry(), ascii::space_type>
{
    LogParser() : LogParser::base_type(start)
    {
        using qi::int_;
        using qi::lit;
        using qi::double_;
        using qi::lexeme;
        using ascii::char_;
        using qi::space;

        timestamp %= int_ >> '-' >>
                        int_ >> '-' >>
                        int_ >>
                        int_ >> ':' >>
                        int_ >> ':' >>
                        int_ >> '.' >>
                        int_;

        displaytime %=  int_ >> ':' >>
                        int_ >> ':' >>
                        int_ >> '.' >>
                        int_;

        strokeactive %= int_;

        measurement  %= int_ >> "[bpm]" >> "Dist:" >>
                        int_ >> "[m]"   >> "Strk:" >>
                        int_ >> "Avg:"  >>
                        int_ >> "[m/s]" >> "Tot:" >>
                        int_;

        calories    %= int_ >> "TotCal:" >>
                        int_ >> "CalUp:" >>
                        int_;

        start %=                             timestamp       >>
                    "|" >> "Tm:"         >>  displaytime     >>
                    "|" >> "StrkSt:"     >>  strokeactive    >>
                    "|" >> "HR:"         >>  measurement     >>
                    "|" >> "CalW:"       >>  calories;

    }

    qi::rule<Iterator, Calories, ascii::space_type>             calories;
    qi::rule<Iterator, Measurement, ascii::space_type>          measurement;
    qi::rule<Iterator, bool, ascii::space_type>                 strokeactive;
    qi::rule<Iterator, DisplayTime, ascii::space_type>          displaytime;
    qi::rule<Iterator, TimeStamp, ascii::space_type>            timestamp;
    qi::rule<Iterator, LogEntry(), ascii::space_type>           start;
};



LogReader::LogReader()
{
}

LogReader::~LogReader()
{
}

void LogReader::read( const QString & filename, QVector<LogEntry> & result )
{
    LogParser<std::string::const_iterator> cfg;

    QFile file( filename );
    file.open( QIODevice::ReadOnly );

    while( file.isOpen() && file.bytesAvailable() )
    {
        std::string content( file.readLine().toStdString() );

        LogEntry entry;

        std::string::const_iterator iter( content.begin() );
        std::string::const_iterator end( content.end() );

        bool r = phrase_parse(iter, end, cfg, boost::spirit::ascii::space, entry);

        if (r && iter == end)
        {
            result.append( entry );
        }
        else
        {
            qGeneral.warning( QString("Invalid line: ") + content.c_str() );
        }
    }
}


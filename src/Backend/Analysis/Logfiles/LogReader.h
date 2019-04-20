
#include <QObject>

#include "LogEntry.h"


/** parses logfile data */
class LogReader : public QObject
{
    Q_OBJECT

public:

    LogReader();
    ~LogReader();

    /** reads the given logfile into a vector of @ref{Waterrower::LogEntry} objects */
    void read(const QString & filename , QVector<Waterrower::LogEntry> &result);

private:


};


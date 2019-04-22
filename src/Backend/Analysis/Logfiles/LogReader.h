
#include <QObject>

#include "LogEntry.h"


/** parses logfile data */
class LogReader : public QObject
{
    Q_OBJECT

public:

    LogReader();
    ~LogReader();

    /** reads the given logfile into a vector of @ref{LogEntry} objects */
    void read(const QString & filename , QVector<LogEntry> &result);

private:


};


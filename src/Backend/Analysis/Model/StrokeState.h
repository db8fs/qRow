#ifndef STROKESTATE_H
#define STROKESTATE_H

#include <QVector>

#include "Phase.h"
#include "LogEntry.h"


/** */
class StrokeState
{
public:

    /** enumerates the possible states of a rowing stroke */
    enum State
    {
        Active = 0,     /**< the active rowing phase when force gets applied */
        Recovery = 1    /**< the recovery phase when returning to the start position */
    };


    /** construction */
    StrokeState( const State );


    /** returns false when state transition needs to be performed */
    bool update(const LogEntry &logEntry);

    /** gets the identifier of this stroke */
    State getState() const;

    /** clears the logentries for this state */
    void clear();

    /** converts the received data into a rowing @ref{Phase} object */
    Phase getPhase() const;

    /** true if no log entries are accumulated yet */
    bool hasLogEntries() const;

private:
    const State                     m_state;        /**< the state identifier of this object     */
    QVector<LogEntry>   m_logEntries;   /**< the log entries belonging to this state */
};


#endif // STROKESTATE_H

#include "StrokeDetector.h"

#include "WorkoutSession.h"

#include "Stroke.h"
#include <QDateTime>
#include <QDebug>

#include <algorithm>




StrokeDetector::StrokeDetector()
    :   QObject(),
        m_active  ( StrokeState::Active   ),
        m_recovery( StrokeState::Recovery ),
        m_currentState( &m_recovery )
{}

StrokeDetector::~StrokeDetector()
{
}

Stroke StrokeDetector::getStroke() const
{
    return Stroke( m_active.getPhase(), m_recovery.getPhase() );
}


void StrokeDetector::onCheckStrokeCompletion()
{
    if( m_recovery.hasLogEntries() && m_active.hasLogEntries() )
    {
        Stroke stroke( m_active.getPhase(), m_recovery.getPhase() );

        Q_EMIT strokeCompleted( stroke );

        m_active.clear();
        m_recovery.clear();
    }
}

void StrokeDetector::update(const Waterrower::LogEntry &logEntry)
{
    if( logEntry.measurement.strokeCount > 0)
    {
        if( !m_currentState->update( logEntry ) )
        {
            if( m_currentState->getState() == StrokeState::Active )
            {
                m_currentState = &m_recovery;
            }
            else
            {
                m_currentState = &m_active;

                onCheckStrokeCompletion();
            }

            // state changed and previous update didn't succeed
            m_currentState->update( logEntry );

            Q_EMIT stateChanged( m_currentState->getState() );
        }
    }
}


#if 0
void StrokeModel::update(const Waterrower::LogEntry &logEntry)
{
    m_private->update( logEntry );

#if 0
    if( isCompleted( m_private->m_currentLogEntry, logEntry ) )
    {
        Phase active(   buildPhase( m_private->m_active.m_logEntries ) );
        Phase recovery( buildPhase( m_private->m_recovery.m_logEntries ) );

        Stroke stroke( active, recovery );

        Q_EMIT strokeCompleted( stroke );

        m_private->m_active.m_logEntries.clear();
        m_private->m_recovery.m_logEntries.clear();
    }

    if( isActive( m_private->m_active.m_logEntries, logEntry ) )
    {
        m_private->m_active.m_logEntries.append( logEntry );
    }
    else
    {
        m_private->m_recovery.m_logEntries.append( logEntry );
    }

    m_private->m_currentLogEntry = logEntry;
#endif
}

#endif


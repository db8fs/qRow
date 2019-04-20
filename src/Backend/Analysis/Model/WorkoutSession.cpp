#include "WorkoutSession.h"




WorkoutSession::WorkoutSession()
    : QObject(),
      m_strokes(),
      m_strokeDetector()
{
    connect( &m_strokeDetector, &StrokeDetector::strokeCompleted, this, &WorkoutSession::addStroke );
}

WorkoutSession::WorkoutSession(const WorkoutSession & rhs)
    : QObject(),
      m_strokes( rhs.m_strokes ),
      m_strokeDetector()
{
    connect( &m_strokeDetector, &StrokeDetector::strokeCompleted, this, &WorkoutSession::addStroke );
}

WorkoutSession::~WorkoutSession()
{
}

QDateTime WorkoutSession::getStartTime() const
{
    if( !m_strokes.empty() )
    {
        return m_strokes[0].getStartTime();
    }
    return QDateTime();
}

QDateTime WorkoutSession::getStopTime() const
{
    if( !m_strokes.empty() )
    {
        return m_strokes[ m_strokes.size()-1 ].getStopTime();
    }
    return QDateTime();
}


int WorkoutSession::getStrokeCount() const
{
    return m_strokes.size();
}

void WorkoutSession::update(const QVector<Waterrower::LogEntry> &logEntries)
{
    for( auto i : logEntries )
    {
        m_strokeDetector.update( i );
    }
}

WorkoutSession &WorkoutSession::operator=(const WorkoutSession & rhs )
{
    if( &rhs != this )
    {
        m_strokes = rhs.m_strokes;
    }

    return *this;
}

const QVector<Stroke> &WorkoutSession::getStrokes() const
{
    return m_strokes;
}


void WorkoutSession::addStroke(const Stroke &stroke)
{
    m_strokes.push_back( stroke );
}

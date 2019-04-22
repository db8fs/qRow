#include "StrokeState.h"

#include "RowingData.h"

#include <QDebug>

Phase buildPhase( const QVector<LogEntry> & entries )
{
    if( entries.size() > 0)
    {
        QVector<QDateTime> timestamps;

        QMap<int, QDateTime> heartRates;
        QMap<int, QDateTime> distances;
        QMap<int, QDateTime> averageSpeeds;
        QMap<int, QDateTime> totalSpeeds;

        for( auto log : entries )
        {
            timestamps.append( log.timeStamp.toQDateTime() );

            if( heartRates.find( log.measurement.heartRateBPM ) == heartRates.end() )
            {
                heartRates[ log.measurement.heartRateBPM ] = log.timeStamp.toQDateTime();
            }

            if( distances.find( log.measurement.distanceMeters ) == distances.end() )
            {
                distances[ log.measurement.distanceMeters ] = log.timeStamp.toQDateTime();
            }

            if( averageSpeeds.find( log.measurement.averageSpeedCentimetersPerSecond ) == averageSpeeds.end() )
            {
                averageSpeeds[ log.measurement.averageSpeedCentimetersPerSecond ] = log.timeStamp.toQDateTime();
            }

            if( totalSpeeds.find( log.measurement.totalSpeed ) == totalSpeeds.end() )
            {
                totalSpeeds[ log.measurement.totalSpeed ] = log.timeStamp.toQDateTime();
            }
        }

        QDateTime startTime( *std::min_element( timestamps.begin(), timestamps.end() ) );
        QDateTime stopTime( *std::max_element( timestamps.begin(), timestamps.end() ) );

        return Phase( RowingData( startTime, stopTime, heartRates, distances, averageSpeeds, totalSpeeds ) );
    }

    return Phase();
}

StrokeState::StrokeState(const State state )
    : m_state( state )
{
}

StrokeState::State StrokeState::getState() const
{
    return m_state;
}

void StrokeState::clear()
{
    m_logEntries.clear();
}

Phase StrokeState::getPhase() const
{
    return buildPhase( m_logEntries );
}

bool StrokeState::hasLogEntries() const
{
    return !m_logEntries.empty();
}


bool StrokeState::update(const LogEntry &logEntry)
{
    if( m_state == State::Active   && logEntry.strokeActive ||
        m_state == State::Recovery && !logEntry.strokeActive )
    {
        m_logEntries.append( logEntry );

#if 0
        qDebug().nospace() << logEntry.timeStamp.year << "-" << logEntry.timeStamp.month << "-" << logEntry.timeStamp.day << " "
                           << logEntry.timeStamp.hour << ":" << logEntry.timeStamp.minute << ":" << logEntry.timeStamp.second << "." << logEntry.timeStamp.microseconds << " "
                           << logEntry.strokeActive << " Nr. " << logEntry.measurement.strokeCount;
#endif
        return true;
    }

    return false;
}


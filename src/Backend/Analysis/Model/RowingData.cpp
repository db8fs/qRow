#include "RowingData.h"

RowingData::RowingData()
    : m_startTime(),
      m_stopTime(),
      m_heartRates(),
      m_distances(),
      m_averageSpeeds(),
      m_totalSpeeds()
{

}

RowingData::RowingData( const QDateTime & startTime,
                         const QDateTime & stopTime,
                         const QMap<int, QDateTime> & heartRates,
                         const QMap<int, QDateTime> & distances,
                         const QMap<int, QDateTime> & averageSpeeds,
                         const QMap<int, QDateTime> & totalSpeeds)
    : m_startTime( startTime ),
      m_stopTime( stopTime ),
      m_heartRates( heartRates ),
      m_distances( distances ),
      m_averageSpeeds( averageSpeeds ),
      m_totalSpeeds( totalSpeeds )
{
}

const QDateTime &RowingData::getStartTime() const
{
    return m_startTime;
}

const QDateTime &RowingData::getStopTime() const
{
    return m_stopTime;
}

#if 0
QMap<int, QDateTime> mergeData( const QMap<int, QDateTime> & lhs, const QMap<int, QDateTime> & rhs )
{
    QMap<int, QDateTime> result( lhs );

    for( QMap<int, QDateTime>::const_iterator it( rhs.begin() ); it != rhs.end(); ++it )
    {
        result.insert( it->key(), it->value() );
    }

    return result;
}

RowingData::RowingData(const RowingData &lhs, const RowingData &rhs)
    : m_startTime( std::min( lhs.m_startTime, rhs.m_startTime) ),
      m_stopTime( std::max( lhs.m_stopTime, rhs.m_stopTime ) ),
      m_heartRates( mergeData( lhs.m_heartRates, rhs.m_heartRates) ),
      m_distances( mergeData( lhs.m_distances, rhs.m_distances) ),
      m_averageSpeeds( mergeData( lhs.m_averageSpeeds, rhs.m_averageSpeeds ) ),
      m_totalSpeeds( mergeData( lhs.m_totalSpeeds, rhs.m_totalSpeeds ) )
{
}

#endif

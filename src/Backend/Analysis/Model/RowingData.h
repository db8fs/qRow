#ifndef ROWINGDATA_H
#define ROWINGDATA_H

#include <QDateTime>
#include <QMap>

/** */
class RowingData
{
public:
    RowingData();

    RowingData( const QDateTime & startTime,
                     const QDateTime & stopTime,
                     const QMap<int, QDateTime> & heartRates,
                     const QMap<int, QDateTime> & distances,
                     const QMap<int, QDateTime> & averageSpeeds,
                     const QMap<int, QDateTime> & totalSpeeds );

    /** merges the given objects into a common interval */
    RowingData( const RowingData & lhs, const RowingData & rhs );

    friend class Phase;

    const QDateTime & getStartTime() const;
    const QDateTime & getStopTime() const;

private:
    QDateTime m_startTime;
    QDateTime m_stopTime;
    QMap<int, QDateTime> m_heartRates;
    QMap<int, QDateTime> m_distances;
    QMap<int, QDateTime> m_averageSpeeds;
    QMap<int, QDateTime> m_totalSpeeds;
};



#endif // ROWINGDATA_H

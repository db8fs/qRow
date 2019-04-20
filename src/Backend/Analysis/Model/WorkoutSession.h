#ifndef WORKOUTSESSION_H
#define WORKOUTSESSION_H

#include <QAbstractListModel>
#include <QVector>
#include "Stroke.h"
#include "StrokeDetector.h"

/** */
class WorkoutSession : public QObject
{
    Q_OBJECT

public:
    WorkoutSession();
    WorkoutSession(const WorkoutSession & rhs);
    ~WorkoutSession();

    QDateTime getStartTime() const;
    QDateTime getStopTime() const;

    int getStrokeCount() const;

    int getDistance();
    int getAverageHeartRate();
    int getStrokesPerMinute();

    int getMinimumSpeed();
    int getMaximumSpeed();
    int getAverageSpeed();

    void update( const QVector<Waterrower::LogEntry> & logEntries );

    WorkoutSession & operator=( const WorkoutSession & );

    const QVector<Stroke> & getStrokes() const;


private Q_SLOTS:
    void addStroke( const Stroke & stroke );


private:
    QVector<Stroke> m_strokes;
    StrokeDetector  m_strokeDetector;

};

#endif // WORKOUTSESSION_H

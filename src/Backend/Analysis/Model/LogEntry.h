#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <QDateTime>

namespace Waterrower
{
    struct TimeStamp
    {
        TimeStamp() : year(0), month(0), day(0), hour(0), minute(0), second(0), microseconds(0) {}

        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
        int microseconds;

        QDateTime toQDateTime() const
        {
            return QDateTime( QDate( year, month, day), QTime( hour, minute, second, microseconds / 1000) );
        }
    };

    struct DisplayTime
    {
        DisplayTime()   : hour(0), minute(0), second(0), decisecond(0) {}

        int hour;
        int minute;
        int second;
        int decisecond;
    };

    struct Measurement
    {
        Measurement()   : heartRateBPM(0), distanceMeters(0), strokeCount(0), averageSpeedCentimetersPerSecond(0), totalSpeed(0) {}

        int heartRateBPM;
        int distanceMeters;
        int strokeCount;
        int averageSpeedCentimetersPerSecond;
        int totalSpeed;
    };

    struct Calories
    {
        Calories()  : caloriesWatts(0), caloriesTotal(0), caloriesUp(0) {}

        int caloriesWatts;
        int caloriesTotal;
        int caloriesUp;
    };

    struct LogEntry
    {
        TimeStamp       timeStamp;
        DisplayTime     displayTime;
        bool            strokeActive;
        Measurement     measurement;
        Calories        calories;
    };

} // namespace Waterrower

#endif // LOGENTRY_H

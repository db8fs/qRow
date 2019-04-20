#ifndef PHASE_H
#define PHASE_H

#include <QObject>
#include <QVector>
#include "LogEntry.h"
#include "RowingData.h"


/** */
class Phase : public QObject
{
    Q_OBJECT

    RowingData m_dataSamples;

public:
    Phase();
    Phase( const RowingData & rowingData );
    Phase( const Phase & rhs);
    ~Phase();

    const QDateTime & getStartTime() const;
    const QDateTime & getStopTime() const;

    /** */
    Phase & operator=( const Phase & rhs );

    void plot() const;
};


#endif // PHASE_H

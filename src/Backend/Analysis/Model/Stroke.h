#ifndef STROKE_H
#define STROKE_H

#include <QObject>
#include "Phase.h"
#include "LogEntry.h"


/** */
class Stroke : public QObject
{
    Q_OBJECT

    Phase m_active;
    Phase m_recovery;

public:
    Stroke();
    Stroke( const Phase & active, const Phase & recovery );
    Stroke(const Stroke &rhs);
    ~Stroke();

    Stroke & operator=( const Stroke & rhs );

    const QDateTime &getStartTime() const;
    const QDateTime &getStopTime() const;

    /** */
    void plot() const;

};

#endif // STROKE_H

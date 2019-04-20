#ifndef STROKEDETECTOR_H
#define STROKEDETECTOR_H

#include "LogEntry.h"
#include <QObject>

#include "Stroke.h"
#include "StrokeState.h"


/** statemachine for stroke phase detection */
class StrokeDetector : public QObject
{
    Q_OBJECT

public:

    StrokeDetector();
    ~StrokeDetector();

    /** performs state transitions and updates stroke states */
    void update(const Waterrower::LogEntry &logEntry);

    /** gets the current (possibly incomplete) stroke */
    Stroke getStroke() const;

private Q_SLOTS:
    void onCheckStrokeCompletion();

private:
    StrokeState             m_active;               /**< stroke state describing the active rowing phase, when force gets applied */
    StrokeState             m_recovery;             /**< stroke state describing the recovery rowing phase */

    StrokeState*            m_currentState;         /**< current state of the rowing model */

Q_SIGNALS:

    /** */
    void stateChanged( StrokeState::State );

    void strokeCompleted( const Stroke & stroke );
};


#endif // STROKEDETECTOR_H

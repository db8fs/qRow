#ifndef WORKOUTCONTROLLER_H
#define WORKOUTCONTROLLER_H

#include <QObject>
#include <QList>

#include "WorkoutSession.h"
#include "StrokeDetector.h"


/** controls workout sessions */
class WorkoutController : public QObject
{
    Q_OBJECT

public:
    WorkoutController( class SessionModel & sessionModel, class WorkoutModel & workoutModel );
    ~WorkoutController();

    void importLogfile( const QString& path );

    void start();
    void stop();

    QVector<WorkoutSession> getSessions();

public Q_SLOTS:

    /** loads the session with the given @ref{index} of the @ref{SessionModel} into the @ref{WorkoutModel} */
    void setSession( const QModelIndex & index );

private:
    /** the data model holding all workout sessions */
    class SessionModel &   m_sessionModel;

    /** the data model for one selected workout session */
    class WorkoutModel &   m_workoutModel;

    /** */
    WorkoutSession  m_currentSession;

};

#endif // WORKOUTCONTROLLER_H

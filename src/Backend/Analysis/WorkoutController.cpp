#include "WorkoutController.h"

#include "Logfiles/LogReader.h"

#include "Model/SessionModel.h"
#include "Model/WorkoutSession.h"
#include "Model/WorkoutModel.h"

WorkoutController::WorkoutController(SessionModel &sessionModel, WorkoutModel & workoutModel )
    : QObject(),
      m_sessionModel( sessionModel ),
      m_workoutModel( workoutModel ),
      m_currentSession()
{
}

WorkoutController::~WorkoutController()
{
}

void WorkoutController::importLogfile(const QString &path)
{
    WorkoutSession  session;

    LogReader reader;
    QVector<LogEntry> logEntries;

    reader.read( path, logEntries );

    session.update( logEntries );

    m_sessionModel.addSession( session );
}

void WorkoutController::start()
{
    // start serial port listening

}

void WorkoutController::stop()
{
    // stop serial port listening
}

void WorkoutController::setSession(const QModelIndex &index)
{
    m_workoutModel.loadSession( m_sessionModel.getSession( index ) );
}


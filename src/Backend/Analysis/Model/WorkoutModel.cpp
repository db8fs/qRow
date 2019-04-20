#include "WorkoutModel.h"

#include "WorkoutSession.h"

WorkoutModel::WorkoutModel()
    : QAbstractListModel(),
      m_currentSession( nullptr )
{
}

WorkoutModel::~WorkoutModel()
{
}

int WorkoutModel::rowCount(const QModelIndex &parent) const
{
    if( nullptr != m_currentSession )
    {
        return m_currentSession->getStrokeCount();
    }

    return 0;
}

QVariant WorkoutModel::data(const QModelIndex &index, int role) const
{
    if( m_currentSession == nullptr )
        return QVariant();

    QVector<Stroke> strokes( m_currentSession->getStrokes() );

    if( index.row() < 0 || index.row() >= strokes.count() )
        return QVariant();

    const Stroke & stroke( strokes[ index.row() ] );

    switch ( role  )
    {
    case IndexRole:
        return QVariant(index);
    case StartTimeRole:
        return QVariant( stroke.getStartTime() );
    case StopTimeRole:
        return QVariant( stroke.getStopTime() );
    }

    return QVariant();
}

QHash<int, QByteArray> WorkoutModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ WorkoutModel::IndexRole ] = "index";
    roles[ WorkoutModel::StartTimeRole ] = "starttime";
    roles[ WorkoutModel::StopTimeRole ] = "stoptime";
    return roles;
}

void WorkoutModel::loadSession(const WorkoutSession *session)
{
    if( m_currentSession != session )
    {
        m_currentSession = session;
        Q_EMIT layoutChanged();
    }
}

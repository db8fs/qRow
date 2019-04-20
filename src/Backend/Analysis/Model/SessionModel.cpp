#include "SessionModel.h"

SessionModel::SessionModel()
    : QAbstractListModel(),
      m_sessions()
{
}

SessionModel::~SessionModel()
{
}

void SessionModel::addSession(const class WorkoutSession & session )
{
    m_sessions.append( session );
}

int SessionModel::rowCount(const QModelIndex &parent) const
{
    return m_sessions.count();
}

QVariant SessionModel::data(const QModelIndex &index, int role) const
{
    if( index.row() < 0 || index.row() >= m_sessions.count() )
        return QVariant();

    const WorkoutSession & session( m_sessions[ index.row() ] );

    switch ( role  )
    {
    case IndexRole:
        return QVariant(index);
    case StartTimeRole:
        return QVariant( session.getStartTime() );
    case StopTimeRole:
        return QVariant( session.getStopTime() );
    }

    return QVariant();
}

QHash<int, QByteArray> SessionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ SessionModel::IndexRole ] = "index";
    roles[ SessionModel::StartTimeRole ] = "starttime";
    roles[ SessionModel::StopTimeRole ] = "stoptime";
    return roles;
}

const WorkoutSession* SessionModel::getSession(const QModelIndex &index) const
{
    if( index.row() < 0 || index.row() >= m_sessions.count() )
        return nullptr;

    return &m_sessions[ index.row() ];
}

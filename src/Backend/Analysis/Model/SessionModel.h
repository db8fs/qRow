#ifndef SESSIONMODEL_H
#define SESSIONMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "WorkoutSession.h"

/** data model for managing workout sessions */
class SessionModel : public QAbstractListModel
{
    Q_OBJECT

    QVector<WorkoutSession> m_sessions;

public:
    SessionModel();
    ~SessionModel();

    enum WorkoutRoles
    {
        IndexRole = Qt::UserRole + 1,
        StartTimeRole = Qt::UserRole + 2,
        StopTimeRole = Qt::UserRole + 3,
    };

    /** adds the given session to the workout model */
    void addSession( const class WorkoutSession & session );

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QHash<int, QByteArray> roleNames() const;

    const class WorkoutSession* getSession(const QModelIndex &index) const;

};


#endif // SESSIONMODEL_H

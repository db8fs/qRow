#ifndef WORKOUTMODEL_H
#define WORKOUTMODEL_H

#include <QAbstractListModel>

/** */
class WorkoutModel : public QAbstractListModel
{
    Q_OBJECT

public:
    WorkoutModel();
    WorkoutModel(const WorkoutModel & rhs);
    ~WorkoutModel();

    enum SessionRoles
    {
        IndexRole = Qt::UserRole + 1,
        StartTimeRole = Qt::UserRole + 2,
        StopTimeRole = Qt::UserRole + 3,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    /** loads the given session detail into this workout model */
    void loadSession( const class WorkoutSession* session );

Q_SIGNALS:
    void sessionChanged();

private:
    const class WorkoutSession*     m_currentSession;

};

#endif // WORKOUTMODEL_H

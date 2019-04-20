#include "Stroke.h"

#include <QDebug>

Stroke::Stroke()
 : m_active(),
   m_recovery()
{
}

Stroke::Stroke(const Phase &active, const Phase &recovery)
    : m_active( active ),
      m_recovery( recovery )
{
}

Stroke::Stroke(const Stroke & rhs)
    : m_active( rhs.m_active ),
      m_recovery( rhs.m_recovery )
{
}

Stroke::~Stroke()
{
}

Stroke &Stroke::operator=(const Stroke &rhs)
{
    if( this != &rhs )
    {
        m_active = rhs.m_active;
        m_recovery = rhs.m_recovery;
    }

    return *this;
}

const QDateTime & Stroke::getStartTime() const
{
    return m_active.getStartTime();
}

const QDateTime &Stroke::getStopTime() const
{
    return m_recovery.getStopTime();
}

void Stroke::plot() const
{
    qDebug() << "\nSTROKE START";
    m_active.plot();
    qDebug() << "RECOVERY";
    m_recovery.plot();
    qDebug() << "STROKE STOP\n\n";
}


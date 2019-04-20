#include "Phase.h"

#include <QDebug>

Phase::Phase()
    : m_dataSamples()
{
}

Phase::Phase( const RowingData & data)
    : m_dataSamples( data )
{

}

Phase::Phase(const Phase & rhs )
    : m_dataSamples( rhs.m_dataSamples )
{
}

Phase::~Phase()
{
}

const QDateTime & Phase::getStartTime() const
{
    return m_dataSamples.getStartTime();
}

const QDateTime &Phase::getStopTime() const
{
    return m_dataSamples.getStopTime();
}

Phase & Phase::operator=(const Phase &rhs)
{
    if( this != &rhs )
    {
        m_dataSamples = rhs.m_dataSamples;
    }

    return *this;
}


void Phase::plot() const
{
    qDebug().nospace() << "Start: " << m_dataSamples.m_startTime.date().year() << "-" << m_dataSamples.m_startTime.date().month() << "-" << m_dataSamples.m_startTime.date().day()
                       << " " << m_dataSamples.m_startTime.time().hour() << ":" << m_dataSamples.m_startTime.time().minute() << ":" << m_dataSamples.m_startTime.time().second() << "." << m_dataSamples.m_startTime.time().msec() << " "
                       << "Stop: " << m_dataSamples.m_stopTime.date().year() << "-" << m_dataSamples.m_stopTime.date().month() << "-" << m_dataSamples.m_stopTime.date().day()
                       << " " << m_dataSamples.m_stopTime.time().hour() << ":" << m_dataSamples.m_stopTime.time().minute() << ":" << m_dataSamples.m_stopTime.time().second() << "." << m_dataSamples.m_stopTime.time().msec();
}




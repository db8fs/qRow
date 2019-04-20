#include "WaterrowerController.h"

#include <QDebug>
#include <QDateTime>

#include "WaterrowerSerial.h"

using namespace Waterrower::Serial;

class Reader_Private
{
public:

    WaterrowerVersion       Version;                    /**< model+software version */
    WaterrowerCalibration   Calibration;                /**< calibration data */

    int AverageSpeed = {0};                             /**< averaged speed [cm/s] */
    int HeartRate = {0};                                /**< beats per minute */
    int Distance = {0};                                 /**< meters */
    int TankVolume = {0};                               /**< deciliters */
    int CaloriesTotal = {0};                            /**< total of worked calories */
    int ActualSpeed = {0};                              /**< actual speed [cm/s] */
    int Strokes = {0};                                  /**< strokes */
    int Age = {35};                                     /**< age */
    WaterrowerController::HeartRateZone Zone = { WaterrowerController::NoTraining };    /**< actual training zone */

};


WaterrowerController::WaterrowerController(QObject *parent)
    : QObject(parent),
      m_private( new Reader_Private() ),
      m_lowPriorityTimer(),
      m_highPriorityTimer(),
      m_serialAdapter()
{
    connect( &m_serialAdapter, &SerialAdapter::waterrowerDisconnected, this, &WaterrowerController::onWaterrowerDisconnected );
    connect( &m_serialAdapter, &SerialAdapter::waterrowerLineReceived, this, &WaterrowerController::onWaterrowerLineReceived );
    connect( &m_highPriorityTimer, &QTimer::timeout, this, &WaterrowerController::onHighPriorityPollRequest );
    connect( &m_lowPriorityTimer, &QTimer::timeout, this, &WaterrowerController::onLowPriorityPollRequest );
}

WaterrowerController::~WaterrowerController()
{
    delete m_private;
}

int WaterrowerController::distance() const
{
    return m_private->Distance;
}

int WaterrowerController::actualSpeed() const
{
    return m_private->ActualSpeed;
}

int WaterrowerController::averageSpeed() const
{
    return m_private->AverageSpeed;
}

int WaterrowerController::modelVersion() const
{
    return m_private->Version.model;
}

int WaterrowerController::majorVersion() const
{
    return m_private->Version.major;
}

int WaterrowerController::minorVersion() const
{
    return m_private->Version.minor;
}

int WaterrowerController::tankVolume() const
{
    return m_private->TankVolume;
}

int WaterrowerController::heartRate() const
{
    return m_private->HeartRate;
}

int WaterrowerController::caloriesTotal() const
{
    return m_private->CaloriesTotal;
}

int WaterrowerController::strokes() const
{
    return m_private->Strokes;
}

int WaterrowerController::maximumPulse() const
{
    // Tanaka equation, based on linear regression for maximum pulse estimation
    const float theta_0( 208.f );
    const float theta_1( -0.7f );

    return qRound( theta_0 + theta_1 * (float) m_private->Age );
}

WaterrowerController::HeartRateZone WaterrowerController::paceZone() const
{
    float percent( (100.f * m_private->HeartRate) / maximumPulse() );

    HeartRateZone zone( NoTraining );

    if( percent >= 90.f )
    {
        zone = Anearobic_Sprint_90_100_Percent;
    }
    else if( percent >= 80.f )
    {
        zone = Anaerobic_Fast_80_90_Percent;
    }
    else if( percent >= 70.f )
    {
        zone = Aerobic_Moderate_70_80_Percent;
    }
    else if( percent >= 60.f )
    {
        zone = Aerobic_Comfortable_60_70_Percent;
    }
    else if ( percent >= 50.f )
    {
        zone = Aerobic_Easy_50_60_Percent;
    }

    return zone;
}

int WaterrowerController::age() const
{
    return m_private->Age;
}

void WaterrowerController::setAge(int age)
{
    if( m_private->Age != age )
    {
        m_private->Age = age;
        Q_EMIT ageChanged();
        Q_EMIT maximumPulseChanged();
    }
}


void WaterrowerController::onWaterrowerLineReceived(const QString & line)
{
    Waterrower::Serial::receive( *this, line );
}

void WaterrowerController::onWaterrowerDisconnected()
{
    m_highPriorityTimer.stop();
    m_lowPriorityTimer.stop();
    Q_EMIT waterrowerDisconnected();
}

void WaterrowerController::onHighPriorityPollRequest()
{
    Waterrower::Serial::sendRequestsHighPriority( m_serialAdapter );
}


void WaterrowerController::onLowPriorityPollRequest()
{
    Waterrower::Serial::sendRequestsLowPriority( m_serialAdapter );
}


void WaterrowerController::onWaterrowerConnected()
{
    Waterrower::Serial::sendRequestVersion( m_serialAdapter );

    // initialize the process variables of this data model by querying initial values on connection and updating them onChange
    WaterrowerController::onHighPriorityPollRequest();
    WaterrowerController::onLowPriorityPollRequest();

    m_highPriorityTimer.start( 500 );
    m_lowPriorityTimer.start( 15000 );

    Q_EMIT waterrowerConnected();
}



void WaterrowerController::onPing()
{
    if( m_private->ActualSpeed != 0 )
    {
        m_private->ActualSpeed = 0;
        Q_EMIT actualSpeedChanged();
    }
}

void WaterrowerController::onPulseCount( int pulsesPer25msec )
{
    const int       SAMPLE_WINDOW_MSEC( 25 );
    const int       MILLISECONDS_PER_SECOND( 1000 );
    const int       SAMPLES_PER_SEC( MILLISECONDS_PER_SECOND / SAMPLE_WINDOW_MSEC );

    const double    CM_PER_PULSE( m_private->Calibration.getDistancePerPin() );

    double          cmPerSample( pulsesPer25msec * CM_PER_PULSE );
    double          cmPerSecond( cmPerSample * SAMPLES_PER_SEC );

    int actualSpeed( qRound( cmPerSecond ) );

    if( actualSpeed != m_private->ActualSpeed )
    {
        m_private->ActualSpeed = actualSpeed;

        Q_EMIT actualSpeedChanged();
    }
}


void WaterrowerController::onHeartrateReceived( int heartRate )
{
    if( heartRate != m_private->HeartRate )
    {
        HeartRateZone zone( paceZone() );

        if( zone != m_private->Zone )
        {
            m_private->Zone = zone;
            Q_EMIT paceZoneChanged();
        }

        m_private->HeartRate = heartRate;
        Q_EMIT heartRateChanged();
    }
}

void WaterrowerController::onAverageSpeedReceived( int averageSpeed )
{
    if( averageSpeed != m_private->AverageSpeed )
    {
        m_private->AverageSpeed = averageSpeed;
        Q_EMIT averageSpeedChanged();
    }
}

void WaterrowerController::onDistanceReceived( int distanceMeters )
{
    if( distanceMeters != m_private->Distance )
    {
        m_private->Distance = distanceMeters;
        Q_EMIT distanceChanged();
    }
}

void WaterrowerController::onStrokeStart()
{
    Q_EMIT strokeStarted();
}

void WaterrowerController::onStrokeEnd()
{
    Q_EMIT strokeEnded();
}

void WaterrowerController::onVersionInfo( const Waterrower::Serial::WaterrowerVersion & version)
{
    if( version.model != m_private->Version.model ||
        version.major != m_private->Version.major ||
        version.minor != m_private->Version.minor )
    {
        m_private->Version = version;

        Q_EMIT versionChanged();
    }
}

void WaterrowerController::onKeyPadInteraction( Waterrower::Serial::KeyPadInteraction button)
{
        Q_EMIT keyPadInteraction(button );
}

void WaterrowerController::onTankVolumeReceived(int volume )
{
    if( volume != m_private->TankVolume && volume > 0 )
    {
        m_private->TankVolume = volume;

        Q_EMIT tankVolumeChanged();
    }
}

void WaterrowerController::onCalibrationPinsPerXXcm(int pinsPerXXcm)
{
    if( pinsPerXXcm != m_private->Calibration.pinsPerXXcm && pinsPerXXcm > 0 )
    {
        m_private->Calibration.pinsPerXXcm = pinsPerXXcm;

        Q_EMIT calibrationChanged( m_private->Calibration.pinsPerXXcm, m_private->Calibration.distanceXXcm );
    }
}

void WaterrowerController::onCalibrationDistanceXXcm(int distanceXXcm )
{
    if( distanceXXcm != m_private->Calibration.distanceXXcm && distanceXXcm > 0 )
    {
        m_private->Calibration.distanceXXcm = distanceXXcm;

        Q_EMIT calibrationChanged( m_private->Calibration.pinsPerXXcm, m_private->Calibration.distanceXXcm );
    }
}

void WaterrowerController::onTotalCaloriesReceived(int calories )
{
    if( calories != m_private->CaloriesTotal)
    {
        m_private->CaloriesTotal = calories;

        Q_EMIT caloriesTotalChanged();
    }
}

void WaterrowerController::onStrokesReceived(int strokes)
{
    if( strokes != m_private->Strokes )
    {
        m_private->Strokes = strokes;

        Q_EMIT strokesChanged();
    }
}




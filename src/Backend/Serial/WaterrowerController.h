#ifndef WATERROWERCONTROLLER_H
#define WATERROWERCONTROLLER_H

#include "WaterrowerProtocol.h"
#include "IProtocolHandler.h"
#include "SerialAdapter.h"
#include "../System/Logging.h"
#include <QObject>


/** responsible for receiving and translating serial USB communication into Qt signals */
class WaterrowerController : public QObject,
                             private Waterrower::Serial::IProtocolHandler
{
public:


    /** enumerates the possible pace zones for training */
    enum HeartRateZone
    {
        NoTraining = 0,                         /**< Normal pulse frequency, no training effect at all.                     */
        Aerobic_Easy_50_60_Percent,             /**< Training: beginning-level aerobic training, stress reduction           */
        Aerobic_Comfortable_60_70_Percent,      /**< Training: basic cardiovascular training, good recovery pace            */
        Aerobic_Moderate_70_80_Percent,         /**< Training: improved aerobic capacity, optimal cardiovascular training   */
        Anaerobic_Fast_80_90_Percent,           /**< Training: Improved anaerobic capacity and threshold, improved speed    */
        Anearobic_Sprint_90_100_Percent         /**< Training: Anaerobic and muscular endurance, increased power            */
    };

private:

    Q_OBJECT

    Q_ENUM( HeartRateZone )

    Q_PROPERTY( int distance READ distance NOTIFY distanceChanged )
    Q_PROPERTY( int actualSpeed READ actualSpeed NOTIFY actualSpeedChanged )
    Q_PROPERTY( int averageSpeed READ averageSpeed NOTIFY averageSpeedChanged )
    Q_PROPERTY( int modelVersion READ modelVersion NOTIFY versionChanged )
    Q_PROPERTY( int majorVersion READ majorVersion NOTIFY versionChanged )
    Q_PROPERTY( int minorVersion READ minorVersion NOTIFY versionChanged )
    Q_PROPERTY( int tankVolume READ tankVolume NOTIFY tankVolumeChanged )
    Q_PROPERTY( int heartRate READ heartRate NOTIFY heartRateChanged )
    Q_PROPERTY( int caloriesTotal READ caloriesTotal NOTIFY caloriesTotalChanged )
    Q_PROPERTY( int strokes READ strokes NOTIFY strokesChanged )
    Q_PROPERTY( HeartRateZone paceZone READ paceZone NOTIFY paceZoneChanged )
    Q_PROPERTY( int age READ age WRITE setAge NOTIFY ageChanged )
    Q_PROPERTY( int maximumPulse READ maximumPulse NOTIFY maximumPulseChanged )

public:
    explicit WaterrowerController(QObject *parent = nullptr);

    ~WaterrowerController();


    int distance() const;
    int actualSpeed() const;
    int averageSpeed() const;

    int modelVersion() const;
    int majorVersion() const;
    int minorVersion() const;

    int tankVolume() const;
    int heartRate() const;
    int caloriesTotal() const;
    int strokes() const;

    int maximumPulse() const;

    HeartRateZone paceZone() const;

    int age() const;
    void setAge(int age);

signals:

    /** signals that a waterrower rowing ergometer has connected to the USB port */
    void waterrowerConnected();

    /** signals that a waterrower rowing ergometer has disconnected from the USB port */
    void waterrowerDisconnected();

    /** signals that the waterrower version message changed */
    void versionChanged();

    /** signals that the waterrower's distance register changed */
    void distanceChanged();

    /** high-frequency updates of the actual rowing speed [cm/s] (max. 40 Hz) */
    void actualSpeedChanged();

    /** signals that the waterrower's average speed register changed [cm/s] */
    void averageSpeedChanged();

    /** signals that the waterrower's heartrate register changed */
    void heartRateChanged();

    /** signals the the waterrower's tankvolume register changed */
    void tankVolumeChanged();

    /** signals that a stroke is being started */
    void strokeStarted();

    /** signals that a stroke has been completed */
    void strokeEnded();

    /** signals that a keypad interaction happened */
    void keyPadInteraction( Waterrower::Serial::KeyPadInteraction );

    /** signals the change of the internal waterrower calibration */
    void calibrationChanged( int pinsPerXXcm, int distanceXXcm );

    /** signals the change of the total amount of burned calories */
    void caloriesTotalChanged();

    /** signals stroke changes */
    void strokesChanged();

    /** signals zone changes */
    void paceZoneChanged();

    /** signals changes of the age within the data model */
    void ageChanged();

    /** signals changes of the maximum pulse */
    void maximumPulseChanged();

private slots:

    /** line received from the waterrower ergometer via the USB-UART */
    void onWaterrowerLineReceived( const QString & );

    /** handles disconnect events */
    void onWaterrowerDisconnected();

    /** polls the waterrower monitor only few times per sec */
    void onLowPriorityPollRequest();

    /** polls the waterrower ergometer for extended data packets */
    void onHighPriorityPollRequest();

private:

    /* -- Waterrower::Serial::IProtocolHandler -- */

    void onWaterrowerConnected();
    void onPing();
    void onPulseCount( int pulsesPer25msec );
    void onHeartrateReceived( int heartRate );
    void onAverageSpeedReceived( int averageSpeed );
    void onDistanceReceived( int distanceMeters );
    void onStrokeStart();
    void onStrokeEnd();
    void onVersionInfo(const Waterrower::Serial::WaterrowerVersion &version );
    void onKeyPadInteraction(Waterrower::Serial::KeyPadInteraction );
    void onTankVolumeReceived( int );
    void onCalibrationPinsPerXXcm( int );
    void onCalibrationDistanceXXcm( int );
    void onTotalCaloriesReceived(int);
    void onStrokesReceived( int strokes );

private:

    class Reader_Private* m_private;

    QTimer                              m_lowPriorityTimer;     /**< timer for requesting low priority ergometer monitor data */
    QTimer                              m_highPriorityTimer;    /**< timer for requesting extended ergometer computer data with higher priority  */
    Waterrower::Serial::SerialAdapter   m_serialAdapter;

    friend class Reader_Private;
};


#endif // WATERROWERCONTROLLER_H

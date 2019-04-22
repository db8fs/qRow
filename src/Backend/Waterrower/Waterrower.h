#ifndef WATERROWERCONTROLLER_H
#define WATERROWERCONTROLLER_H

#include "Transceiver/ProtocolV4/IProtocolHandler.h"
#include "Transceiver/Serial/SerialAdapter.h"
#include <QObject>

/** responsible for receiving and translating serial USB communication into Qt signals */
class Waterrower : public QObject
{
    Q_OBJECT

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

		Q_ENUM(HeartRateZone)

		Q_PROPERTY(int distance READ distance NOTIFY distanceChanged)
		Q_PROPERTY(int actualSpeed READ actualSpeed NOTIFY actualSpeedChanged)
		Q_PROPERTY(int averageSpeed READ averageSpeed NOTIFY averageSpeedChanged)
		Q_PROPERTY(int modelVersion READ modelVersion NOTIFY versionChanged)
		Q_PROPERTY(int majorVersion READ majorVersion NOTIFY versionChanged)
		Q_PROPERTY(int minorVersion READ minorVersion NOTIFY versionChanged)
		Q_PROPERTY(int tankVolume READ tankVolume NOTIFY tankVolumeChanged)
		Q_PROPERTY(int heartRate READ heartRate NOTIFY heartRateChanged)
		Q_PROPERTY(int caloriesTotal READ caloriesTotal NOTIFY caloriesTotalChanged)
		Q_PROPERTY(int strokes READ strokes NOTIFY strokesChanged)
		Q_PROPERTY(HeartRateZone paceZone READ paceZone NOTIFY paceZoneChanged)
		Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)
		Q_PROPERTY(int maximumPulse READ maximumPulse NOTIFY maximumPulseChanged)

public:
	explicit Waterrower(QObject *parent = nullptr);

	~Waterrower();


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
	void keyPadInteraction( int );

	/** signals the change of the internal waterrower calibration */
	void calibrationChanged(int pinsPerXXcm, int distanceXXcm);

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



private:

    class WaterrowerModelV4* const		m_model;

	class ProtocolV4Adapter* const	m_protocolAdapter;

	class Transceiver* const		m_transceiver;
};


#endif // WATERROWERCONTROLLER_H

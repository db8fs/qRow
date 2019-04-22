#include "../Waterrower.h"
#include "WaterrowerModelV4.h"

struct DataModelV4_Private
{
	WaterrowerVersion       Version;                    /**< model+software version */
	WaterrowerCalibration   Calibration;                /**< calibration data */

	int AverageSpeed = { 0 };                             /**< averaged speed [cm/s] */
	int HeartRate = { 0 };                                /**< beats per minute */
	int Distance = { 0 };                                 /**< meters */
	int TankVolume = { 0 };                               /**< deciliters */
	int CaloriesTotal = { 0 };                            /**< total of worked calories */
	int ActualSpeed = { 0 };                              /**< actual speed [cm/s] */
	int Strokes = { 0 };                                  /**< strokes */
	int Age = { 35 };                                     /**< age */
	Waterrower::HeartRateZone Zone = { Waterrower::NoTraining };    /**< actual training zone */
};


/////


WaterrowerModelV4::WaterrowerModelV4()
	: AbstractWaterrowerModel(),
	  m_private( new DataModelV4_Private() )
{
}

WaterrowerModelV4::~WaterrowerModelV4()
{
	delete m_private;
}




void WaterrowerModelV4::onWaterrowerConnected()
{
	Q_EMIT AbstractWaterrowerModel::waterrowerConnected();
}


void WaterrowerModelV4::onHeartrateReceived(int heartRate)
{
	if (heartRate != m_private->HeartRate)
	{
		Waterrower::HeartRateZone zone( (Waterrower::HeartRateZone) this->getPaceZone() );

		if (zone != m_private->Zone)
		{
			m_private->Zone = zone;
			Q_EMIT AbstractWaterrowerModel::paceZoneChanged();
		}

		m_private->HeartRate = heartRate;
		Q_EMIT AbstractWaterrowerModel::heartRateChanged();
	}
}

void WaterrowerModelV4::setActualSpeedCmPerSec(int actualSpeed)
{
	if (actualSpeed != m_private->ActualSpeed)
	{
		m_private->ActualSpeed = actualSpeed;

		Q_EMIT AbstractWaterrowerModel::actualSpeedChanged();
	}
}

void WaterrowerModelV4::onAverageSpeedReceived(int averageSpeed)
{
	if (averageSpeed != m_private->AverageSpeed)
	{
		m_private->AverageSpeed = averageSpeed;
		Q_EMIT AbstractWaterrowerModel::averageSpeedChanged();
	}
}

void WaterrowerModelV4::onDistanceReceived(int distanceMeters)
{
	if (distanceMeters != m_private->Distance)
	{
		m_private->Distance = distanceMeters;
		Q_EMIT AbstractWaterrowerModel::distanceChanged();
	}
}

void WaterrowerModelV4::onStrokeStart()
{
	Q_EMIT AbstractWaterrowerModel::strokeStarted();
}

void WaterrowerModelV4::onStrokeEnd()
{
	Q_EMIT AbstractWaterrowerModel::strokeEnded();
}

void WaterrowerModelV4::setVersion(int model, int major, int minor)
{
	if (model != m_private->Version.model ||
		major != m_private->Version.major ||
		minor != m_private->Version.minor)
	{
		m_private->Version.model = model;
		m_private->Version.major = major;
		m_private->Version.minor = minor;

		Q_EMIT AbstractWaterrowerModel::versionChanged();
	}
}

void WaterrowerModelV4::onKeyPadInteraction(int button)
{
	Q_EMIT AbstractWaterrowerModel::keyPadInteraction(button);
}

void WaterrowerModelV4::onTankVolumeReceived(int volume)
{
	if (volume != m_private->TankVolume && volume > 0)
	{
		m_private->TankVolume = volume;

		Q_EMIT AbstractWaterrowerModel::tankVolumeChanged();
	}
}

void WaterrowerModelV4::onCalibrationPinsPerXXcm(int pinsPerXXcm)
{
	if (pinsPerXXcm != m_private->Calibration.pinsPerXXcm && pinsPerXXcm > 0)
	{
		m_private->Calibration.pinsPerXXcm = pinsPerXXcm;

		Q_EMIT AbstractWaterrowerModel::calibrationChanged( m_private->Calibration.pinsPerXXcm, m_private->Calibration.distanceXXcm );
	}
}


void WaterrowerModelV4::onCalibrationDistanceXXcm(int distanceXXcm)
{
	if (distanceXXcm != m_private->Calibration.distanceXXcm && distanceXXcm > 0)
	{
		m_private->Calibration.distanceXXcm = distanceXXcm;

		Q_EMIT AbstractWaterrowerModel::calibrationChanged( m_private->Calibration.pinsPerXXcm, m_private->Calibration.distanceXXcm );
	}
}

void WaterrowerModelV4::onTotalCaloriesReceived(int calories)
{
	if (calories != m_private->CaloriesTotal)
	{
		m_private->CaloriesTotal = calories;

		Q_EMIT AbstractWaterrowerModel::caloriesTotalChanged();
	}
}

void WaterrowerModelV4::onStrokesReceived(int strokes)
{
	if (strokes != m_private->Strokes)
	{
		m_private->Strokes = strokes;

		Q_EMIT AbstractWaterrowerModel::strokesChanged();
	}
}


void WaterrowerModelV4::setAge(int age)
{
	if (m_private->Age != age)
	{
		m_private->Age = age;
		Q_EMIT AbstractWaterrowerModel::ageChanged();
		Q_EMIT AbstractWaterrowerModel::maximumPulseChanged();
	}
}




///

int WaterrowerModelV4::getDistance() const
{
	return m_private->Distance;
}

int WaterrowerModelV4::getActualSpeed() const
{
	return m_private->ActualSpeed;
}

int WaterrowerModelV4::getAverageSpeed() const
{
	return m_private->AverageSpeed;
}

int WaterrowerModelV4::getModelVersion() const
{
	return m_private->Version.model;
}

int WaterrowerModelV4::getFirmwareVersionMajor() const
{
	return m_private->Version.major;
}

int WaterrowerModelV4::getFirmwareVersionMinor() const
{
	return m_private->Version.minor;
}

int WaterrowerModelV4::getTankVolume() const
{
	return m_private->TankVolume;
}

int WaterrowerModelV4::getHeartRate() const
{
	return m_private->HeartRate;
}

int WaterrowerModelV4::getCaloriesTotal() const
{
	return m_private->CaloriesTotal;
}

int WaterrowerModelV4::getStrokes() const
{
	return m_private->Strokes;
}

int WaterrowerModelV4::getMaximumPulse() const
{
	// Tanaka equation, based on linear regression for maximum pulse estimation
	const float theta_0(208.f);
	const float theta_1(-0.7f);

	return qRound(theta_0 + theta_1 * (float)m_private->Age);
}

int WaterrowerModelV4::getPaceZone() const
{
	float percent((100.f * m_private->HeartRate) / this->getMaximumPulse());

	Waterrower::HeartRateZone zone(Waterrower::NoTraining);

	if (percent >= 90.f)
	{
		zone = Waterrower::Anearobic_Sprint_90_100_Percent;
	}
	else if (percent >= 80.f)
	{
		zone = Waterrower::Anaerobic_Fast_80_90_Percent;
	}
	else if (percent >= 70.f)
	{
		zone = Waterrower::Aerobic_Moderate_70_80_Percent;
	}
	else if (percent >= 60.f)
	{
		zone = Waterrower::Aerobic_Comfortable_60_70_Percent;
	}
	else if (percent >= 50.f)
	{
		zone = Waterrower::Aerobic_Easy_50_60_Percent;
	}

	return (int) zone;
}

int WaterrowerModelV4::getAge() const
{
	return m_private->Age;
}


int WaterrowerModelV4::getDistancePerPin() const
{
	return m_private->Calibration.getDistancePerPin();
}

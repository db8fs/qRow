
#include "Waterrower.h"

#include "Model/WaterrowerModelV4.h"
#include "Transceiver/Transceiver.h"




/////

class ProtocolV4Adapter : public IProtocolHandler
{
	class WaterrowerModelV4 & m_model;

public:
	ProtocolV4Adapter( WaterrowerModelV4 & model )
		: m_model( model )
	{}

	~ProtocolV4Adapter()
	{}



private:

	/* -- IProtocolHandler -- */

	void onWaterrowerConnected()
	{
		m_model.onWaterrowerConnected();
	}

	void onPing()
	{
		m_model.setActualSpeedCmPerSec( 0 );
	}


	void onPulseCount(int pulsesPer25msec)
	{
		const int       SAMPLE_WINDOW_MSEC(25);
		const int       MILLISECONDS_PER_SECOND(1000);
		const int       SAMPLES_PER_SEC(MILLISECONDS_PER_SECOND / SAMPLE_WINDOW_MSEC);

		const double    CM_PER_PULSE( m_model.getDistancePerPin() );

		double          cmPerSample(pulsesPer25msec * CM_PER_PULSE);
		double          cmPerSecond(cmPerSample * SAMPLES_PER_SEC);

		int actualSpeed(qRound(cmPerSecond));

		m_model.setActualSpeedCmPerSec( actualSpeed );
	}




	void onHeartrateReceived(int heartRate)
	{
		m_model.onHeartrateReceived( heartRate );
	}

	void onAverageSpeedReceived(int averageSpeed)
	{
		m_model.onAverageSpeedReceived( averageSpeed );
	}

	void onDistanceReceived(int distanceMeters)
	{
		m_model.onDistanceReceived( distanceMeters );
	}

	void onStrokeStart()
	{
		m_model.onStrokeStart();
	}

	void onStrokeEnd()
	{
		m_model.onStrokeEnd();
	}

	void onVersionInfo(const WaterrowerVersion & version)
	{
		m_model.setVersion( version.model, version.major, version.minor );
	}

	void onKeyPadInteraction(KeyPadInteraction button)
	{
		m_model.onKeyPadInteraction( (int) button );
	}

	void onTankVolumeReceived(int volume)
	{
		m_model.onTankVolumeReceived( volume );
	}

	void onCalibrationPinsPerXXcm(int pinsPerXXcm)
	{
		m_model.onCalibrationPinsPerXXcm( pinsPerXXcm );
	}

	void onCalibrationDistanceXXcm(int distanceXXcm)
	{
		m_model.onCalibrationDistanceXXcm( distanceXXcm );
	}

	void onTotalCaloriesReceived(int calories)
	{
		m_model.onTotalCaloriesReceived( calories );
	}

	void onStrokesReceived(int strokes)
	{
		m_model.onStrokesReceived( strokes );
	}


};




/////

Waterrower::Waterrower(QObject *parent)
	: QObject(parent),
	m_model( new WaterrowerModelV4() ),
	m_protocolAdapter( new ProtocolV4Adapter( *m_model ) ),
	m_transceiver( new Transceiver( *m_protocolAdapter ) )
{
	Q_ASSERT( m_model != nullptr );
	Q_ASSERT( m_transceiver != nullptr );
	Q_ASSERT( m_protocolAdapter != nullptr );

	connect( m_model, &AbstractWaterrowerModel::waterrowerConnected, this, &Waterrower::waterrowerConnected);
	connect( m_model, &AbstractWaterrowerModel::waterrowerConnected, m_transceiver, &Transceiver::onWaterrowerConnected);
	connect( m_transceiver, &Transceiver::waterrowerDisconnected, this, &Waterrower::waterrowerDisconnected );

	connect( m_model, &AbstractWaterrowerModel::versionChanged, this, &Waterrower::versionChanged );
	connect( m_model, &AbstractWaterrowerModel::distanceChanged, this, &Waterrower::distanceChanged );
	connect( m_model, &AbstractWaterrowerModel::actualSpeedChanged, this, &Waterrower::actualSpeedChanged );
	connect( m_model, &AbstractWaterrowerModel::averageSpeedChanged, this, &Waterrower::averageSpeedChanged );
	connect( m_model, &AbstractWaterrowerModel::heartRateChanged, this, &Waterrower::heartRateChanged );
	connect( m_model, &AbstractWaterrowerModel::tankVolumeChanged, this, &Waterrower::tankVolumeChanged );
	connect( m_model, &AbstractWaterrowerModel::strokeStarted, this, &Waterrower::strokeStarted );
	connect( m_model, &AbstractWaterrowerModel::strokeEnded, this, &Waterrower::strokeEnded );
	connect( m_model, &AbstractWaterrowerModel::keyPadInteraction, this, &Waterrower::keyPadInteraction);
	connect( m_model, &AbstractWaterrowerModel::calibrationChanged, this, &Waterrower::calibrationChanged );
	connect( m_model, &AbstractWaterrowerModel::caloriesTotalChanged, this, &Waterrower::caloriesTotalChanged );
	connect( m_model, &AbstractWaterrowerModel::strokesChanged, this, &Waterrower::strokesChanged );
	connect( m_model, &AbstractWaterrowerModel::paceZoneChanged, this, &Waterrower::paceZoneChanged );
	connect( m_model, &AbstractWaterrowerModel::ageChanged, this, &Waterrower::ageChanged );
	connect( m_model, &AbstractWaterrowerModel::maximumPulseChanged, this, &Waterrower::maximumPulseChanged );
}

Waterrower::~Waterrower()
{
	delete m_transceiver;
	delete m_model;
}

int Waterrower::distance() const
{
	return m_model->getDistance();
}

int Waterrower::actualSpeed() const
{
	return m_model->getActualSpeed();
}

int Waterrower::averageSpeed() const
{
	return m_model->getAverageSpeed();
}


int Waterrower::modelVersion() const
{
	return m_model->getModelVersion();
}

int Waterrower::majorVersion() const
{
	return m_model->getFirmwareVersionMajor();
}

int Waterrower::minorVersion() const
{
	return m_model->getFirmwareVersionMinor();
}

int Waterrower::tankVolume() const
{
	return m_model->getTankVolume();
}

int Waterrower::heartRate() const
{
	return m_model->getHeartRate();
}

int Waterrower::caloriesTotal() const
{
	return m_model->getCaloriesTotal();
}

int Waterrower::strokes() const
{
	return m_model->getStrokes();
}

int Waterrower::maximumPulse() const
{
	return m_model->getMaximumPulse();
}

Waterrower::HeartRateZone Waterrower::paceZone() const
{
	return (Waterrower::HeartRateZone) m_model->getPaceZone();
}

int Waterrower::age() const
{
	return m_model->getAge();
}

void Waterrower::setAge(int age)
{
	m_model->setAge(age);
}


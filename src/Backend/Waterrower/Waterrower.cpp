
#include "Waterrower.h"

#include "Model/WaterrowerModelV4.h"
#include "Transceiver/Transceiver.h"


#include "Protocol/ProtocolV4Adapter.h"


struct Waterrower_Private
{
	Waterrower_Private()
		:	m_model(),
			m_protocolAdapter( m_model ),
			m_transceiver( m_protocolAdapter )
	{
	}

	WaterrowerModelV4	m_model;
	ProtocolV4Adapter	m_protocolAdapter;
	Transceiver			m_transceiver;
};




/////

Waterrower::Waterrower(QObject *parent)
	: QObject(parent),
	m_private( new Waterrower_Private() )
{
	connect( &m_private->m_model, &AbstractWaterrowerModel::waterrowerConnected, this, &Waterrower::waterrowerConnected);
	connect( &m_private->m_model, &AbstractWaterrowerModel::waterrowerConnected, &m_private->m_transceiver, &Transceiver::onWaterrowerConnected);
	connect( &m_private->m_transceiver, &Transceiver::waterrowerDisconnected, this, &Waterrower::waterrowerDisconnected );
			 
	connect( &m_private->m_model, &AbstractWaterrowerModel::versionChanged, this, &Waterrower::versionChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::distanceChanged, this, &Waterrower::distanceChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::actualSpeedChanged, this, &Waterrower::actualSpeedChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::averageSpeedChanged, this, &Waterrower::averageSpeedChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::heartRateChanged, this, &Waterrower::heartRateChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::tankVolumeChanged, this, &Waterrower::tankVolumeChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::strokeStarted, this, &Waterrower::strokeStarted );
	connect( &m_private->m_model, &AbstractWaterrowerModel::strokeEnded, this, &Waterrower::strokeEnded );
	connect( &m_private->m_model, &AbstractWaterrowerModel::keyPadInteraction, this, &Waterrower::keyPadInteraction);
	connect( &m_private->m_model, &AbstractWaterrowerModel::calibrationChanged, this, &Waterrower::calibrationChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::caloriesTotalChanged, this, &Waterrower::caloriesTotalChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::strokesChanged, this, &Waterrower::strokesChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::paceZoneChanged, this, &Waterrower::paceZoneChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::ageChanged, this, &Waterrower::ageChanged );
	connect( &m_private->m_model, &AbstractWaterrowerModel::maximumPulseChanged, this, &Waterrower::maximumPulseChanged );
}

Waterrower::~Waterrower()
{
	delete m_private;
}

int Waterrower::distance() const
{
	return m_private->m_model.getDistance();
}

int Waterrower::actualSpeed() const
{
	return m_private->m_model.getActualSpeed();
}

int Waterrower::averageSpeed() const
{
	return m_private->m_model.getAverageSpeed();
}


int Waterrower::modelVersion() const
{
	return m_private->m_model.getModelVersion();
}

int Waterrower::majorVersion() const
{
	return m_private->m_model.getFirmwareVersionMajor();
}

int Waterrower::minorVersion() const
{
	return m_private->m_model.getFirmwareVersionMinor();
}

int Waterrower::tankVolume() const
{
	return m_private->m_model.getTankVolume();
}

int Waterrower::heartRate() const
{
	return m_private->m_model.getHeartRate();
}

int Waterrower::caloriesTotal() const
{
	return m_private->m_model.getCaloriesTotal();
}

int Waterrower::strokes() const
{
	return m_private->m_model.getStrokes();
}

int Waterrower::maximumPulse() const
{
	return m_private->m_model.getMaximumPulse();
}

Waterrower::HeartRateZone Waterrower::paceZone() const
{
	return (Waterrower::HeartRateZone) m_private->m_model.getPaceZone();
}

int Waterrower::age() const
{
	return m_private->m_model.getAge();
}

void Waterrower::setAge(int age)
{
	m_private->m_model.setAge(age);
}


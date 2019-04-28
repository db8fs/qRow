#include "ProtocolV4Adapter.h"

#include "Model/WaterrowerModelV4.h"

#include "Transceiver/Serial/SerialAdapter.h"

#include "ProtocolV4/Parser/Parser.h"

#include "ProtocolV4/Commands/CommandSetV4.h"
#include "ProtocolV4/Logging/Logging.h"


struct ProtocolV4Adapter_Private : private IProtocolHandler
{
	ProtocolV4Adapter_Private( class WaterrowerModelV4 & model )
		:	m_model( model ),
			m_parser( *this )
	{
	}



	// ----- IProtocol ----- //

	void onPulseCount(int pulsesPer25msec)
	{
		const int       SAMPLE_WINDOW_MSEC(25);
		const int       MILLISECONDS_PER_SECOND(1000);
		const int       SAMPLES_PER_SEC(MILLISECONDS_PER_SECOND / SAMPLE_WINDOW_MSEC);

		const double    CM_PER_PULSE(m_model.getDistancePerPin());

		double          cmPerSample(pulsesPer25msec * CM_PER_PULSE);
		double          cmPerSecond(cmPerSample * SAMPLES_PER_SEC);

		int actualSpeed(qRound(cmPerSecond));

		m_model.setActualSpeedCmPerSec(actualSpeed);
	}


	void onWaterrowerConnected()							{	m_model.onWaterrowerConnected();									}

	void onPing()											{	m_model.setActualSpeedCmPerSec(0);									}

	void onHeartrateReceived(int heartRate)					{	m_model.onHeartrateReceived(heartRate);								}

	void onAverageSpeedReceived(int averageSpeed)			{	m_model.onAverageSpeedReceived(averageSpeed);						}

	void onDistanceReceived(int distanceMeters)				{	m_model.onDistanceReceived(distanceMeters);							}

	void onStrokeStart()									{	m_model.onStrokeStart();											}

	void onStrokeEnd()										{	m_model.onStrokeEnd();												}

	void onVersionInfo(const WaterrowerVersion & version)	{	m_model.setVersion(version.model, version.major, version.minor);	}

	void onKeyPadInteraction(KeyPadInteraction button)		{	m_model.onKeyPadInteraction((int)button);							}

	void onTankVolumeReceived(int volume)					{	m_model.onTankVolumeReceived(volume);								}

	void onCalibrationPinsPerXXcm(int pinsPerXXcm)			{	m_model.onCalibrationPinsPerXXcm(pinsPerXXcm);						}

	void onCalibrationDistanceXXcm(int distanceXXcm)		{	m_model.onCalibrationDistanceXXcm(distanceXXcm);					}

	void onTotalCaloriesReceived(int calories)				{	m_model.onTotalCaloriesReceived(calories);							}

	void onStrokesReceived(int strokes)						{	m_model.onStrokesReceived(strokes);									}




	WaterrowerModelV4 & m_model;
	
	Parser m_parser;
};





// ---- ProtocolV4 control ----- //

ProtocolV4Adapter::ProtocolV4Adapter(class WaterrowerModelV4 & model)
	:	m_private( new ProtocolV4Adapter_Private( model) )
{}

ProtocolV4Adapter::~ProtocolV4Adapter()
{
	delete m_private;
}


// request firmware version and rowing computer model
void ProtocolV4Adapter::txVersionRequest(SerialAdapter & serialAdapter)
{
	serialAdapter.write("IV?\r\n");
}

// sends the data requests with high update rate	
void ProtocolV4Adapter::txHighPrioRequests(SerialAdapter & serialAdapter)
{
	serialAdapter.write( buildDataCommand(Register_HeartRate, DLC_1_Byte, WR_Command_RequestValue) );
	serialAdapter.write( buildDataCommand(Register_Distance, DLC_2_Byte, WR_Command_RequestValue) );
	serialAdapter.write( buildDataCommand(Register_AverageSpeed, DLC_2_Byte, WR_Command_RequestValue) );
	serialAdapter.write( buildDataCommand(Register_Calories, DLC_3_Byte, WR_Command_RequestValue) );
	serialAdapter.write( buildDataCommand(Register_Strokes, DLC_2_Byte, WR_Command_RequestValue) );
}

// sends the data requests with low update rate
void ProtocolV4Adapter::txLowPrioRequests(SerialAdapter & serialAdapter)
{
	serialAdapter.write( buildDataCommand(Register_TankVolume, DLC_1_Byte, WR_Command_RequestValue) );
	serialAdapter.write( buildDataCommand(Register_CalibrationPinsPerXXcm, DLC_1_Byte, WR_Command_RequestValue) );
	serialAdapter.write( buildDataCommand(Register_CalibrationDistanceXXcm, DLC_1_Byte, WR_Command_RequestValue) );
}

bool ProtocolV4Adapter::rxReceive(const QString & line)
{
	return m_private->m_parser.parse(line);
}




#include "ProtocolV4.h"

#include "Logging/Logging.h"
#include "Parser/Parser.h"


struct Protocol_Private
{
	Protocol_Private( IProtocolHandler & protocolHandler )
		: m_parser( protocolHandler )
	{

	}

	Parser m_parser;
};



ProtocolV4::ProtocolV4(class IProtocolHandler & protocolHandler)
	: m_private( new Protocol_Private(protocolHandler) )
{
}

ProtocolV4::~ProtocolV4()
{
	delete m_private;
}


bool ProtocolV4::receive(const QString & line)
{
	return m_private->m_parser.parse( line );
}


QByteArray  ProtocolV4::getCmdHeartRate() const
{
	return buildDataCommand( Register_HeartRate, DLC_1_Byte );
}

QByteArray  ProtocolV4::getCmdDistance() const
{
	return buildDataCommand( Register_Distance, DLC_2_Byte);
}

QByteArray  ProtocolV4::getCmdAverageSpeed() const
{
	return buildDataCommand(Register_AverageSpeed, DLC_2_Byte);
}

QByteArray  ProtocolV4::getCmdCalories() const
{
	return buildDataCommand(Register_Calories, DLC_3_Byte);
}

QByteArray  ProtocolV4::getCmdStrokes() const
{
	return buildDataCommand(Register_Strokes, DLC_2_Byte);
}

QByteArray  ProtocolV4::getCmdTankVolume() const
{
	return buildDataCommand(Register_TankVolume, DLC_1_Byte);
}

QByteArray  ProtocolV4::getCmdCalibrationPinsPerXXcm() const
{
	return buildDataCommand(Register_CalibrationPinsPerXXcm, DLC_1_Byte);
}

QByteArray  ProtocolV4::getCmdCalibrationDistanceXXcm() const
{
	return buildDataCommand(Register_CalibrationDistanceXXcm, DLC_1_Byte);
}


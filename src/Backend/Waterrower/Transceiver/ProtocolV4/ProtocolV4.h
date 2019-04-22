#ifndef COMMANDS_H_
#define COMMANDS_H_

struct WaterrowerVersion { int model = { 0 }; int major = { 0 }; int minor = { 0 }; };

struct WaterrowerCalibration
{
	double getDistancePerPin() const { return (double)distanceXXcm / (double)pinsPerXXcm; }		/**< returns the distance represented by one pin count of the rotary encoder [cm] */

	int pinsPerXXcm = { 32 }; int distanceXXcm = { 35 };
};

/** enumerates the possible operations possible on waterrower data registers */
enum WR_Command
{
	WR_Command_RequestValue = 0,    /**< requests the value stored in a register address */
	WR_Command_Value = 1            /**< measurement value stored at an register address */
};


/** enumerates the possible data lengths that may be read from the waterrower ergometer */
enum DLC
{
	DLC_1_Byte = 0,     /**< data length of 8-Bits  */
	DLC_2_Byte = 1,     /**< data length of 16-bits */
	DLC_3_Byte = 2      /**< data length of 24-bits */
};


/** enumerates the possible registers that may be read from the waterrower ergometer */
enum Register
{
	Register_HeartRate = 0,             /**< Waterrower ergometer register storing the heartrate in beats per minute [bpm] */
	Register_Distance,                  /**< Waterrower ergometer register storing the rowed distance in meters [m] */
	Register_AverageSpeed,              /**< Waterrower ergometer register storing the average speed in meters per second [m/s] */
	Register_TankVolume,                /**< Waterrower ergometer register storing the tank volume used for computations [l] */
	Register_CalibrationPinsPerXXcm,    /**< Waterrower ergometer register storing the nr of pin edges allowed to equal xx cm */
	Register_CalibrationDistanceXXcm,   /**< Waterrower ergometer register storing the nr of cm per flagged xxcm no. of pins */
	Register_Calories,                  /**< Waterrower ergometer register storing the nr of total calories */
	Register_Strokes
};


/** enumerates the basic commands, the waterrower ergometer is sending without being requested. */
enum BasicCommand
{
	BasicCommand_PING = 0,          /**< Waterrower ergometer sends PING as keep-alive packet, when no rowing operation is done */
	BasicCommand_StrokeStart,       /**< Waterrower ergometer sends StrokeStart, when force gets applied on the paddle */
	BasicCommand_StrokeEnd,         /**< Waterrower ergometer sends StrokeEnd, when force is no longer being applied on the paddle */
	BasicCommand_USB_ACK,           /**< Waterrower ergometer sends USB Acknowledge to indicate the remote control by the PC. */
	BasicCommand_PulsesPer25msec    /**< Waterrower ergometer sends the amount of pulses per 25msec for being applied in sophisticated rowing models */
};

/** enumerates keypad interactions that may get sent by the waterrower ergometer */
enum KeyPadInteraction
{
	KeyPad_RESET = 0,               /**< Reset button has been pressed */
	KeyPad_Units,                   /**< Units selection has been pressed  (toggles units for intensity/average intensity/distance displaying ) */
	KeyPad_Zones,                   /**< Zones selection has been pressed  (toggles training zones for heartrate/intensity/strokecount) */
	KeyPad_WorkoutPrograms,         /**< Workout program has been selected (toggles [interval](distance|duration) workout programs) */
	KeyPad_UpArrow,                 /**< Moves one selection upwards*/
	KeyPad_Ok,                      /**< Acknowledges current selection */
	KeyPad_DownArrow,               /**< Moves one selection downwards*/
	KeyPad_Advanced,                /**< Advanced program selection */
	KeyPad_StoredPrograms,          /**< Accesses last 9 historic workout programs, as stored by the monitor's AutoStore function */
	KeyPad_HoldCancel               /**< Cancels current selection */
};

#include <QByteArray>

class ProtocolV4
{
public:
	ProtocolV4(class IProtocolHandler & protocolHandler);
	~ProtocolV4();


	QByteArray  getCmdHeartRate() const;
	QByteArray  getCmdDistance() const;
	QByteArray  getCmdAverageSpeed() const;
	QByteArray  getCmdCalories() const;
	QByteArray  getCmdStrokes() const;
	QByteArray  getCmdTankVolume() const;
	QByteArray  getCmdCalibrationPinsPerXXcm() const;
	QByteArray  getCmdCalibrationDistanceXXcm() const;


	bool receive( const QString & line );

private:
	struct Protocol_Private* const	m_private;

};



#endif
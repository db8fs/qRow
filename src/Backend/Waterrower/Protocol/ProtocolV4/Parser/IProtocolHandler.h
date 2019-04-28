#ifndef IPROTOCOLHANDLER_H
#define IPROTOCOLHANDLER_H

/** implementers of this interface may process serial protocol data */
class IProtocolHandler
{
public:
    virtual ~IProtocolHandler() {}

    virtual void onWaterrowerConnected() = 0;                           /**< called when the serial connection has been established successfully */
    virtual void onPing() = 0;                                          /**< keep-alive message has been received (connection is still there to the waterrower) */
    virtual void onPulseCount( int pulsesPer25msec ) = 0;               /**< called when a pulsecount message has been received */
    virtual void onHeartrateReceived( int heartRate ) = 0;              /**< called when a heartrate message has been received */
    virtual void onAverageSpeedReceived( int averageSpeed ) = 0;        /**< called when the average speed message has been received */
    virtual void onDistanceReceived( int distanceMeters ) =  0;         /**< called when the distance message has been received */
    virtual void onStrokeStart() = 0;                                   /**< called when a stroke is being started */
    virtual void onStrokeEnd()  = 0;                                    /**< called when a stroke is being completed */
    virtual void onKeyPadInteraction( enum KeyPadInteraction ) = 0;     /**< called when the user interacted with the keypad buttons, passing @ref{KeyPadInteraction} argument */
    virtual void onVersionInfo( const struct WaterrowerVersion & ) = 0; /**< called when the model/software version has been queried */
    virtual void onTankVolumeReceived( int volume ) = 0;                /**< called when the tank volume (as stored in the rowing computer) has been received */
    virtual void onCalibrationPinsPerXXcm( int pins ) = 0;              /**< called when the number of pin edges allowed to equal xx cm has been received (pins_per_xxcm) */
    virtual void onCalibrationDistanceXXcm( int distance ) = 0;         /**< called when the number of cm per flagged xxcm no. of pins has been received (distance_xxcm) */
	virtual void onTotalCaloriesReceived(int calories) = 0;				/**< called when the total calories message has been received */
	virtual void onStrokesReceived(int strokes) = 0;
};

class IProtocolHandler;
typedef void (IProtocolHandler::*pFnArgumentVoid)       ();                 /**< function pointer type with ZERO argument functor signature */
typedef void (IProtocolHandler::*pFnArgumentInt)        ( int );            /**< function pointer type with ONE INTEGER argument in functor signature */
typedef void (IProtocolHandler::*pFnArgumentIntInt)     ( int, int );       /**< function pointer type with TWO INTEGER arguments in functor signature */
typedef void (IProtocolHandler::*pFnArgumentIntIntInt)  ( int, int, int);   /**< function pointer type with THREE INTEGER arguments in functor signature */
typedef void (IProtocolHandler::*pFnArgumentVersion)    ( const WaterrowerVersion & );
typedef void (IProtocolHandler::*pFnArgumentKeyPad)     ( enum KeyPadInteraction );



#endif // IPROTOCOLHANDLER_H

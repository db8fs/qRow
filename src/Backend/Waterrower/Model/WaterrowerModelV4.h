#ifndef WATERROWERMODELV4_H_
# define WATERROWERMODELV4_H_

#include "AbstractWaterrowerModel.h"

class WaterrowerModelV4 : public AbstractWaterrowerModel
{
public:

	WaterrowerModelV4();
	~WaterrowerModelV4();


	int getDistance() const;

	int getActualSpeed() const;

	int getAverageSpeed() const;

	int getModelVersion() const;

	int getFirmwareVersionMajor() const;

	int getFirmwareVersionMinor() const;

	int getTankVolume() const;

	int getHeartRate() const;

	int getCaloriesTotal() const;

	int getStrokes() const;

	int getMaximumPulse() const;

	int getPaceZone() const;

	int getAge() const;

	int getDistancePerPin() const;

	// data model changes

	void onWaterrowerConnected();

	void onHeartrateReceived(int heartRate);

	void setActualSpeedCmPerSec(int actualSpeed);

	void onAverageSpeedReceived(int averageSpeed);

	void onDistanceReceived(int distanceMeters);

	void onStrokeStart();

	void onStrokeEnd();

	void setVersion(int model, int major, int minor);

	void onKeyPadInteraction(int button);

	void onTankVolumeReceived(int volume);

	void onCalibrationPinsPerXXcm(int pinsPerXXcm);

	void onCalibrationDistanceXXcm(int distanceXXcm);

	void onTotalCaloriesReceived(int calories);

	void onStrokesReceived(int strokes);

	void setAge(int age);

private:

	struct DataModelV4_Private* const m_private;

};

#endif // WATERROWERMODELV4_H_

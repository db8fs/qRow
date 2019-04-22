#ifndef ABSTRACTWATERROWERMODEL_H_
# define ABSTRACTWATERROWERMODEL_H_

#include <QObject>

class AbstractWaterrowerModel : public QObject
{
	Q_OBJECT

public:
	AbstractWaterrowerModel();
	virtual ~AbstractWaterrowerModel() = 0;

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



};

#endif // ABSTRACTWATERROWERMODEL_H_
#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <QObject>
#include <QRect>

#include <opencv2/objdetect.hpp>

/** estimates the user's face in given video images */
class FaceDetector : public QObject
{
    Q_OBJECT

public:
    FaceDetector();

Q_SIGNALS:

    /** takes an image frame as input for face detection */
    void receiveImage( const cv::Mat & image );

    /** outputs the successful detection of a face, described by the rectangle */
    void faceDetected( const QRect & faceRegion );

private Q_SLOTS:
    void onProcessImage( const cv::Mat & image );

private:
    cv::CascadeClassifier m_faceDetector;
};

#endif // FACEDETECTOR_H

#include "FaceDetector.h"

#include "VideoTools.h"

#include "../System/Logging.h"

#include <QDebug>

#include <QTime>

FaceDetector::FaceDetector()
    : QObject()
{
    if( !m_faceDetector.load( "C:/Projekte/opencv/data/haarcascades/haarcascade_frontalface_default.xml" ) )
    {
        qVideo.warning( "OpenCVFilterRunnable: could not load XML file.", Waterrower::Logger::LogLevel_Basic );
    }

    connect( this, &FaceDetector::receiveImage, this, &FaceDetector::onProcessImage );
}

void FaceDetector::onProcessImage(const cv::Mat &image)
{
    QTime timer;
    timer.start();

    std::vector<cv::Rect> faces( OpenCV::detectFaces( image, m_faceDetector ) );

    qVideo.info( QString("Num Faces: ") + faces.size() );

    for( auto face : faces )
    {
        Q_EMIT faceDetected( QRect( face.x, face.y, face.width, face.height ));

        //qDebug() << "Face: (" << face.tl().x << "," << face.tl().y << ") - (" << face.br().x << "," << face.br().y << ") - Time: " << timer.elapsed();
    }
}

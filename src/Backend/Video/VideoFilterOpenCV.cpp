#include "VideoFilterOpenCV.h"

#include <QVideoFilterRunnable>
#include <QDebug>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "VideoTools.h"
#include "FaceDetector.h"

//TODO: adapter schreiben, der Qt-Formate in OpenCV-Formate transformiert (und zurueck)

class OpenCVFilterRunnable : public QVideoFilterRunnable
{
    FaceDetector & m_faceDetector;


public:

    OpenCVFilterRunnable( FaceDetector & faceDetector )
        : QVideoFilterRunnable(),
          m_faceDetector( faceDetector )
    {
    }


    /** overrides the QVideoFilterRunnable pure-virtual interface method for implementing the filter */
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
    {
        Q_UNUSED(flags)

        using namespace cv;

        int     gaussianBlurSize( 7 );
        double  gaussianBlurCoef( 5.6);
        int     cannyKernelSize( 7 );
        double  cannyThreshold( 50. );


        if( nullptr != input )
        {
            if( input->isValid() )
            {
                QVideoFrame::PixelFormat pixelFormat( input->pixelFormat() );

                input->map(QAbstractVideoBuffer::ReadWrite);

                Mat mat( OpenCV::fromVideoFrame( *input ) );
                Mat edges( OpenCV::toGreyscale( mat, pixelFormat ) );

                cv::equalizeHist( edges, edges );
                Q_EMIT m_faceDetector.receiveImage( edges );


#if 0

                cv::GaussianBlur(edges, edges, Size(gaussianBlurSize,gaussianBlurSize), gaussianBlurCoef, gaussianBlurCoef);
                cv::Canny(edges, edges, cannyThreshold, 3 * cannyThreshold, cannyKernelSize);
#endif

                OpenCV::toVideoFrame( *input, OpenCV::fromGreyscale( edges, pixelFormat ) );

                input->unmap();
                return *input;
            }

            return *input;
        }

        return QVideoFrame();
    }

};


VideoFilterOpenCV::VideoFilterOpenCV(FaceDetector &faceDetector)
    : m_faceDetector( faceDetector )
{
}

QVideoFilterRunnable *VideoFilterOpenCV::createFilterRunnable()
{
    return new OpenCVFilterRunnable( m_faceDetector );
}

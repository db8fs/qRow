#include "VideoTools.h"

#include "../System/Logging.h"

#include <QtGlobal>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>


namespace OpenCV
{
    using namespace cv;

    /** converts a QVideoFrame into an OpenCV matrix */
    cv::Mat fromVideoFrame( QVideoFrame & input )
    {
        return cv::Mat( input.height(), input.width(), OpenCV::getArrayType( input.pixelFormat() ), input.bits() );
    }

    /** converts a given OpenCV matrix into a QVideoFrame for being rendered with QML */
    void toVideoFrame( QVideoFrame & output, const cv::Mat & input )
    {
        // todo... check bounds of input/output
        memcpy( output.bits(), input.data, output.height() * output.bytesPerLine() );
    }

    cv::Mat toGreyscale( const cv::Mat &input, QVideoFrame::PixelFormat pixelFormat )
    {
        int channels( input.channels() );

        if( channels == 1 )
            return Mat(input);

        cv::Mat mat;

        switch(pixelFormat)
        {
        case QVideoFrame::Format_ARGB32:
            qCritical( "VideoTools::toGreyscale(): ARGB32 not supported!" );
            break;
        case QVideoFrame::Format_ARGB32_Premultiplied:
            qCritical( "VideoTools::toGreyscale(): ARGB32_Premultiplied not supported!" );
            break;
        case QVideoFrame::Format_RGB32:
            cvtColor( input, mat, COLOR_RGBA2GRAY );
            break;
        case QVideoFrame::Format_RGB24:
            cvtColor( input, mat, COLOR_RGB2GRAY );
            break;
        case QVideoFrame::Format_RGB565:
            qCritical( "VideoTools::toGreyscale(): RGB565 not supported!" );
            break;
        case QVideoFrame::Format_RGB555:
            qCritical( "VideoTools::toGreyscale(): RGB555 not supported!" );
            break;
        case QVideoFrame::Format_ARGB8565_Premultiplied:
            qCritical( "VideoTools::toGreyscale(): ARGB8565_Premultiplied not supported!" );
            break;
        case QVideoFrame::Format_BGRA32:
            cvtColor( input, mat, COLOR_BGRA2GRAY );
            break;
        case QVideoFrame::Format_BGRA32_Premultiplied:
            qCritical( "VideoTools::toGreyscale(): BGRA32_Premultiplied not supported!" );
            break;
        case QVideoFrame::Format_BGR32:
            cvtColor( input, mat, COLOR_BGRA2GRAY );
            break;
        case QVideoFrame::Format_BGR24:
            cvtColor( input, mat, COLOR_BGR2GRAY );
            break;
        case QVideoFrame::Format_BGR565:
            cvtColor( input, mat, COLOR_BGR5652GRAY );
            break;
        case QVideoFrame::Format_BGR555:
            cvtColor( input, mat, COLOR_BGR5552GRAY );
            break;
        case QVideoFrame::Format_BGRA5658_Premultiplied:
            cvtColor( input, mat, COLOR_BGRA2GRAY );
            qVideo.info( "VideoTools::toGreyscale(): BGRA5658_Premultiplied not supported!" );
            break;
        case QVideoFrame::Format_YUV420P:
            cvtColor( input, mat, COLOR_YUV2GRAY_I420 );
            break;
        case QVideoFrame::Format_NV12:
            cvtColor( input, mat, COLOR_YUV2GRAY_NV12 );
            break;
        default:
            break;
        }

        return mat;
    }



    Mat fromGreyscale(const Mat &input, QVideoFrame::PixelFormat pixelFormat)
    {
        int channels( input.channels() );

        if( channels > 1 )
            return Mat(input);

        cv::Mat mat;

        switch(pixelFormat)
        {
        case QVideoFrame::Format_ARGB32:
            qCritical( "VideoTools::fromGreyscale(): ARGB32 not supported!" );
            break;
        case QVideoFrame::Format_ARGB32_Premultiplied:
            qCritical( "VideoTools::fromGreyscale(): ARGB32_Premultiplied not supported!" );
            break;
        case QVideoFrame::Format_RGB32:
            cvtColor( input, mat, COLOR_GRAY2RGBA);
            break;
        case QVideoFrame::Format_RGB24:
            cvtColor( input, mat, COLOR_GRAY2RGB );
            break;
        case QVideoFrame::Format_RGB565:
            qCritical( "VideoTools::fromGreyscale(): RGB565 not supported!" );
            break;
        case QVideoFrame::Format_RGB555:
            qCritical( "VideoTools::fromGreyscale(): RGB555 not supported!" );
            break;
        case QVideoFrame::Format_ARGB8565_Premultiplied:
            qCritical( "VideoTools::fromGreyscale(): ARGB8565_Premultiplied not supported!" );
            break;
        case QVideoFrame::Format_BGRA32:
            cvtColor( input, mat, COLOR_GRAY2BGRA );
            break;
        case QVideoFrame::Format_BGRA32_Premultiplied:
            qCritical( "VideoTools::fromGreyscale(): BGRA32_Premultiplied not supported!" );
            break;
        case QVideoFrame::Format_BGR32:
            cvtColor( input, mat, COLOR_GRAY2BGRA );
            break;
        case QVideoFrame::Format_BGR24:
            cvtColor( input, mat, COLOR_GRAY2BGR );
            break;
        case QVideoFrame::Format_BGR565:
            cvtColor( input, mat, COLOR_GRAY2BGR565 );
            break;
        case QVideoFrame::Format_BGR555:
            cvtColor( input, mat, COLOR_GRAY2BGR555 );
            break;
        case QVideoFrame::Format_BGRA5658_Premultiplied:
            qCritical( "VideoTools::fromGreyscale(): BGRA5658_Premultiplied not supported!" );
            break;
        case QVideoFrame::Format_YUV420P:
            qCritical( "VideoTools::fromGreyscale(): YUV420P not supported!" );
            break;
        case QVideoFrame::Format_NV12:
            {
                cv::Mat tmp;
                cvtColor( input, tmp, COLOR_GRAY2BGRA );
                cvtColor( tmp, mat, COLOR_BGRA2YUV_I420 );
            }
            break;
        default:
            break;
        }

        return mat;
    }



    cv::Mat toHSI()
    {
        return cv::Mat();
    }



    int getArrayType(QVideoFrame::PixelFormat pixelFormat)
    {
        switch(pixelFormat)
        {
        case QVideoFrame::Format_ARGB32:
            return CV_8UC4;
        case QVideoFrame::Format_ARGB32_Premultiplied:
            return CV_8UC4;
        case QVideoFrame::Format_RGB32:
            return CV_8UC4;
        case QVideoFrame::Format_RGB24:
            return CV_8UC3;
        case QVideoFrame::Format_RGB565:
            return CV_8UC2;
        case QVideoFrame::Format_RGB555:
            return CV_8UC2;
        case QVideoFrame::Format_ARGB8565_Premultiplied:
            return CV_8UC3;
        case QVideoFrame::Format_BGRA32:
            return CV_8UC4;
        case QVideoFrame::Format_BGRA32_Premultiplied:
            return CV_8UC4;
        case QVideoFrame::Format_BGR32:
            return CV_8UC4;
        case QVideoFrame::Format_BGR24:
            return CV_8UC3;
        case QVideoFrame::Format_BGR565:
            return CV_8UC2;
        case QVideoFrame::Format_BGR555:
            return CV_8UC2;
        case QVideoFrame::Format_BGRA5658_Premultiplied:
            return CV_8UC3;
        case QVideoFrame::Format_YUV420P:
            return CV_8UC1;
        case QVideoFrame::Format_NV12:
            return CV_8UC1;
        default:
            break;
        }

        qFatal( "OpenCV::getArrayType: Unknown pixel format!" );
        return CV_8UC1;
    }



    std::vector<cv::Rect> detectFaces( const cv::Mat & image, cv::CascadeClassifier & faceDetector )
    {
        std::vector<cv::Rect> faces;
        faceDetector.detectMultiScale( image, faces, 1.1, 2, CASCADE_SCALE_IMAGE | CASCADE_DO_CANNY_PRUNING, Size(30, 30) );
        return faces;
    }

}

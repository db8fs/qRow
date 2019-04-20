#ifndef VIDEOTOOLS_H
#define VIDEOTOOLS_H

#include <QVideoFrame>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>

namespace OpenCV
{
    /** converts a QVideoFrame into an OpenCV matrix */
    cv::Mat fromVideoFrame( QVideoFrame & input );

    /** converts a given OpenCV matrix into a QVideoFrame for being rendered with QML */
    void toVideoFrame( QVideoFrame & output, const cv::Mat & input );

    /** converts the given input matrix into 8bit greyscale format */
    cv::Mat toGreyscale(const cv::Mat & input, QVideoFrame::PixelFormat pixelFormat );

    /** converts the given 8bit greyscale format into VideoOutput compatible format */
    cv::Mat fromGreyscale(const cv::Mat & input, QVideoFrame::PixelFormat pixelFormat );

    /** converts the given input image into HSI format */
    cv::Mat toHSI( const cv::Mat & input );

    /** returns the OpenCV matrix type from the given pixelformat */
    int getArrayType(QVideoFrame::PixelFormat pixelFormat);

    /** returns a vector of rectangles, guessing faces within an image. */
    std::vector<cv::Rect> detectFaces( const cv::Mat & image, cv::CascadeClassifier & faceDetector );
}

#endif // VIDEOTOOLS_H

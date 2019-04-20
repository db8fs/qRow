#ifndef VIDEOFILTEROPENCV_H
#define VIDEOFILTEROPENCV_H

#include <QAbstractVideoFilter>


class VideoFilterOpenCV : public QAbstractVideoFilter
{
    class FaceDetector & m_faceDetector;

public:

    VideoFilterOpenCV( class FaceDetector & faceDetector );

    QVideoFilterRunnable* createFilterRunnable();

Q_SIGNALS:
    void finished( QObject* result );
};

#endif // VIDEOFILTEROPENCV_H

#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlEngine>
#include <QQuickView>
#include <QQmlContext>

#include "Backend/Analysis/Model/WorkoutModel.h"
#include "Backend/Analysis/Model/SessionModel.h"
#include "Backend/Analysis/WorkoutController.h"
#include "Backend/Video/VideoFilterOpenCV.h"

#include "Backend/Video/FaceDetector.h"
#include "SampleData.h"

#include "Backend/Waterrower/Waterrower.h"

#include "Frontend/CLI/ArgumentParser.h"


int main( int argc, char** argv)
{
    QVector< QString > foo( getSampleLogfiles() );

    WorkoutModel        workoutModel;
    SessionModel        sessionsModel;
    WorkoutController   workoutController( sessionsModel, workoutModel );

    int i(0);
    for( QString item : foo )
    {
        ++i;

        if( i > 10 )
        {
            workoutController.importLogfile( item );
          break;
        }
    }


    QGuiApplication app( argc, argv );
    QQmlApplicationEngine engine;

	app.setApplicationName("qRow");
	app.setApplicationVersion("0.1.1");

	ArgumentParser parser;
	parser.process(app);


    Waterrower waterrower;

#if HAVE_OPENCV
	qRegisterMetaType<cv::Mat>("cv::Mat");

    FaceDetector faceDetector;
    VideoFilterOpenCV openCVFilter( faceDetector );

    engine.rootContext()->setContextProperty( "faceDetector", &faceDetector);
    engine.rootContext()->setContextProperty( "openCVFilter", &openCVFilter );
#endif
	
	engine.rootContext()->setContextProperty( "waterrower", &waterrower );
	engine.rootContext()->setContextProperty( "sessionsModel", &sessionsModel );
    engine.rootContext()->setContextProperty( "workoutModel", &workoutModel);
    engine.rootContext()->setContextProperty( "workoutController", &workoutController );

    engine.load( QUrl(QStringLiteral("qrc:/src/Frontend/main.qml") ) );


    return app.exec();
}


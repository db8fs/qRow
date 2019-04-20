#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QMessageLogger>
#include <QDebug>
#include <QCommandLineParser>

#include "Backend/Analysis/Model/WorkoutModel.h"
#include "Backend/Analysis/Model/SessionModel.h"
#include "Backend/Analysis/WorkoutController.h"
#include "Backend/Video/VideoFilterOpenCV.h"

#include "Backend/Video/FaceDetector.h"
#include "SampleData.h"

#include "Backend/Serial/WaterrowerController.h"
#include "../System/Logging.h"

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

    qRegisterMetaType<cv::Mat>("cv::Mat");

    QGuiApplication app( argc, argv );
    QQmlApplicationEngine engine;

    app.setApplicationName( "qRow" );
    app.setApplicationVersion( "0.1.0" );

    ArgumentParser parser;
    parser.process( app );

    WaterrowerController waterrowerController;

    FaceDetector faceDetector;
    VideoFilterOpenCV openCVFilter( faceDetector );

    engine.rootContext()->setContextProperty( "waterrower",         &waterrowerController );
    engine.rootContext()->setContextProperty( "faceDetector",       &faceDetector);
    engine.rootContext()->setContextProperty( "openCVFilter",       &openCVFilter );
    engine.rootContext()->setContextProperty( "sessionsModel",      &sessionsModel );
    engine.rootContext()->setContextProperty( "workoutModel",       &workoutModel);
    engine.rootContext()->setContextProperty( "workoutController",  &workoutController );

    engine.load( QUrl(QStringLiteral("qrc:/src/Frontend/main.qml") ) );


    return app.exec();
}


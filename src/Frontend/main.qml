import QtQml 2.2
import QtQuick 2.1
import QtQuick.Controls 2.0


ApplicationWindow {
    id: root
    width: 1024
    height: 768
    visible: true

    WorkoutDetails {
        x: 300
        y: 10
    }

    WorkoutSessions {
        x : 10
        height: parent.height
    }

    /*
    VideoAnalysis {
        x: 300
        y: 300
    }
    */

    Item {
        x: 400
        y: 270
        width: 50
        height: 30

        Label {
            id: lbl
            text: "Alter: "
            font.pixelSize: 20
        }

        TextInput {
            validator: IntValidator{}
            x: lbl.x + lbl.width
            width: 20
            text: waterrower.age
            onTextChanged: { waterrower.age = text }
            font.pixelSize: 20
        }
    }

    Label {
        id: version
        x: 500
        y: 300
        text: "Model: " + waterrower.modelVersion + " SW V." + waterrower.majorVersion + "." + waterrower.minorVersion
        font.pixelSize: 30
    }

    Label {
        id: tankVolume
        x: 500
        y: 330
        text: "TankVolume: " + waterrower.tankVolume / 10. + "l"
        font.pixelSize: 30
    }

    Label {
        id: caloriesTotal
        x: 500
        y: 360
        text: "kCal total: " + waterrower.caloriesTotal / 100.
        font.pixelSize: 30
    }


    Label {
        id: heartrate
        x: 500
        y: 400
        text: "Heartrate: " + waterrower.heartRate + " bpm"
        font.pixelSize: 30
    }

    Label {
        id: maxPulse
        x: 500
        y: 430
        text: "max bpm: " + waterrower.maximumPulse + "bpm"
    }


    Label {
        id: strokes
        x: 500
        y: 460
        text: "Strokes: " + waterrower.strokes
        font.pixelSize: 30
    }

    Label {
        id: zone
        x: 500
        y: 490
        text: "Zone: " + waterrower.paceZone
        font.pixelSize: 30
    }


    Label {
        id: distance
        x: 500
        y: 520
        text: "Distance: " + waterrower.distance + "m"
        font.pixelSize: 30
    }


    Label {
        id: actualSpeed
        x: 500
        y: 550
        text: "ActualSpeed: " + waterrower.actualSpeed + "cm/s"
        font.pixelSize: 30
    }




    Label {
        id: averageSpeed
        x: 500
        y: 600
        text: "AverageSpeed: " + waterrower.averageSpeed + "cm/s"
        font.pixelSize: 30
    }



}

import QtQuick 2.0

Item {
    width: 300
    height: 300

    Component {
           id: workoutDelegate
           Item {
               width: 100
               height: 20
               Text { text: starttime; anchors.fill: parent }
               MouseArea {
                   anchors.fill: parent
                   acceptedButtons: Qt.LeftButton | Qt.RightButton
               }
           }
       }

    ListView {
        id: workoutDetailView
        interactive: true
        height: 500
        delegate: workoutDelegate
        model: workoutModel
        onReboundChanged:  strokeCompleted
    }

}

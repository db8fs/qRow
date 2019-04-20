import QtQuick 2.0

Item {

    Component {
           id: sessionsDelegate
           Item {
               width: 100
               height: 20
               Text { text: starttime; anchors.fill: parent }
               MouseArea {
                   anchors.fill: parent
                   acceptedButtons: Qt.LeftButton | Qt.RightButton
                   onClicked: workoutController.setSession( index );
               }
           }
       }


    ListView {
        id : sessionsView
        interactive: true
        height: 500
        //anchors.fill: parent
        model: sessionsModel
        delegate: sessionsDelegate
    }

}

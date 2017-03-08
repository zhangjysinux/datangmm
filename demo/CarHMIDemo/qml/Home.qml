import QtQuick 2.0

Image {
    id: root
    anchors.fill: parent

    property bool online: false
    property bool useWebkit: false
    property var styles: Styles {}

    source: "qrc:/qml/res/imgs/carousel_bg_v2o.png"
    clip: true

    MyCarFailureView {
        id: carFailureView

        width: parent.width
        height: parent.height
    }

    AppContainer {
        opacity: carFailureView.active ? 0.0 : 1.0;
        width: parent.width
        height: parent.height

        Behavior on opacity {
            NumberAnimation {
                duration: 500
                easing.type: Easing.OutExpo
            }
        }
    }

    // Cycle through car orientation
    MouseArea {
        width: 100
        height: 100
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            carFailureView.active = true;
            carFailureView.advanceFailure();
        }
    }

    // Hide car
    MouseArea {
        width: 100
        height: 100
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        onClicked: {
            carFailureView.active = false;
            carFailureView.hideFailures();
        }
    }
}



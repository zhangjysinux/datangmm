import QtQuick 2.0

Item {
    MyCarAdjustor {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 200
        }
    }

    Row {
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 200
        }

        ToggleButton {
            inactiveImage: "res/imgs/btn-auto-off.png"
            activeImage: "res/imgs/btn-auto-on.png"
        }

        ToggleButton {
            inactiveImage: "res/imgs/btn-fan-off.png"
            activeImage: "res/imgs/btn-fan-on.png"
        }

        ToggleButton {
            inactiveImage: "res/imgs/btn-aircon-ff.png"
            activeImage: "res/imgs/btn-aircon-on.png"
        }

        ToggleButton {
            inactiveImage: "res/imgs/btn-winheat-off.png"
            activeImage: "res/imgs/btn-winheat-on.png"
        }

        ToggleButton {
            inactiveImage: "res/imgs/btn-windefrost-off.png"
            activeImage: "res/imgs/btn-windefrost-on.png"
        }
    }
}

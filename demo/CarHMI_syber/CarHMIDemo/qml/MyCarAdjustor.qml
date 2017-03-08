import QtQuick 2.0

Item {
    width: background.width
    height: background.height

    Image {
        id: background
        source: "res/imgs/aircon-bg.png"
    }

    Image {
        source: "res/imgs/20C.png"
        x: 219
        y: 51
    }

    Image {
        source: "res/imgs/22C.png"
        x: 622
        y: 51
    }

    FlowSlider {
        anchors {
            left: parent.left
            leftMargin: 12
            top: parent.top
            topMargin: 10
        }
        value: 3
    }

    FlowSlider {
        anchors {
            left: parent.left
            leftMargin: 415
            top: parent.top
            topMargin: 10
        }
        value: 7
    }

    ToggleButton {
        activeImage: "res/imgs/arrow-02.png"
        width: 58
        height: 46
        x: 163
        y: 119
        toggled: true
    }

    ToggleButton {
        activeImage: "res/imgs/arrow-01.png"
        width: 46
        height: 48
        x: 109
        y: 158
    }

    ToggleButton {
        activeImage: "res/imgs/arrow-01.png"
        width: 46
        height: 48
        x: 513
        y: 158
        toggled: true
    }

    ToggleButton {
        activeImage: "res/imgs/arrow-02.png"
        width: 58
        height: 46
        x: 565
        y: 119
    }

    ToggleButton {
        activeImage: "res/imgs/btn-seat-heat.png"
        width: 70
        height: 62
        x: 257
        y: 170
    }

    ToggleButton {
        activeImage: "res/imgs/btn-seat-heat.png"
        width: 70
        height: 62
        x: 660
        y: 170
        toggled: true
    }
}

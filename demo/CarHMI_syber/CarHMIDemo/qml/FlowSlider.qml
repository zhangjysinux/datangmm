import QtQuick 2.0

Item {
    id: slider
    width: background.width
    height: background.height

    property int value: 1
    property int min: 1
    property int max: 9

    Image {
        anchors.centerIn: parent
        id: background
        source: "res/imgs/fan-step-0" + slider.value + ".png"
    }

    MouseArea {
        anchors.fill: parent

        function update() {
            var value = (slider.height - mouseY) / slider.height * slider.max;
            value = Math.min(slider.max, Math.max(slider.min, value));
            slider.value = value;
        }

        onClicked: update()
        onPositionChanged: update()
    }
}

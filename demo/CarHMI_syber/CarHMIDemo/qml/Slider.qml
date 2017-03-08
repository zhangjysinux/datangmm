import QtQuick 2.0

Item {
    signal valueChanged(real value)

    Image {
        id: sliderBg

        source: "res/imgs/slider_bg.png"
    }

    Image{
        source: "res/imgs/slider_pointer.png"
        y: -height/4 + 7
        onXChanged: valueChanged((x-sliderBg.x)/(sliderBg.x + sliderBg.width - width))

        MouseArea {
            anchors.fill: parent
            anchors.margins: -10
            drag.target: parent
            drag.axis: Drag.XAxis
            drag.minimumX: sliderBg.x
            drag.maximumX: sliderBg.x + sliderBg.width - parent.width
            onPressed: setHighlight(parent)
            onReleased: removeHighlight(parent)
        }
    }
}

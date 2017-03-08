import QtQuick 2.0

Item {
    anchors.fill: parent

    Image {
        id: imgGraph

        source: "res/imgs/graph_bg.png"
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 90
    }

    Image {
        id: imgWarning

        source: "res/imgs/icon_warning_oil.png"
        anchors.left: imgGraph.left
        anchors.top: parent.top
        anchors.topMargin: 60
    }

    Text {
        text: "Oil level LOW"
        font.pixelSize: styles.titleSize
        color: styles.orange
        anchors.left: imgGraph.left
        anchors.top: imgWarning.bottom
    }
}

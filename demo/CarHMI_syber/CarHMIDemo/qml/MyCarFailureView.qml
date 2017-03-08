import QtQuick 2.0
import CarHMIDemo 1.0

Item {
    property alias animating: animTimer.running
    property alias currentFailure: carItem.currentOrientation
    property bool active: false

    function advanceFailure()
    {
        currentFailure++;
        if (currentFailure >= carItem.orientations.length-1)
            currentFailure=1;
        console.log("current failure set to", currentFailure)
    }

    function hideFailures()
    {
        currentFailure=0;
        animating=false;
    }

    Text {
        text: carItem.note
        horizontalAlignment: Text.AlignHCenter
        anchors.topMargin: 100
        anchors.fill: parent
        font.family: styles.normalFont
        font.pixelSize: text ? styles.titleSize : 1
        color: styles.orange
        opacity: text ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation {
                duration: 500
                easing.type: Easing.OutExpo
            }
        }

        Behavior on font.pixelSize {
            NumberAnimation {
                duration: 500
                easing.type: Easing.OutElastic
            }
        }
    }

    CarItem {
        id: carItem

        property var orientations: [
            makeOrientationEntry(),
            makeOrientationEntry("Fuel LOW"),
            makeOrientationEntry("Oil level LOW", 205),
            makeOrientationEntry("Door open", 67, -45),
            makeOrientationEntry("Door open", 107, -90),
            makeOrientationEntry("Door open", 76, 90),
            makeOrientationEntry("Door open", 117, 180),
            makeOrientationEntry("Trunk open", 147, -90),
            makeOrientationEntry("Tire pressure LOW", 216),
            makeOrientationEntry("Tire pressure LOW", 217, -90, -30),
            makeOrientationEntry("Tire pressure LOW", 218, 90, -30),
            makeOrientationEntry("Tire pressure LOW", 219, 180),
            makeOrientationEntry("Check light", 39),
            makeOrientationEntry("Check light", 50, -90),
            makeOrientationEntry("Check light", 40, 90),
            makeOrientationEntry("Check light", 52, -180),
        ]
        property int currentOrientation: 0

        function makeOrientationEntry(text, index, dk, di) {
            return {
                text: text || "",
                index: index || -1,
                di: di || 0.0,
                dj: 0.0,
                dk: dk || 0.0,
                steps: 25
            }
        }

        anchors.fill: parent
        focus: true
        source: "res/meshes/car9.3ds"

        onCurrentOrientationChanged: {
            var entry = carItem.orientations[carItem.currentOrientation];
            setHighlight(entry.index);
            setNote(entry.text);
            setOrientation(entry.di, entry.dj, entry.dk, entry.steps);
        }

        Keys.onPressed: {
            var n = Number(event.key - Qt.Key_0);
            if (n >= 0 && n < orientations.length) {
                currentOrientation = n;
            }
        }

        // Animate car failures by looping through them
        Timer {
            id: animTimer

            interval: 2000
            repeat: true
            onTriggered: {
                carItem.currentOrientation++;
                if (carItem.currentOrientation > carItem.orientations.length-1)
                    carItem.currentOrientation = 0;
            }
        }
    }

    Image {
        source: "res/imgs/button_next.png"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        visible: active
    }

    Image {
        source: "res/imgs/btn-back.png"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        visible: active
    }

}

import QtQuick 2.0

AppBase {
    property int currentSpeed: 112
    property int minSpeed: 98
    property int maxSpeed: 127

    carouselView:
        DriveCarouselView {
            anchors.fill: parent
        }

    detailsView:
        DriveDetailsView {
            anchors.fill: parent
        }

    Timer {
        interval: 2000
        repeat: true
        running: true
        onTriggered: {
            var delta = Math.random(42);
            if (delta > 0.7)
                currentSpeed++;
            else if (delta < 0.3)
                currentSpeed--;

            currentSpeed += delta;
            currentSpeed = Math.max(minSpeed, currentSpeed);
            currentSpeed = Math.min(maxSpeed, currentSpeed);
        }
    }
}


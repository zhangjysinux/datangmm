import QtQuick 2.0

AppBase {
    carouselView: MusicCarouselView {
        anchors.fill: parent
    }

    detailsView: MusicDetailsView {
        anchors.fill: parent
    }
}

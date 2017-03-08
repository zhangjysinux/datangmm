import QtQuick 2.0

AppBase {
    carouselView:
        MyCarCarouselView {
            anchors.centerIn: parent
            anchors.fill: parent
    }

    detailsView:
        MyCarDetailsView {
            anchors.fill: parent
        }
}

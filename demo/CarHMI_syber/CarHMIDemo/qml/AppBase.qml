import QtQuick 2.0
import QtGraphicalEffects 1.0

MouseArea {
    id: appBase

    property alias carouselView: carouselView.children
    property alias detailsView: detailsView.children

    property real detailsOpacity: mode === _APP_VIEW_CAROUSEL ? 0.0 : 1.0
    property int mode: _APP_VIEW_CAROUSEL

    property int _APP_VIEW_CAROUSEL: 0
    property int _APP_VIEW_DETAILS: 1

    signal maximized
    signal minimized

    function maximize() {
        mode = _APP_VIEW_DETAILS;
    }

    function minimize() {
        mode = _APP_VIEW_CAROUSEL;
    }

    function setHighlight(item)
    {
        if (!item)
            return;

        highlightEffect.parent = item;
        highlightEffect.z = -1
        highlightEffect.visible = true;
        highlightEffect.source = item;
    }

    function removeHighlight(item)
    {
        if (!item)
            return;

        item.opacity = 1.0;
        highlightEffect.source = undefined;
        highlightEffect.parent = appBase;
        highlightEffect.visible = false;
    }

    width: carouselView.width
    height: carouselView.height

    Behavior on detailsOpacity {
        PropertyAnimation {
            duration: 500
        }
    }

    Image {
        id: carouselView

        source: "res/imgs/carousel_stripe_bg.png"
        anchors.left: parent.left
    }

    Image {
        id: detailsView

        source: "res/imgs/details_screen_bg.png"
        anchors.left: carouselView.right
        anchors.leftMargin: styles.uiAppGap
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        opacity: detailsOpacity
    }

    Image {
        id: btnClose

        source: "res/imgs/btn-back.png"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        visible: mode === _APP_VIEW_DETAILS

        MouseArea {
            anchors.fill: parent
            onPressed: appBase.setHighlight(btnClose)
            onReleased: {
                appBase.removeHighlight(btnClose);
                mode = _APP_VIEW_CAROUSEL;
                minimized();
            }
        }
    }

    Glow {
        id: highlightEffect

        anchors.fill: source
        radius: 20
        spread: 1.0
        color: "white"
        visible: false
    }
}

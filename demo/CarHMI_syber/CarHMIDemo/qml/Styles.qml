import QtQuick 2.0

Item {
    property color electricBlue: "#7DF9FF"
    property color blue: "#68b8d5"
    property color orange: "#f79f04"
    property color green: "#87c41c"

    property color backgroundColor: "black"
    property color highlightColor: electricBlue
    property color lowColor: "white"

    property string normalFont: normalFont.name
    property int normalSize: 30
    property int titleSize: 38
    property int hugeNumberSize: 100

    property int uiAppWidth: 392
    property int uiAppHeight: 800
    property int uiAppGap: 22
    property int tileWidth: 348
    property int tileHeightTop: 30
    property int tileHeightMiddle: 270
    property int tileHeightBottom: 290
    property int tileOffset: 240

    property int carouselIconWidth: 200
    property int carouselIconHeight: 200

    property string appBackgroundPicture: "res/imgs/carousel_stripe_bg.png"
    property color appBorderColor: "#a0a0a4"
    property int appBorderWeight: 1

    FontLoader {
        id: normalFont

        source:  "res/fonts/Panefresco500wtRegular.ttf"
    }
}

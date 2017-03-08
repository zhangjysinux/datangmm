import QtQuick 2.0

PathView {
    id: appContainer

    property int startX: -appContainer.width/3
    property int endX: appContainer.width + appContainer.width/3
    property int animationSpeed: 500

    property bool appOpen
    property var activeDelegate
    property int minimizedAppContX
    property int appWidth: styles.uiAppWidth

    width: 100
    height: 62
    highlightMoveDuration: 0
    interactive: !appOpen

    Behavior on x {
        PropertyAnimation {
            duration: animationSpeed
        }
    }

    Behavior on startX {
        PropertyAnimation {
            duration: animationSpeed
        }
    }

    Behavior on endX {
        PropertyAnimation {
            duration: animationSpeed
        }
    }

    path: Path {
        startX: appContainer.startX
        startY: appContainer.height/2

        PathLine {
            x: appContainer.endX
            y: appContainer.height/2
        }
    }

    model: ListModel {
        ListElement {
            appSource: "ContactsApp.qml"
        }
        ListElement {
            appSource: "NavigationApp.qml"
        }
        ListElement {
            appSource: "MyCarApp.qml"
        }
        ListElement {
            appSource: "MusicApp.qml"
        }
        ListElement {
            appSource: "DriveApp.qml"
        }
    }

    delegate: Loader {
        id: loader

        source: "qrc:/qml/" + appSource
        width: appWidth

        Behavior on width {
            SequentialAnimation {
                PropertyAnimation  {
                    duration: animationSpeed
                }
                ScriptAction {
                    script: if (activeDelegate) {
                                activeDelegate.maximize();
                                activeDelegate = undefined;
                            }
                }
            }
        }

        MouseArea {
            id: ma

            width: styles.carouselIconWidth
            height: styles.carouselIconHeight
            x: styles.carouselIconWidth / 2
            y: loader.item ? (parent.height-styles.carouselIconHeight)/2 : 0
            z: loader.z + 1
            //enabled: loader.width < appContainer.width
            onClicked: {
                if (loader.item.mode === loader.item._APP_VIEW_CAROUSEL) {
                    activeDelegate = loader.item;
                    minimizedAppContX = appContainer.x;
                    var realIndex = (index+offset)%count;
                    appContainer.x = appContainer.width*(2-realIndex)+appContainer.width/2
                    appWidth = appContainer.width;
                    appContainer.startX = -2*appContainer.width;
                    appContainer.endX = appContainer.width + 2*appContainer.width;
                    appOpen = true;
                } else {
                    appWidth = styles.uiAppWidth;
                    appContainer.startX = -appContainer.width/3;
                    appContainer.endX = appContainer.width + appContainer.width/3;
                    appOpen = false;
                    appContainer.x = minimizedAppContX;
                    loader.item.mode = loader.item._APP_VIEW_CAROUSEL;
                }
            }
        }

        Connections {
            target: loader.item
            onMinimized: {
                appWidth = styles.uiAppWidth;
                appContainer.startX = -appContainer.width/3;
                appContainer.endX = appContainer.width + appContainer.width/3;
                appOpen = false;
                appContainer.x = minimizedAppContX;
            }
        }
    }
}

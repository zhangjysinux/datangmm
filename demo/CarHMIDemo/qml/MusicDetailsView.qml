import QtQuick 2.0
import QtQuick.XmlListModel 2.0
import QtGraphicalEffects 1.0

Item {
    id: rootItem
    property size thumbSize: "250x250"
    property size textSize: "250x40"
    property int itemSpacing: 30

    anchors.fill: parent

    XmlListModel {
        id: albumCovers

        source: "res/albums/choons.xml"
        query: "/recordcollection/album"

        XmlRole {
            name: "cover"
            query: "cover/string()"
        }

        XmlRole {
            name: "artist"
            query: "artist/string()"
        }

        XmlRole {
            name: "title"
            query: "title/string()"
        }

        XmlRole {
            name: "year"
            query: "year/number()"
        }

        property int _SORT_NONE: 0
        property int _SORT_ARTIST: 1
        property int _SORT_TITLE: 2
        property int _SORT_YEAR: 3

        property var sortedOrder: []

        signal sorted

        property var data_getters: [
            undefined,
            function(entry) { return entry.artist; },
            function(entry) { return entry.title; },
            function(entry) { return entry.year; }
        ]

        function sort(sortOrder) {
            var i;
            var new_order = [];
            for (i = 0; i < count; i++) {
                new_order[i] = i;
            }

            if (sortOrder !== _SORT_NONE) {
                var getter = data_getters[sortOrder];
                new_order.sort(function(a, b) {
                    a = getter(get(a));
                    b = getter(get(b));

                    if (a < b) {
                        return -1;
                    } else if (a > b) {
                        return 1;
                    } else {
                        return 0;
                    }
                });
            }


            // The current new_order mapping is sorted index -> index,
            // for quick lookup below based on logical index we
            // flip this to index -> sorted index
            sortedOrder = [];
            for (i = 0; i < new_order.length; i++) {
                sortedOrder[new_order[i]] = i;
            }
            sorted();
        }
    }

    Flickable {
        id: coverList

        anchors.fill: parent

        property int cellWidth: thumbSize.width
        property int cellHeight: thumbSize.height + textSize.height

        property int columns: Math.min(2, Math.max(1, Math.floor(width / cellWidth)))

        flickableDirection: Flickable.VerticalFlick

        contentWidth: columns * cellWidth
        contentHeight: Math.floor(albumCovers.count / columns) * cellHeight + 300

        Repeater {
            model: albumCovers
            delegate: itemDrawer
        }

        transform: Rotation {
            angle: 25
            origin.x: coverList.width / 2
            origin.y: coverList.height / 2
            axis { x: 1; y: 0; z: 0 }
        }
    }

    Column {
        id: button_row

        spacing: 10

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 10

        Repeater {
            model: [
                [albumCovers._SORT_NONE, "Original Order"],
                [albumCovers._SORT_ARTIST, "Sort By Artist"],
                [albumCovers._SORT_TITLE, "Sort By Title"],
                [albumCovers._SORT_YEAR, "Sort By Year"]
            ]

            delegate: Button {
                onClicked: albumCovers.sort(modelData[0])
                text: modelData[1]
                width: 100
                height: 40
            }
        }
    }

    Component {
        id: itemDrawer

        Flipable {
            id: wrapper

            property int sortedIndex: index
            property int row: Math.floor(sortedIndex / coverList.columns)
            property int column: sortedIndex % coverList.columns
            property bool flipped: false

            width: coverList.cellWidth
            height: coverList.cellHeight
            x: flipped ?  coverList.width/2 - width/2 : (coverList.width / 2) - (column * (coverList.cellWidth + itemSpacing))
            y: flipped ? coverList.contentY + coverList.height/2 - height/2 : row * (coverList.cellHeight + itemSpacing)

            Connections {
                target: albumCovers
                onSorted: sortedIndex = albumCovers.sortedOrder[index];
            }

            Behavior on x {
                NumberAnimation {
                    duration: 500
                }
            }
            Behavior on y {
                NumberAnimation {
                    duration: 500
                }
            }

            Behavior on width {
                NumberAnimation {
                    duration: 500
                }
            }
            Behavior on height {
                NumberAnimation {
                    duration: 500
                }
            }


            front: Column {
                anchors.fill: parent
                Image {
                    id: playlistImage
                    width: wrapper.width
                    height: wrapper.height - playlistNameText.height
                    source: "res/albums/" + albumCovers.get(index).cover
                    asynchronous: true
                }

                Text {
                    id: playlistNameText
                    text: albumCovers.get(index).artist + " - " + albumCovers.get(index).title
                    width: wrapper.width
                    height: textSize.height
                    color: "white"
                    wrapMode: Text.Wrap
                    style: Text.Sunken
                    font.bold: true
                }
            }

            back: Rectangle {
                anchors.fill: parent
                color: "black"
                border.color: styles.highlightColor
                border.width: 1
                clip: true

                ListView {
                    id: backlistview

                    anchors.fill: parent
                    anchors.margins: 5

                    model: XmlListModel {
                        id: tracks_model
                        source: "res/albums/choons.xml"
                        query: "/recordcollection/album[" + (index+1) + "]/tracks/track"

                        XmlRole {
                            name: "tindex"
                            query: "@index/number()"
                        }

                        XmlRole {
                            name: "ttitle"
                            query: "@title/string()"
                        }

                        XmlRole {
                            name: "tlength"
                            query: "@length/string()"
                        }

                    }

                    delegate: Item {
                        width: backlistview.width
                        height: childrenRect.height

                        Text {
                            id: numText

                            text: tindex + ". "
                            color: "white"
                            width: 20
                            font.family: styles.normalFont
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignRight
                        }

                        Text {
                            text: ttitle
                            color: "white"
                            wrapMode: Text.ElideRight
                            anchors.left: numText.right
                            anchors.leftMargin: 5
                            anchors.right: lenText.left
                            font.family: styles.normalFont
                            font.pixelSize: 12
                        }

                        Text {
                            id: lenText

                            text: tlength ? " (" + tlength + ")" :""
                            color: "white"
                            wrapMode: Text.ElideLeft
                            anchors.right: parent.right
                            font.family: styles.normalFont
                            font.pixelSize: 12
                        }
                    }
                }
            }

            transform: Rotation {
                id: rotation
                origin.x: wrapper.width/2
                origin.y: wrapper.height/2
                axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
                angle: 0    // the default angle
            }

            states: [
                State {
                    name: "back"
                    when: wrapper.flipped

                    PropertyChanges {
                        target: rotation
                        angle: 180
                    }

                    PropertyChanges {
                        target: wrapper
                        scale: 2
                    }
                }
            ]

            transitions: [
                Transition {
                    SequentialAnimation {
                        PropertyAnimation {
                            property: "scale"
                            duration: 500
                            easing.type: Easing.OutCubic
                        }
                        NumberAnimation {
                            target: rotation
                            property: "angle"
                            duration: 400
                            easing.type: Easing.InOutCirc
                        }
                    }
                },
                Transition {
                    from: "back"

                    SequentialAnimation {
                        ParallelAnimation {
                            PropertyAnimation {
                                property: "scale"
                                duration: 900
                                easing.type: Easing.OutCubic
                            }
                            NumberAnimation {
                                target: rotation
                                property: "angle"
                                duration: 900
                                easing.type: Easing.InOutCirc
                            }
                        }
                        ScriptAction {
                            script: {
                                wrapper.parent = coverList.contentItem;
                            }
                        }
                    }
                }
            ]

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!flipped) {
                        wrapper.parent = trackViewer;
                    }
                    wrapper.flipped = !wrapper.flipped;
                }
            }
        }
    }

    Item {
        id: trackViewer

        width: coverList.width
        height: coverList.height
        y: -coverList.contentY

        MouseArea {
            anchors.fill: parent
            visible: trackViewer.children.length === 2
        }
    }
}

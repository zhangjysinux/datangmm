import QtQuick 2.0

Item {
    id: sectionScroller
    /*! 需要被处理的ListView*/
    property ListView listView: null

    /*! 控件x坐标与listView右边的距离，默认值为80 */
    property int positionXRightMargin: 80
    property bool bIsKeyPress: false
    Image {
        parent: listView; anchors.centerIn: parent;
        source: "qrc:/main/images/person_abc.png";
        sourceSize: Qt.size(128,150)
        Text{
            id: centerInText
            anchors.centerIn: parent
            color: "#ffffff"
            font.pixelSize: 80
            font.family: mainroot.newFont
        }
        scale: container.pressed ? 1 : 0
        Behavior on scale { NumberAnimation{ duration: 100 } }
    }

    MouseArea {
        id: container
        x: listView.x + listView.width - width; y: currentY();
        width: pressed ? 200 : positionXRightMargin; height: columnArea.height; parent: listView;
        preventStealing: true; //必须设置的属性，否则Flickable会独占鼠标事件。
        visible: sectionScroller.visible; // The parent now is listView

        function currentY(){
            var h = (listView.height - height) / 2;
            if(h < 100){
                h = 2*h > 100 ? 2*h - 100 : 0;
            }
            return h;
        }

        Column{
            id: columnArea
            Repeater{
                id: repeater
                Text{
                    id: listText
                    width: container.width + (container.pressed ? (200 - positionXRightMargin) : 0); height: paintedHeight;
                    verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter;
                    font.family: "方正兰亭准黑_GBK"
                    font.pixelSize: 26
                    text: modelData;
                    color: "white"

                    //color: (container.pressed && privateData.currentIndex === index) ? "#ff2828" : "#333333";
                    //color: (container.pressed && privateData.currentIndex === index) ? "#ffffff" : "#333333";
                }//home/zhangjy/SyberOS-SDK/SDKQt/qt530/usr/lib/qt5/qml/com/syberos/basewidgets/control2.0/items/CSectionScroller.qml
            }
        }

        onPressed: privateData.adjustPos(container.mouseY);

        onReleased: {
            var temp = null;
            var keyword = ''
            privateData.adjustPos(container.mouseY);
            if (privateData.currentText() === "#") {
                temp = listView.model.getContacterValue(0,"size");
                keyword = temp.toString().charAt(0);
                if (keyword >= "1" && keyword <= "9" ) {
                    listView.positionViewAtIndex(0, ListView.Beginning);
                    return
                }
            } else {
                for (var i = 0; i < listView.count; i++) {
                    temp = listView.model.getContacterValue(i,"size");
                    if (temp.toString().charAt(0) === privateData.currentText()) {
                        bIsKeyPress = true
                        listView.positionViewAtIndex(i, ListView.Beginning);
                        return;
                    }
                }
            }
        }

        onPositionChanged: privateData.adjustPos(container.mouseY);
    }

    Connections {
        target: listView
        ignoreUnknownSignals: true
        onCountChanged: {
            if(gInputContext.softwareInputPanelVisible) {
            } else {
                privateData.initSectionData(listView);
            }
        }
    }
    //    Connections{
    //        target: listView.model
    //        ignoreUnknownSignals: true
    //        onDataChanged: privateData.initSectionData(listView)
    //    }
    QtObject{
        id: privateData
        property var sections: null;
        property var sectionIndexes: null;
        property int currentIndex : -1;

        function adjustPos(y) {
            if( y >= 0 && y <= container.height){
                var val = Math.ceil(sections.length * y * 1.0 / container.height);
                currentIndex = val < 2 ? 0 : val - 1;
                centerInText.text = currentText()
            }
        }

        function currentText(){
            return sections[currentIndex];
        }

        function initSectionData(list) {
            if (list && list.model){
                sections = [];
                sectionIndexes = [];
                var tmpItem = null;
                var stringlist = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                var currentChar = '';
                for (var i = 0; i < stringlist.length; i++) {
                    currentChar = stringlist.charAt(i)
                    if (sections.indexOf(currentChar) === - 1) {
                        sections.push(currentChar);
                        sectionIndexes.push(i);
                    }
                }

                //                for (var i = 0; i < list.count; i++) {
                //                    //tmpItem = list.model.get(i);
                //                    //currentChar = tmpItem[list.section.property].charAt(0).toUpperCase(); // 首字母的大写形式
                //                    tmpItem = list.model.getContacterValue(i,"size");
                //                    currentChar = tmpItem.toString().charAt(0);
                //                    if (sections.indexOf(currentChar) === - 1) { // 此处可以完成去掉重复
                //                        sections.push(currentChar);
                //                        sectionIndexes.push(i);
                //                    }
                //                }
            }
            repeater.model = sections; // 变化完成后设置model
        }
    }
}

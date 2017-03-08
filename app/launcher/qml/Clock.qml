import QtQuick 2.0

//此Rect为子左边时钟界面
Rectangle {
    id: clockMainRect
    color: "gray"
    border.color: "lightgray"

    //显示日期框背景图片
    Image {
        id: dateImg
        anchors.top: parent.top
        anchors.topMargin: 28
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/img/date.png"
    }

    //显示日期,如:xxxx年xx月xx日
    Text {
        id: datetimeText
        anchors.top: dateImg.top
        anchors.topMargin: 30
        anchors.horizontalCenter: dateImg.horizontalCenter
        text: viewModel.date
        color: "white"
        font.bold: true
        font.pixelSize: 40
    }

    //显示星期
    Text {
        id: weekText
        anchors.top: datetimeText.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: dateImg.horizontalCenter
        text : viewModel.week
        color: "white"
        font.bold: true
        font.pixelSize: 40
    }

    //加载外部字体
    FontLoader {
        id: lcdFont
        source: "qrc:/font/LCDN.TTF"
    }
    //显示时间:只显示时和分,如:xx:xx:
    Text {
        id: timeText
        anchors.top: dateImg.bottom
        anchors.topMargin: 34
        anchors.left: parent.left
        anchors.leftMargin: (parent.width / 2) - (width * 2 / 3)
        text: viewModel.time
        color: "white"
        font.bold: true
        font.pixelSize: 50
        font.family: lcdFont.name
    }

    //显示秒
    Text {
        id: secText
        anchors.top: timeText.top
        anchors.left: timeText.right
        text: {
            if (viewModel.sec < 10)
                return "0" + viewModel.sec
            else
                return viewModel.sec
        }
        color: "white"
        font.bold: true
        font.pixelSize: 50
        font.family: lcdFont.name
    }

    //表模型Rect
    Rectangle {
        id: clockRect
        width: 273
        height: 273
        anchors.top: timeText.bottom
        anchors.topMargin: 32
        anchors.horizontalCenter: parent.horizontalCenter
        color: "transparent"

        //表盘背景
        Image {
            source: "qrc:/img/clockdial.png"
        }
        //时针
        Image {
            id: hourPoint
            x: parent.width / 2 - 9
            y: parent.height / 2 - 75
            source: "qrc:/img/hourpoint.png"
            transform: Rotation {
                id: hourRotation
                origin.x: 9; origin.y: 75
                angle: viewModel.hour * 30
            }
        }
        //分针
        Image {
            id: minPoint
            x: parent.width / 2 - 19 / 2
            y: parent.height / 2 - 96
            source: "qrc:/img/minpoint.png"
            transform: Rotation {
                id: minRotation
                origin.x: 19 / 2; origin.y: 96
                angle: viewModel.min * 6
            }
        }
        //秒针
        Image {
            id: secPoint
            x: (parent.width / 2) - (15 / 2)
            y: parent.height / 2 - 113
            source: "qrc:/img/secpoint.png"
            transform: Rotation {
                id: secRotation
                origin.x: 15 / 2; origin.y: 113
                angle: viewModel.sec * 6
            }
        }
        //表壳上面的光
        Image {
            source: "qrc:/img/light.png"
        }

    }
} // clock Rectangle END

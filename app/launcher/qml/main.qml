import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage: CPage {
        id: rootPage
        width:parent.width
        height:parent.height
        orientationPolicy: CPageOrientation.LockLandscape

        //----BEGIN----设置横屏格式
        Component.onCompleted: {
            if (gScreenInfo.currentOrientation === 2 ||
                    gScreenInfo.currentOrientation === 8) {
                rootPage.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                rootPage.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
        Connections {
            target: gScreenInfo
            onCurrentOrientationChanged:{
                if (gScreenInfo.currentOrientation === 2 ||
                        gScreenInfo.currentOrientation === 8) {
                    rootPage.statusBarHoldEnabled = false
                    gScreenInfo.setStatusBar(false);
                } else {
                    rootPage.statusBarHoldEnabled = true
                    gScreenInfo.setStatusBar(true);
                }
            }
        }
        //----END----

        contentAreaItem: Rectangle {
            color: "black"

            // 左侧钟表界面
            Clock {
                id: clockMainRect
                anchors.left: parent.left
                anchors.top: parent.top
                width: 400
                height: parent.height
            }

            // 右边菜单栏的界面
            Rectangle {
                id: menuRect
                width: parent.width - clockMainRect.width
                height: parent.height
                anchors.top: parent.top
                anchors.left: clockMainRect.right
                color: "black"

                Grid {
                    anchors.centerIn: parent
                    rows: 2
                    columns: 3
                    rowSpacing: 68
                    columnSpacing: 37

                    // 电话本按钮
                    ExButton {
                        id: phonebookButton
                        pressedImage: "qrc:/img/phonebook-clicked.png"
                        normalImage: "qrc:/img/phonebook.png"
                        onClicked: {
                            console.log("phonebookButton clicked");
                            viewModel.execPhonebookApp();
                        }
                    }

                    // 通话记录按钮
                    ExButton {
                        id: callHistoryButton
                        pressedImage: "qrc:/img/callHistory-clicked.png"
                        normalImage: "qrc:/img/callHistory.png"
                        onClicked: {
                            console.log("callHistoryButton clicked");
                            viewModel.execCallHistoryApp();
                        }
                    }

                    // 拨号盘按钮
                    ExButton {
                        id: dialButton
                        pressedImage: "qrc:/img/dial-clicked.png"
                        normalImage: "qrc:/img/dial.png"
                        onClicked: {
                            console.log("dialButton clicked");
                            viewModel.execDialApp();
                        }
                    }

                    // 群组消息按钮
                    ExButton {
                        id: groupMessageButton
                        pressedImage: "qrc:/img/groupMessage-clicked.png"
                        normalImage: "qrc:/img/groupMessage.png"
                        count: viewModel.unreadGroupMessage
                        onClicked: {
                            console.log("GroupMessageButton Clicked");
                            viewModel.execGroupMessageApp();
                        }
                    }

                    // 短消息按钮
                    ExButton {
                        id: messageButton
                        pressedImage: "qrc:/img/message-clicked.png"
                        normalImage: "qrc:/img/message.png"
                        count: viewModel.unreadMessage
                        onClicked: {
                            console.log("MessageButton Clicked");
                            viewModel.execMessageApp();
                        }
                    }

                    // 设置按钮
                    ExButton {
                        id: settingButton
                        pressedImage: "qrc:/img/setting-clicked.png"
                        normalImage: "qrc:/img/setting.png"
                        onClicked: {
                            console.log("settingButton clicked");
                            viewModel.execSettingApp();
                        }
                    }

                } // Grid-END

            } // menuRect-END

        } // contentAreaItem-END
    }// CPage-END
}


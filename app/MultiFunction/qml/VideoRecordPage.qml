import QtQuick 2.0
import QtMultimedia 5.2
import com.syberos.basewidgets 2.0

CPage {
    id: root

    contentAreaItem: Item {
        Camera {
            id: camera
            captureMode: Camera.CaptureVideo
            videoRecorder {
                resolution: "640x480"
                frameRate: 10
            }
        }

        VideoOutput {
            anchors.fill: parent
            fillMode: VideoOutput.Stretch
            autoOrientation: true
            source: camera
        }

        Loader {
            id: infoRect
            anchors.centerIn: parent
        }

        Component {
            id: infoComponent

            Rectangle {
                width: 492; height: 362
                border.color: "white"
                border.width: 5

                Image {
                    anchors.fill: parent
                    source: "qrc:/res/infobg.png"
                }

                Text {
                    id: text
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 124
                    text: "保存成功"
                    color: "white"
                    font.pixelSize: 30
                }

                CButton {
                    anchors.left: parent.left
                    anchors.top : text.bottom
                    anchors.leftMargin: 54
                    anchors.topMargin: 66
                    width: 170; height: 103
                    backgroundComponent: Item {
                        width: parent.width; height: parent.height
                        Image {
                            anchors.fill: parent
                            source: "qrc:/res/stillrecordbtn.png"

                        }
                    }
                    onClicked: {
                        infoRect.sourceComponent = null
                        //captureButton.state = "Capturing"

                    }
                }

                CButton {
                    anchors.right: parent.right
                    anchors.top: text.bottom
                    anchors.rightMargin: 54
                    anchors.topMargin: 66
                    width: 170; height: 103
                    backgroundComponent: Item {
                        width: parent.width; height: parent.height
                        Image {
                            anchors.fill: parent
                            source: "qrc:/res/viewfilebtn.png"
                        }
                    }
                    onClicked: {
                        pageStack.push("qrc:/qml/VideoViewPage.qml");
                        infoRect.sourceComponent = null;
                    }
                }
            }
        }

        // head
        Item {
            width: parent.width
            height: 96
            anchors.left: parent.left
            anchors.top: parent.top

            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: 0.2
            }

            Item {
                width: 96; height: 72
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 32

                Image {
                    anchors.centerIn: parent
                    source: "qrc:/res/back.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        pageStack.pop();
                    }
                }
            }

            Item {
                width: 72; height: 72
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 32

                Image {
                    anchors.centerIn: parent
                    source: "qrc:/res/switch.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }
        } //head_end

        // tail
        Item {
            width: parent.width
            height: 230
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: 0.2
            }

            Rectangle {
                width: 153; height: 153
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 38
                border.width: 4
                border.color: "#ffffff"
                color: "transparent"
                radius: width

                Rectangle {
                    id: captureButton
                    width: 133; height: 133
                    anchors.centerIn: parent
                    color: "#ff0000"
                    radius: width;
                    states: [
                        State {
                            name: "Capture"
                            StateChangeScript {
                                script: {
                                    captureButton.width = captureButton.height = 133;
                                    captureButton.radius = captureButton.width;
                                    camera.videoRecorder.stop();
                                    infoRect.sourceComponent = infoComponent;
                                }
                            }
                        },
                        State {
                            name: "Capturing"
                            StateChangeScript {
                                script: {
                                    captureButton.width = captureButton.height = 100;
                                    captureButton.radius = captureButton.width / 4;
                                    camera.videoRecorder.outputLocation = "/home/user/sinux/com.sinux.MultiFunction/videos/" + videoRecord.getCurrentDateTime() + ".mp4";
                                    camera.videoRecorder.record();
                                }
                            }
                        }
                    ]

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            captureButton.state =
                                    captureButton.state == "Capturing" ? "Capture" : "Capturing";
                        }
                    }
                }
            }
        }
    }
}

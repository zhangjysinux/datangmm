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

            imageCapture {
                onImageCaptured: {

                }
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
                        control.state = "VideoCapturing"
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
            id: control
            width: parent.width
            height: 230
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            state: "VideoCapture"

            states: [
                State {
                    name: "VideoCapture"
                    StateChangeScript {
                        script: {
                            captureButton.width = captureButton.height = 133;
                            captureButton.radius = captureButton.width;
                            captureButton.color = "#ff0000";
                            videoText.opacity = 1;
                            photoText.opacity = 0.2;
                            camera.captureMode = Camera.CaptureVideo;
                            camera.videoRecorder.stop();
                        }
                    }
                },
                State {
                    name: "VideoCapturing"
                    StateChangeScript {
                        script: {
                            captureButton.width = captureButton.height = 100;
                            captureButton.radius = captureButton.width / 4;
                            captureButton.color = "#ff0000";
                            videoText.opacity = 1;
                            photoText.opacity = 0.2;
                            camera.videoRecorder.outputLocation = "videos/" + videoRecord.getCurrentDateTime() + ".mp4";
                            camera.videoRecorder.record();
                        }
                    }
                },
                State {
                    name: "PhotoCapture"
                    StateChangeScript {
                        script: {
                            captureButton.width = captureButton.height = 133;
                            captureButton.radius = captureButton.width;
                            captureButton.color = "#ffffff";
                            videoText.opacity = 0.2;
                            photoText.opacity = 1;
                            camera.captureMode = Camera.CaptureStillImage;
                        }
                    }
                }
            ]

            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: 0.2
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                spacing: 30
                Text {
                    id: videoText
                    text: "视频"
                    font.pointSize: 14
                    color: "white"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            control.state = "VideoCapture"
                        }
                    }
                }
                Text {
                    id: photoText
                    text: "照片"
                    font.pointSize: 14
                    color: "white"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            control.state = "PhotoCapture"
                        }
                    }
                }
            }

            Rectangle {
                width: 153; height: 153
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                border.width: 4
                border.color: "#ffffff"
                color: "transparent"
                radius: width

                Rectangle {
                    id: captureButton
                    width: 133; height: 133
                    anchors.centerIn: parent
                    color: "#ff0000"
                    radius: width

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (control.state == "VideoCapture") {
                                control.state = "VideoCapturing";
                            } else if (control.state == "VideoCapturing") {
                                control.state = "VideoCapture";
                                infoRect.sourceComponent = infoComponent;
                            } else {
                                camera.imageCapture.captureToLocation("images/" + videoRecord.getCurrentDateTime() + ".jpg")
                            }
                        }
                    }
                }
            }
        }
    }
}

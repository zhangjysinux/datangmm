import QtQuick 2.0
import com.syberos.multimedia.camera 1.0

CameraRecorderPlugin {
    id: cameraRecorder
    enablePluginMode: true
    onVideoSaved: {
        console.log("video file: " , filePath);
    }
    Component.onDestruction :{
        gScreenInfo.setStatusBar(true)
        gScreenInfo.setStatusBarStyle("black")
        chatRoom.statusBarHoldItemColor("black")
    }
}

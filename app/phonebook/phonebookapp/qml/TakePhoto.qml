import com.syberos.multimedia.camera 1.0
import com.syberos.basewidgets 2.0
import QtQuick 2.0

CameraPlugin {
    id: __cameraPlugin

    Component.onCompleted:{
//        //chatRoom.statusBarHoldItemColor("black")
        gScreenInfo.setStatusBar(false)
        mainroot.setPopCamera(true)
        console.log("cccccccccccccccccccccccccccc")
    }
    Component.onDestruction: {
        gScreenInfo.setStatusBar(true)
        gScreenInfo.setStatusBarStyle("black")
//        chatRoom.statusBarHoldItemColor("black")
        mainroot.setPopCamera(false)
        console.log("ddddddddddddddddddddddddddddddd")

    }

//    Connections {
//        target: mainroot
//        onPopCamera: {
//            pageStack.pop()
//        }
//    }
}

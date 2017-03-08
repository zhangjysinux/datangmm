import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage:CPage{
        width:parent.width
        height:parent.height
        function callInComing(value,remoteIpAddr)
        {
            pageStack.push("qrc:/qml/MainPage.qml", {"handId":value,"remoteIpAddr":remoteIpAddr},true)
            //csDbus.setDisplayState(true)
        }
        function startVideoIn(callId , phoneNum)
        {
            //csDbus.setDisplayState(true)
            //viewModel.acceptAble = true
            pageStack.push("qrc:/qml/RealtimeVideoPage.qml", {"callId":callId,"phoneNum":phoneNum},true)
        }
        MainPage {
            anchors.fill: parent
        }
    }
}

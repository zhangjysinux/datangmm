import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage:CPage{
        width:parent.width
        height:parent.height
        orientationPolicy: CPageOrientation.LockLandscape

        Home {
            anchors.fill: parent
        }
    }
}

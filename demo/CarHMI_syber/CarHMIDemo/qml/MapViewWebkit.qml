import QtQuick 2.0
import QtWebKit 3.0

WebView {
    anchors.fill: parent
    url: online ? "res/map/MapLoader.html" : "res/map/MapLoaderOffline.html"
    onLoadingChanged: {
        console.log("WebView::onLoadingChanged:",
                    loadRequest.status, loadRequest.errorString, loadRequest.errorCode)
        if (loadRequest.status === WebView.LoadSucceededStatus)
            loadingText.visible = false;
    }
}


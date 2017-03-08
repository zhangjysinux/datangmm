import QtQuick 2.0
import com.syberos.basewidgets 2.0
import com.syberos.multimedia.photos 2.0

ManagerPreviewMain {
    id: photoViewer

    property var myindex: ""
    property string filepath: ""
    property var myparent: null
    barVisible: false
    toolBarVisible: false
    Component.onCompleted: {
        photoViewer.currentmodel = imageModel
        for (var i = 0; i< indexModel.count;i++)
        {
            if (myindex == indexModel.get(i).sources){
                photoViewer.currentindex = i
                break;
            }
        }
    }

    ListModel {
        id: indexModel
    }

    ListModel {
      id: imageModel
      Component.onCompleted: {
          getData.getImageMessageList()
      }
    }
    Connections {
        target: m_messageModel
        onSignalFindImage: {
            console.log("onSignalFindImage===",sources)
            indexModel.append({"sources":sources})
            var path = sources.substring(7,sources.length)
            imageModel.append({"url":"file://" + encodeURIComponent(path)})
        }
    }
    onStatusChanged: {
       if (status === CPageStatus.WillShow) {
           gScreenInfo.setStatusBar(false);
       } else if (status === CPageStatus.WillHide) {
           if (gScreenInfo.currentOrientation == 2 ||
                   gScreenInfo.currentOrientation == 8) {
               gScreenInfo.setStatusBar(false);
           } else {
               gScreenInfo.setStatusBar(true);
               gScreenInfo.setStatusBarStyle("black");
           }

       }
    }
    onClicked: {
        pageStack.pop(myparent, true)
    }



//    onFilepathChanged: {
//        console.debug("photo viewer image path:", filepath);
//        var paths = [];
//        imageModel.clear();
//        imageModel.append({"url": "file://" +  encodeURIComponent(filepath)});
//        currentindex = 0;
//        photoViewer.currentmodel = imageModel
//    }



} //end

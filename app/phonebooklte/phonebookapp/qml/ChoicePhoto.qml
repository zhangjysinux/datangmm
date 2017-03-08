import QtQuick 2.0
import com.syberos.multimedia.photos 2.0
import com.syberos.basewidgets 2.0

HeadPortraitBrowseMain{
    apertureShape: false
    property var parentSource: null
    /*!  这个信号在确认按钮按下时发出，url表示编辑完成的头像的路径 */
    onSetHeadPortrait:{
        console.debug("-----------onSetHeadPortrait---------url: ", url)
        var setUrl = "file://"+url
        parentSource.setSource(setUrl)
        pageStack.pop(parentSource)
    }
}


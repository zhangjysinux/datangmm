
function getFileName(filePath) {
    return filePath.substring(filePath.lastIndexOf("/")+1, filePath.length);
}

function getFileNameNoSuffix(filePath) {
    var fileName = getFileName(filePath);
    if (fileName.indexOf(".") < 0) {
        return fileName;
    } else {
        return fileName.substring(0, fileName.lastIndexOf("."));
    }
}

function getDir(filePath) {
    if (filePath === "")
        return "";
    else
        return filePath.substr(0, filePath.lastIndexOf('/'));
}

function getSizeStr(bytes) {
    var unit = ["B", "KB", "MB", "GB", "TB"]
    var i = 0
    if (bytes < 0) {
        print("bytes wrong: ", bytes)
        return "0"
    }

    while(bytes>1024 && i<unit.length) {
        bytes /= 1024.0
        i++
        //str = Number(bytes).toPrecision(4) + unit[i]
    }

    var precision = bytes >= 1000 ? 4 : 3
    return Number(bytes).toPrecision(precision) + unit[i]
}


function formatDate(date) {
    var datetime = new Date(date);
    var year = datetime.getFullYear();
    var month = datetime.getMonth() + 1;
    var day = datetime.getDate();
    var hour = datetime.getHours();
    var minute = datetime.getMinutes().toString();
    if(minute.length == 1)
        minute = "0" + minute;
    return year + "-" + month + "-" + day + " " + hour + ":" + minute;
}

function openFile(filepath, fileType) {
    console.debug("open file mimetype:", filepath, fileType);
    //var suffix = FM.getSuffix(fileName)
    if (fileType === "image") {
        __openImageFile(filepath);
    } else if (fileType === "video") {
        __openVideoFile(filepath);
    } else if (fileType === "music") {
        __openAudioFile(filepath);
    } else if (fileType === "text") {
        __openTextFile(filepath);
    } else {
        var filepathUrl = "file://" + filepath;
        console.debug("----x-Office open file----------=======================", filepath, filepathUrl);
         globalApp.runDocument("com.hyfsoft.XofficePro", "XofficePro", "open", "application/pdf", "国雅品.pdf")
    }
}

function __openImageFile(filepath) {
    pageStack.push("qrc:/qml/PhotoViewer.qml", { filepath: filepath });
    console.debug("---- open image file:", filepath);
}

function __openVideoFile(filepath) {
    var filepathUrl = "file://" + filepath;
    console.debug("---- open video file----------", filepath, filepathUrl);
    globalqApp.openUrl("video://play?fileuri=" + filepathUrl);
}

function __openAudioFile(filepath) {
    var comp = Qt.createComponent("qrc:/qml/MusicPlayer.qml");
    if (comp.status == Component.Ready) {
        var musicPlayer = comp.createObject(rootPage, { "filepath": filepath });
        //musicPlayer.exit.connect(function(){musicPlayer.destroy()})
    }
    console.debug("---- open audio file:", filepath);
}

function __openTextFile(filepath) {
    pageStack.push("qrc:/qml/TextViewer.qml", { fullfilepath: filepath })
    console.debug("---- open text file:", filepath);
}

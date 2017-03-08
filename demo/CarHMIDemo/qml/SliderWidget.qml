import QtQuick 2.0

Item {
    property string label
    property string unit
    property var valueList
    property real min
    property real max
    property int type
    property var current
    property int decimals

    property string valueStr

    property int _INTEGER: 0
    property int _REAL: 1
    property int _STRING: 2

    Component.onCompleted: setValue(min)

    function setValue(value)
    {
        switch (type)
        {
        case _INTEGER:
            current = min + (max-min)*value;
            current = Math.floor(current);
            break;
        case _REAL:
            current = min + (max-min)*value;
            current = current.toFixed(decimals);
            break;
        case _STRING:
            current = valueList[Math.floor(value*(valueList.length-1))];
            break;
        }

        if (label)
            valueStr = label + ": ";

        valueStr += current + unit;
    }

    width: 100
    height: 62

    Text {
        id: value

        text: valueStr
        font.family: styles.normalFont
        font.bold: true
        font.pixelSize: styles.normalSize
        color: styles.lowColor
    }

    Slider {
        anchors.top: value.bottom
        anchors.topMargin: 15
        onValueChanged: setValue(value)
    }
}

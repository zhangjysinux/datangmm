/*
 * This file is part of syberos-toolkit-core
 * Copyright (C) 2015 Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
 * Authors:
 *       Cai Zhenbin <caizhenbin@syberos.com>
 *
 * This software, including documentation, is protected by copyright controlled
 * by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
 */

import QtQuick 2.3

/*!
    \qmltype CSwipeTab2
    \inqmlmodule com.syberos.basewidgets
    \since 2.0
    \ingroup viewaddons
    \brief CSwipeTabView的标签页控件

    如果包含ListView控件，且将其指定给了listviewItem，则该ListView的列表项目在左右滑动切换标签页时会有阶梯移动的动画效果。
*/

Item {
    id: mytab
    anchors.fill: parent

    /*! 标签名称 */
    property string title: tabName

    /*! 标签页内可以应用阶梯效果的ListView控件， 当 displacementAnimationEnable 为true时有效 */
    property QtObject listviewItem: null

    /*! 是否启用阶梯移动动画效果，默认为true */
    property bool displacementAnimationEnable: true

    /*! \internal */
    property bool __inserted: false

    /*! \internal 标签名称，已废弃，请使用 title */
    property string tabName: ""

    /*! \internal */
    readonly property real shift: 0.5 //斜线与顶部相交的位置 0～1

    /*! \internal */
    readonly property real speedRate: 1.5 //Item向左右移动速度与tabview移速度的比例，值为1+shift

    /*! \internal */
    property var itemPos: [] //保存Item的动态位置

    /*! \internal */
    property var items: [] //保存需要移动的Item

//    Component {
//        id: decorator
//        ShaderEffectSource { }
//    }

    /*! \internal
        向左滑动开始时初始化子Item位置，排列成一条斜线，滑动方向为 \a direction
    */
    function initPos(direction) {
        var vc = listviewItem.contentItem.visibleChildren; //只移动可见的子Item

        var index = 0;
        var itemsWithPos = []; //保存可见的Item和它们的y值
        items = [];
        for(var i=0; i<vc.length; i++) {
            var myitem = vc[i];
            if(((myitem.y + myitem.height) > listviewItem.contentY) && (myitem.y < (listviewItem.contentY + listviewItem.height))) {
                itemsWithPos[index] = [myitem, myitem.y];
                index++;
            }
        }
        itemsWithPos.sort(function(x,y){return x[1] - y[1]}) //按y值从上到下排序

        var k = listviewItem.width / itemsWithPos.length; //斜线的斜率
        var b = shift * listviewItem.width; //斜线的截距

        for(var j=0; j<itemsWithPos.length; j++) { //计算可见子Item的位置排列成一条斜线的初始位置
//            var temp = itemsWithPos[j][0];
//            var dec = decorator.createObject(listviewItem.contentItem,
//                                             { x:temp.x, y:temp.y,
//                                               width: temp.width, height: temp.height,
//                                               live: false, sourceItem: temp,
//                                               hideSource:true });
//            items[j] = dec;
            items[j] = itemsWithPos[j][0];
            itemPos[j] = (k*j + b) * direction;
        }
    }

    /*! \internal
        以 \a dx 的一个倍数移动子Item位置，方向为 \a direction ，到位则不再移动
    */
    function doAnimate(dx, direction) {
        for(var i=0; i<items.length; i++) {
            itemPos[i] = itemPos[i] - dx*speedRate; //dx是tabview contentX的增量，各子Item需要以更快的速度移动
            var pos = itemPos[i];
            if(pos*direction < 0) //如果已经移动到位，则不再移动
                pos = 0;
            if(pos*direction >= 0)
                items[i].x = pos;
        }
    }

    /*! \internal */
    function restore() {
        for(var i=0; i<items.length; i++) {
            items[i].x = 0;
            //items[i].destroy();
        }
        items = [];
    }
}

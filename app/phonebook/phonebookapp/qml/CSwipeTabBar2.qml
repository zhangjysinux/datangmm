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
import com.syberos.basewidgets 2.0

/*!
    \qmltype CSwipeTabBar2
    \inqmlmodule com.syberos.basewidgets
    \since 2.0
    \ingroup views
    \brief 可左右滑动的多页控件，含有ListView的页滑动时带有条目逐个移动的动画

    CSwipeTabBar2是标签页容器，特点：1.Tab可以左右滑动；2.包含的ListView在左右滑动时有动画效果

    \image cswipetabview.gif

    使用 \l CSwipeTab 在 CSwipeTabView 中添加一个标签页。

    \qml
    CSwipeTabBar2 {
        id: mytabview

        tabBar: CSwipeTabBar {
            tabView: mytabview
        }

        CSwipeTab2 {
            title: "tab1"
            Rectangle { anchors.fill: parent; color: "red" }
        }

        CSwipeTab2 {
            title: "tab2"
            listviewItem: mylist
            displacementAnimationEnable: true //启用逐条移动动效
            ListView {
                id: mylist
                anchors.fill: parent
                spacing: 1
                model: 100
                delegate: Rectangle {
                    width: parent.width
                    height: 60
                    color: "steelblue"
                }
            }
        }
    }
    \endqml

    除默认样式的 \l CSwipeTabBar 之外，也可以使用另一个轻量级的tabBar： \l CLightTabBar
    \qml
    CSwipeTabBar2 {
        id: mytabview
        ......

        tabBar: CLightTabBar {
            tabView: mytabview
        }
        ......
     }
    \endqml
*/

FocusScope {
    id: root
    implicitWidth: 720
    implicitHeight: 1280
    clip: true

    /*!
        控件的标签导航栏，可以使用CSwipeTabBar或CLightTabBar、CTabBar：
        \qml
        CSwipeTabBar2 {
            id: myTabView
            tabBar: CLightTabBar { tabView: myTabView }
            ......
        }
        \endqml
        也可自己定义代替默认的：
        \qml
        CSwipeTabBar2 {
            id: myTabView

            tabBar: Row {
                spacing: 40
                Repeater {
                    model: myTabView.tabModel
                    delegate: CLabel {
                        text: tab.title
                        color: index === myTabView.currentIndex ? "red" : "gray"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: myTabView.currentIndex = index;
                        }
                    }
                }
            }
            ......
        }
        \endqml
    */
    property Item tabBar: null

    /*! 当前标签页的索引值，改变时切换标签，有动画 */
    property int currentIndex: 0

    /*! 标签页的数量 */
    readonly property int count: tabModel.count

    /*! 标签是否可见 */
    property bool tabVisible: true

    /*!
        \qmlproperty enumeration CSwipeTabView::tabPosition
        标签栏位置，取值为：

        \list
        \li Qt.TopEdge (default) - 位于顶部
        \li Qt.BottomEdge - 位于底部
        \endlist
    */
    property int tabPosition: Qt.TopEdge

    /*! \internal tabBar普通标题颜色 */
    property color titleColor: tabBar.hasOwnProperty("titleColor") ? tabBar.titleColor : "gray"

    /*! \internal tabBar高亮标题颜色 */
    property color titleHighlightColor: tabBar.hasOwnProperty("titleHighlightColor") ? tabBar.titleHighlightColor : "black"

    /*! \internal tabBar标题字体大小 */
    property int titlePixelSize: tabBar.hasOwnProperty("titlePixelSize") ? tabBar.titlePixelSize : 32

    /*! \internal 标签页间隔 */
    property int tabSpacing: tabBar.hasOwnProperty("tabSpacing") ? tabBar.tabSpacing : 0

    /*! \internal 高亮条颜色 */
    property color highlighterColor: tabBar.hasOwnProperty("highlighterColor") ? tabBar.highlighterColor : "red"

    /*! 包含CSwipeTab的ListModel，自定义的tabBar可以获取标签title */
    property ListModel tabModel: ListModel { }

    /*!
        \qmlproperty bool CSwipeTabView::interactive

        Tab是否可以左右滑动，默认为true
    */
    property alias interactive: tabContentView.interactive

    property bool bIsLandscape: false
    property int icountx: 0
    /*!
        定位标签页到 \a index，无动画
    */
    function positionViewAtIndex(index) {
        tabContentView.contentX = index * tabContentView.width;
        root.currentIndex = index;
        switchAnimationEnabled = false;
        console.log("ccccccccccccc:",contentX);
    }

    /*! 获取位于 \a index 的 \l CSwipeTab 控件. */
    function getTab(index) {
        var data = tabModel.get(index)
        return data && data.tab
    }

    /*! \internal */
    property bool __completed: false

    ListView {
        id: tabContentView

        property int direction: 1
        property QtObject targetItem: null
        property real originContentX: 0
        property real oldContentX: 0

        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: tabPosition == Qt.TopEdge ? parent.bottom : tabBar.top
        anchors.top: tabPosition == Qt.TopEdge ? tabBar.bottom : parent.top
        clip: true
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.DragOverBounds
        highlightMoveVelocity: tabContentView.width * 5
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        cacheBuffer: tabContentView.width * 10
        rebound: Transition {
            id: reboundTrans
            NumberAnimation {
                properties: "x"
                duration: 700
                easing.type: Easing.OutBounce
            }
        }

        delegate: Item {
            id: tabContainer
            width: tabContentView.width
            height: tabContentView.height
            property QtObject tabItem: tab
            Component.onCompleted: {
                tabItem.parent = tabContainer
                tabItem.anchors.fill = tabContainer
            }
        }

        onMovementEnded: {
            root.currentIndex = tabContentView.currentIndex;
            if(isCuteTab)
                targetItem.restore();
            targetItem = null;
            console.log("aaaaaaaaaaaaaaa:",contentX);
        }

        onInteractiveChanged: {
            if(interactive)
                gSystemUtils.grabMouse(tabContentView);
            else
                gSystemUtils.ungrabMouse(tabContentView);
        }

        property bool isCuteTab: targetItem && targetItem.listviewItem && targetItem.displacementAnimationEnable
        property bool isCuteTabBar: tabBar && (tabBar.hasOwnProperty("__isSwipeTabBar"))
        property int fromIndex: 0
        property int toIndex: 0

        onContentXChanged: {
            console.log("123123123123123:",contentX,bIsLandscape);
            if (bIsLandscape && contentX < 790) {
                contentX = 790;
                console.log("1111:",contentX);
                return;
            }
            else if (bIsLandscape == false && contentX < 360) {
                contentX = 360;
                console.log("2222:",contentX);
                return;
            } else {
                if (bIsLandscape && contentX > 1280) {
                    contentX = 1280;
                    console.log("333:",contentX);
                    return;
                } else if (bIsLandscape == false && contentX > 720) {
                    contentX = 720;
                    console.log("444:",contentX);
                    return;
                }
            }

            icountx = contentX;
            console.log("5555:",contentX);
            if(moving) {
                if(!targetItem) {
                    direction = (contentX > oldContentX) ? 1 : -1;
                    var pos = (contentX > oldContentX) ? contentX + width : contentX;
                    pos = (contentX > oldContentX) ? contentX + width : contentX;
                    var nextItem = tabContentView.itemAt(pos, 0);
                    if(nextItem && nextItem !== tabContentView.currentItem) {
                        targetItem = nextItem.tabItem;
                        if(targetItem && targetItem.listviewItem && targetItem.displacementAnimationEnable) {
                            console.log("aaaaccccccc:",contentX);
                            targetItem.initPos(direction);
                        }
                    }
                }

                if(isCuteTab) {
                    console.log("aaaacbbbbbbb:",contentX);
                    targetItem.doAnimate(contentX - oldContentX, direction);
                }

                if(isCuteTabBar) { //&& moving) {
                    fromIndex = direction > 0 ? (contentX - originX > 0 ? (contentX - originX) / width : -1)
                                              : (contentX - originX) / width + 1;
                    toIndex = fromIndex + direction;
                    if(fromIndex != toIndex) {
                        var percent = 0;
                        if(fromIndex >= count || toIndex >= count)
                            percent = (contentX - originX - (count-1)*width)/(width*(toIndex-fromIndex));
                        else if(fromIndex < 0 || toIndex < 0) {
                            percent = (contentX - originX) / (width*(toIndex-fromIndex));
                        } else
                            percent = (contentX - originX - fromIndex*width)/(width*(toIndex-fromIndex));
                        console.log("percent:",percent)
                        tabBar.moveHighlighterPercent(fromIndex, toIndex, percent);
                    }
                }
            }

            oldContentX = contentX;
        }

        //解决转屏改变currentIndex问题
        onCurrentIndexChanged: {
            if(currentIndex != root.currentIndex && !moving) {
                tabContentView.currentIndex = Qt.binding(function(){return root.currentIndex});
            }
        }
    }

    /*! \internal */
    property int lastIndex: -1
    /*! \internal */
    property bool switchAnimationEnabled: true

    onCurrentIndexChanged: {
        if(!tabContentView.moving && currentIndex != tabContentView.currentIndex && tabContentView.isCuteTabBar) {
            if(switchAnimationEnabled)
                tabBar.switchIndexWidthAnimation(lastIndex, currentIndex);
            else {
                tabBar.initHighlighter();
                switchAnimationEnabled = true;
            }
        }
        lastIndex = currentIndex;
    }

    /*! \internal */
//    property bool changedByRotation: false

//    Connections{
//        target: gScreenInfo
//        ignoreUnknownSignals: true
//        onCurrentOrientationChanged: {
//            changedByRotation = true;
//        }
//    }

    onChildrenChanged: {
        if (__completed)
            addTabs(root.children);
    }

    /*! \internal */
    function addTabs(tabs) {
        for (var i = 0 ; i < tabs.length ; ++i) {
            var tab = tabs[i];
            if (tab.hasOwnProperty("__inserted") && !tab.__inserted) {
                tab.__inserted = true;
                tabModel.append({tab: tab});
            }
        }
    }

    Component.onCompleted: {
        addTabs(root.children);
        tabContentView.model = tabModel;
        tabContentView.currentIndex = Qt.binding(function(){return root.currentIndex});
        tabContentView.positionViewAtIndex(root.currentIndex, ListView.Beginning);

        if(!tabBar) {
            tabBar = Qt.createComponent("CSwipeTabBar.qml").createObject(root, {tabView: root});
        } else
            tabBar.parent = root;

        lastIndex = currentIndex;
        __completed = true;
    }
    onTabBarChanged: {
        tabBar.parent = root;
    }
}

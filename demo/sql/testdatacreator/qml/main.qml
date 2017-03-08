import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage:CPage{
        width:parent.width
        height:parent.height

        Column {
            anchors.fill: parent
            spacing: 4

            CButton {
                text: "LocalInfo"
                onClicked: {
                    viewModel.startCreateLocalInfomation()
                }
            }

            CButton {
                text: "50"
                onClicked: {
                    viewModel.startCreateData1()
                }
            }

            CButton {
                text: "500"
                onClicked: {
                    viewModel.startCreateData2()
                }
            }

            CButton {
                text: "5000"
                onClicked: {
                    viewModel.startCreateData3()
                }
            }

            CButton {
                text: "Clear"
                onClicked: {
                    viewModel.clearAllData()
                }
            }
        }
    }
}


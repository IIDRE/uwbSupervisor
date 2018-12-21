import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

ColumnLayout {
    property bool deviceConnected:  controler_serial_device.cnxStatus
    Title{
        text: qsTr("Serial communication")
    }

    RowLayout{
        Layout.fillWidth:  true
        id:device_com_setting

        Column{
            Layout.minimumWidth:  145
            Layout.alignment: Qt.AlignTop
            Label{color: "black" ; text: "port"}
            ComboBox{
                property int  lastCurIndex: 0
                id:select_device
                currentIndex: lastCurIndex
                model: model_portAvaible
                width: parent.width
                textRole: "info"
                onActivated: {
                    console.log("combo onActivateddevice changed" + index + " "+currentIndex+" "+count)
                    lastCurIndex = index
                }
                onCurrentIndexChanged: {
                    console.log("combo onCurrentIndexChanged changed" +currentIndex+" "+count)
                    if(currentIndex < 0 ) currentIndex = lastCurIndex;
                }
                enabled:!deviceConnected
            }

        }
        Column{
            Layout.alignment: Qt.AlignTop
            id:baudrate
            Layout.minimumWidth:  145
            Label{color: "black" ; text: "baud rate"}
            ComboBox{
                id:select_baudrate
                currentIndex:  controler_serial_device.getIdxBaudrate(115200)
                model: model_baudrate
                width: parent.width
                enabled:! deviceConnected
            }

        }
        Button {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            // Layout.preferredHeight: baudrate.height
            id: control
            text: controler_serial_device.cnxStatus ? qsTr("Disconnect"): qsTr("Connect")
            enabled: select_device.count > 0
            contentItem: Text {
                text: control.text
                font: control.font
                opacity: enabled ? 1.0 : 0.3
                color: "black"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            onClicked: {
                if(! controler_serial_device.cnxStatus)
                    controler_serial_device.connectToSerialPort(select_device.currentIndex,select_baudrate.currentIndex)
                else
                    controler_serial_device.diconnectSerialPort()
            }

        }
    }
    Label{
        text: controler_serial_device.cnxInfo
    }

}

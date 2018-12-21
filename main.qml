import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0


ApplicationWindow{
    visible: true
    //    width: 640
    //    height: 480
    title: qsTr("RTLS by IIDRE")
    // flags: Qt.Window
    visibility: Window.Windowed



    RowLayout {
        id: layout
        anchors.fill: parent
        spacing: 6
        ZoneMap {

            Layout.fillWidth: true
            Layout.fillHeight:  true

            Layout.minimumWidth: 600
            Layout.minimumHeight: Layout.minimumWidth/1.3

            Layout.preferredWidth: 900
            Layout.preferredHeight:  Layout.preferredWidth/1.3


        }
        Rectangle{
            id:root
            gradient: Gradient {
                GradientStop { position: 1.2; color:"#e44f3f" }
                GradientStop { position: 0 ;color: "#ffffff"}
            }

            //Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 200
            Layout.preferredWidth: 450
            Layout.maximumWidth: 600

            ColumnLayout{
                anchors.fill: parent
                Image {
                    Layout.fillWidth:  true
               //     Layout.maximumWidth: 300

                    id: image
                    //width: root.width
                    source: "../images/iidre.png"

                    fillMode: Image.PreserveAspectFit
                }
                DeviceControlSetting{
                    id:device_control
                     Layout.fillWidth:  true
                    Layout.leftMargin: 5
                    Layout.rightMargin:  5
                    Layout.alignment: Qt.AlignLeft
                    onDeviceConnectedChanged: file_manager.deviceConnected =deviceConnected
                }
                DeviceUWBSetting{
                    parent_width :root.width-20
                    id:device_uwb
                     Layout.fillWidth:  true
                    Layout.leftMargin: 5
                    Layout.rightMargin:  5
                    Layout.alignment: Qt.AlignLeft

                }

                FileManager{
                     Layout.fillWidth:  true

                    id:file_manager;
                    Layout.topMargin: 20
                    Layout.leftMargin: 5
                    Layout.rightMargin:  5
                    onDeviceConnectedChanged: console.log(" file_manager device connected "+deviceConnected);
                }
                Repeater{
                    Layout.maximumWidth: 300
                    model:model_anchorFromDevice
                    delegate: AnchorDetails{
                        uid:model.UID
                        dist:model.Dist
                        radio:model.Radio
                    }
                }


                Rectangle{//force les element en haut
                    color:"transparent"
                    Layout.fillWidth:  true
                    Layout.fillHeight:   true
                    //   border.color: "black"
                }

            }

        }

    }
}

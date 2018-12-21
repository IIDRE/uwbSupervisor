import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

ColumnLayout {
    property real parent_width: 0
    Title{
        text: qsTr("UWB configuration")
    }
    Layout.fillWidth: true
    GridLayout{
        columns: 3

        UWB_ComboBox{
            name:"CHAN"
            Layout.alignment: Qt.AlignLeft
            model: model_uwb_CHAN
            currentIndex: model_uwb_obj_CHAN.idx
            onActivated: {
                controler_uwb_config.valueChange(name,index);
                model_uwb_obj_CHAN.idx=index
            }
            Layout.minimumWidth:   parent_width/3
        }
        UWB_ComboBox{
            name:"PRF"
            Layout.alignment: Qt.AlignHCenter
            model:model_uwb_PRF
            currentIndex: model_uwb_obj_PRF.idx
            onActivated: {
                controler_uwb_config.valueChange(name,index);
                model_uwb_obj_PRF.idx=index
            }
            Layout.preferredWidth:   parent_width/3
        }
        UWB_ComboBox{
            name:"TRXcode"
            Layout.alignment: Qt.AlignHCenter
            model:model_uwb_TRXcode
            currentIndex: model_uwb_obj_TRXcode.idx
            onActivated: {
                controler_uwb_config.valueChange(name,index);
                model_uwb_obj_TRXcode.idx=index
            }
            Layout.preferredWidth:   parent_width/3
        }
        UWB_ComboBox{
            name:"BR"
            Layout.alignment: Qt.AlignRight
            model:model_uwb_BR
            currentIndex: model_uwb_obj_BR.idx
            onActivated: {
                controler_uwb_config.valueChange(name,index);
                model_uwb_obj_BR.idx=index
            }
            Layout.preferredWidth:   parent_width/3
        }
        UWB_ComboBox{
            name:"PLEN"
            Layout.alignment: Qt.AlignRight
            model:model_uwb_PLEN
            currentIndex: model_uwb_obj_PLEN.idx
            onActivated: {
                controler_uwb_config.valueChange(name,index);
                model_uwb_obj_PLEN.idx=index
            }
            Layout.preferredWidth:   parent_width/3
        }
        UWB_ComboBox{
            name:"PAC"
            Layout.alignment: Qt.AlignLeft
            model:model_uwb_PAC
            currentIndex: model_uwb_obj_PAC.idx
            onActivated: {
                controler_uwb_config.valueChange(name,index);
                model_uwb_obj_PAC.idx=index
            }
            Layout.preferredWidth:   parent_width/3
        }

        Rectangle{
            Layout.columnSpan: 3
            height: 5
        }

        RowLayout{
            Layout.columnSpan: 3

            Rectangle{
                Layout.fillWidth: true
                Layout.preferredWidth : 20
            }

            Button{
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                text :"open config file"
                visible: false
                //  Layout.preferredWidth: parent_width/3

            }
            Button{
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                text :"save config"
                visible: false
                //  Layout.preferredWidth: parent_width/3
            }
            Button{

                //Layout.preferredWidth: parent_width/3
                Layout.alignment: Qt.AlignRight
                text: "Configure device"
                checkable: true
                onCheckedChanged:{
                    console.log("button check :"+checked)
                    controler_uwb_config.forceConfigure = checked

                    if(checked)
                        controler_device.configDevice()
                }

            }
            Rectangle{
                Layout.fillWidth: true
                Layout.preferredWidth : 20
            }
        }

    }

}

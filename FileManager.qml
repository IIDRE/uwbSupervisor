import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.3
GridLayout {

    property bool deviceConnected: false
    property string lastEvent: controler_file_manager.lastEvent

    onLastEventChanged: {
        label_lastEvent.text = lastEvent
    }

    FileDialog{

        id:fileDialog
        folder: shortcuts.documents
        selectMultiple: false
        selectFolder: false
    }

    id :root
    Layout.fillWidth:  true
    Layout.fillHeight:   true
    Layout.margins: 6

    columns: 2


    flow: GridLayout.LeftToRight

    RowLayout{
        visible: false
        Title{
            Layout.columnSpan: 2
            text: qsTr("File:")
        }
        CheckBox{
            id: octave_box
            text: qsTr("Octave")
            checkState: controler_file_manager.outputOctave ? Qt.Checked : Qt.Unchecked
        }
        CheckBox{
            id: matlab_box
            text: qsTr("Matlab")
        }
    }
    Rectangle{
        Layout.columnSpan: 2
        Layout.fillWidth:  true
        Layout.preferredHeight: childrenRect.height+10
        color:"transparent"
        border.color: "grey"
        Label{
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            id:label_lastEvent
            text:""
        }
    }

    Button_fileManager{
        id:importFromFlash
        Layout.fillWidth:  true
        text:"Import infra (flash)"
        f:fileDialog
        filters: ["json Files (*.json)"]
        selectExisting: false
        enabled: deviceConnected
        onReady:{
            controler_file_manager.importDataFromFlash(fileURL)
            label_lastEvent.text=qsTr(text+":"+controler_file_manager.lastFile)
        }
    }

    Button_fileManager{
        id:exportToFlash
        Layout.fillWidth:  true
        Layout.preferredWidth:   parent.width/2
        text:"Export infra (flash)"
        f:fileDialog
        filters: ["json Files (*.json)"]
        enabled: deviceConnected
        onReady:{
            controler_file_manager.exportDataToFlash(fileURL)
            label_lastEvent.text=qsTr(text+":"+controler_file_manager.lastFile)
        }
    }

    Button_fileManager{
        id:openMap
        Layout.fillWidth:  true
        Layout.preferredWidth:   parent.width/2
        text:"Open map"
        f:fileDialog
        filters: ["map Files (*.json *.dxf *.png *.jpg)","json Files (*.json)","dxf files (*.dxf)","png files (*.png)","jpg files (*.jpg)"]
        onReady:{
            controler_file_manager.openMap(fileURL)
            label_lastEvent.text=qsTr(text+":"+controler_file_manager.lastFile)
        }
    }

    Button_fileManager{
        id:saveMap
        Layout.fillWidth:  true
        text:"Save map"
        selectExisting: false
        filters: ["json Files (*.json)"]
        f:fileDialog
        onReady:{
            controler_file_manager.saveMap(fileURL)
            label_lastEvent.text=qsTr(text+":"+controler_file_manager.lastFile)
        }
    }
}


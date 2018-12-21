import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.3

/* fileDialog_img type FileDialog definie par le parent*/

Button{
    id:root
    property FileDialog f: FileDialog{}
    property bool export_file: false
    property bool selectExisting : true
    property var filters: [""]

    height:  30
    text: ""
    signal ready(var fileURL)

    function fileSelected(){
        console.log("export_file:"+export_file)
        f.accepted.disconnect(fileSelected)
        ready(f.fileUrl);
    }

    onClicked: {
        f.selectExisting = selectExisting
        f.selectFolder=export_file
        f.title = text
        f.nameFilters = filters
        f.accepted.connect(fileSelected)
        f.selectedNameFilter = filters[0];
        f.open();
    }
}


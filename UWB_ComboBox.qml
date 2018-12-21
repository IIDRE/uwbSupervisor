import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Column{
    id:root
    property string name: ""
    property alias model: _comboBox.model
    property alias currentIndex: _comboBox.currentIndex
    signal activated(int index)
    Layout.alignment: Qt.AlignTop

    Label{color: "black" ; text: name}
    ComboBox{
        id:_comboBox
        width: parent.width
        onActivated: {
            console.log("uwb setting "+root.name+" activated "+index)
            root.activated(index)
        }
    }

}

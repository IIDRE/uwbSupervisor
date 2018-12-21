import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Button{
    id:root
    property bool bp_state: false
    property url icon_source: ""
    property string  info: ""

    Layout.fillWidth: true
    Layout.preferredHeight: 30
    highlighted : true
    display: AbstractButton.IconOnly
    checkable: true

    padding: 0
    contentItem: Image {
        anchors.fill : parent
        source: icon_source
    }
    background: Rectangle{
        gradient: Gradient {
            GradientStop { position: 1.2; color:bp_state||root.pressed ?"#e44f3f":"#ffffff" }
            GradientStop { position: 0 ;color: bp_state||root.pressed?"#ffffff":"#ffffff"}
        }
       // color: bp_state?"#e44f3f" :"#ffffff"
        anchors.fill:parent

    }
    onClicked: bp_state = !bp_state


    hoverEnabled: true

    ToolTip.delay: 0
    ToolTip.timeout: 5000
    ToolTip.visible: info.length > 0 ?hovered:false
    ToolTip.text: qsTr(info)

}

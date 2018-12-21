import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import iidre.uwb.anchor 1.0

Popup{
    id:root

    property int idx : 0
    property int type: AnchorType.ANCHOR
    property int type_selected: AnchorType.ANCHOR
    property string uid: ""
    property string description: ""
    property real maxHeight: 0
    property real maxWidht: 0
    property point pos: Qt.point(0,0)


    signal validaded (int idx , string uid , string description , int type)

    function openDialogAnchor(idx , uid , coord , descip , type,pos){
        root.uid = uid
        root.type = type
        root.idx = idx
        root.pos = pos
        if(coord.y+height > maxHeight){
            root.y= coord.y-height
        }else{
            root.y= coord.y
        }

        if(coord.x+width > maxWidht){
            root.x= coord.x -  width
        }else{
            root.x= coord.x
        }


        visible=true
    }


    width: 400
    height: width/2
    modal: true
    focus: true
    margins: 0
    padding: 0
    opacity: 0

    onVisibleChanged: {
        if(!visible){
            x=0;
            y=0;

        }
    }

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    ColumnLayout{
        anchors.fill:parent
        Rectangle{
            id:title
            Layout.fillWidth: true
            width: parent.width
            height: childrenRect.height+10
            color:"black"
            Text {
                anchors.centerIn: parent
                id: name
                text: qsTr("Configuration "+uid +"("+pos.x+";"+pos.y+")")
                color: "white"
            }
        }

        Rectangle{
            gradient: Gradient {
                GradientStop { position: 1.2; color:"#e44f3f" }
                GradientStop { position: 0 ;color: "#ffffff"}
            }
            Layout.fillHeight: true
            Layout.fillWidth: true

            GridLayout{

                anchors.fill:parent
                anchors.margins: 10
                columns: 4
                flow : GridLayout.LeftToRight
                ComboBox {
                    editable: true
                    id:text_uid
                    model:controler_device.listOfID
                    property string  value: root.uid
                    Layout.columnSpan: 3

                    //placeholderText: qsTr("UID :"+uid)
                    onEditTextChanged: {
                        var n = parseInt(editText,16);
                        console.error("onTextEdited :"+editText + " "+n)
                        if(isNaN(n) || n.toString(16) !== editText ){
                            console.error("formating error :"+editText)
                            editText = editText.substring(0,editText.length-1);
                        }
                        value=n.toString(16)
                        console.error("onTextEdited :"+value)
                    }
                    onCurrentIndexChanged: {
                        console.log(currentText)
                        value=currentText

                    }

                    onVisibleChanged: {
                        console.log("onVisibleChanged "+uid)
                        editText=uid
                    }
                }
                RadioButton {
                    id:button_anchor
                    text: qsTr("Anchor")
                    checked: false
                    onVisibleChanged:  checked = type == AnchorType.ANCHOR
                    onCheckedChanged: type_selected=AnchorType.ANCHOR
                }
                TextField {
                    id:text_descrip
                    Layout.columnSpan: 3
                    placeholderText: qsTr("description")
                }
                RadioButton {
                    text: qsTr("Map")
                    checked: false
                    onVisibleChanged:  checked = type == AnchorType.POI
                    onCheckedChanged: type_selected=AnchorType.POI
                }
                Rectangle{
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color:"transparent"
                }

                RowLayout{
                    Layout.columnSpan: 4
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Rectangle{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color:"transparent"
                    }

                    Button{
                        text: qsTr("OK")
                        onClicked:{
                            validaded(idx,text_uid.value,text_descrip.text,type_selected)
                            root.visible=false
                        }
                    }

                    Button{
                        text: qsTr("Cancel")
                        onClicked: {
                            root.visible=false
                        }
                    }
                    Rectangle{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color:"transparent"
                    }

                }



            }

        }
        states: [
            State { when: root.visible;
                PropertyChanges {   target: root; opacity: 1.0    }
            },
            State { when: !root.visible;
                PropertyChanges {   target: root; opacity: 0.0    }
            }
        ]
        transitions: Transition{
            NumberAnimation { property: "opacity"; duration: 800}
        }
    }
}


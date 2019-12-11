/*Copyright Iidre SAS 01 janvier 2019 tech@iidre.com

Ce logiciel est un programme informatique servant à superviser et configurer le réseau UWB du kit Iidre.

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILLtelle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.*/
import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import iidre.uwb.anchor 1.0

Popup{
    id:root

    property int idx : 0
    property int type: AnchorType.UNKNOW
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
        console.log("open dialoAnchor "+type_selected)

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
                        console.log("onTextEdited :"+editText + " "+n)
                        if(isNaN(n) || n.toString(16) !== editText ){
                            console.log("formating error :"+editText)
                            editText = editText.substring(0,editText.length-1);
                        }
                        value=n.toString(16)
                        console.log("onTextEdited :"+value)
                    }
                    onCurrentIndexChanged: {
                        console.log(currentText)
                        value=currentText

                    }

                    onVisibleChanged: {
                        console.log("onVisibleChanged "+uid)
                        console.log("onVisibleChanged "+model)
                        console.log("onVisibleChanged "+currentIndex)

                        currentIndex=-1
                        editText=uid
                    }
                    Component.onCompleted: {
                        console.log("comboboc anchor onCompleted "+uid)
                        editText=uid
                    }
                }
                RadioButton {
                    id:button_anchor
                    text: qsTr("Anchor")
                    onVisibleChanged:  {

                        if(type == AnchorType.UNKNOW){
                            if(type_selected == AnchorType.ANCHOR ) checked = true;
                            else checked = false;
                        }else if(type == AnchorType.ANCHOR ) checked = true;
                        else checked = false;

                    }
                    onCheckedChanged: {
                        if(checked)
                            type_selected=AnchorType.ANCHOR
                    }
                }
                TextField {
                    id:text_descrip
                    Layout.columnSpan: 3
                    placeholderText: qsTr("description")
                }
                RadioButton {
                    text: qsTr("Map")
                    onVisibleChanged:  {
                        if(type == AnchorType.UNKNOW){
                            if(type_selected == AnchorType.POI ) checked = true;
                            else checked = false;
                        }else if(type == AnchorType.POI ) checked = true;
                        else checked = false;
                    }
                    onCheckedChanged: {
                        if(checked)
                            type_selected=AnchorType.POI
                    }
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


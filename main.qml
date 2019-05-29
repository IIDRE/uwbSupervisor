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
import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

ApplicationWindow{
    id:app_windows
    visible: true
    width: 1200
    height: 700
    title: qsTr("RTLS by IIDRE")
    flags: Qt.Window
    visibility: Window.Windowed

    Connections {
        target: controler_message_system
        onCritalMessage: {
            popup.printMsg(msg)
        }
    }

    Popup {
        function printMsg(msg){
            popup.open()
            info.text = qsTr(msg)
        }

        id: popup
        x: app_windows.width/2
        y: app_windows.height/2
        width: 0
        height: 0
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        margins:0
        padding: 0
        Rectangle{
            id:popup_frame
            //anchors.fill:parent
            width: 400
            height: 110
            radius: 20
            border.color: "grey"
            border.width: 5

            MouseArea{
                anchors.fill: parent
                onClicked: popup.close()
            }

            gradient: Gradient {
                GradientStop { position: 2; color:"#e44f3f" }
                GradientStop { position: 0 ;color: "#ffffff"}
            }



            RowLayout{
                anchors.fill:parent

                Image {
                    Layout.leftMargin: 10
                    height: 50
                    width:  50
                    id: img
                    source: "images/warning.png"
                }
                Text {
                    id: info
                    text:"aaaaa"
                    onTextChanged: {
                        var w = paintedWidth + 200
                        console.log("popup msg h:"+paintedHeight+" w:"+paintedWidth)
                        popup_frame.width = Math.max(popup_frame.width,w)
                    }
                }

            }
        }


    }

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
                    model:model_anchorFromDeviceHistory
                    delegate: AnchorDetails{
                        uid:model.UID
                        dist:model.Dist
                        radio:model.Weight
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

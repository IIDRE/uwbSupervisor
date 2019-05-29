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
            Label{color: "black" ; text: "baudrate"}
            ComboBox{
                id:select_baudrate
                currentIndex:  controler_serial_device.getIdxBaudrate(460800)
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

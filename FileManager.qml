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


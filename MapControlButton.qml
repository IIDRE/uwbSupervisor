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

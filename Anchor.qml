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
import QtQuick.Controls 2.4

Rectangle {
    id:root
    property int idx:0
    property point pos:Qt.point(0,0)
    property real dist: 0
    property string id_anchor: "xxxxx"
    property int radio: 0
    property alias print_info: zoneMouse.containsMouse
    property string description: ""
    property bool showInfo: false
    property real size: 5

    signal clicked(string uid, point coord)

    radius: width/2

    color: "black"
    width: size
    height: width
    x:(pos.x-(width/2))
    y:(pos.y-(height/2))

//    x:pos.x
//    y:pos.y

    onRadioChanged: {
        if(print_info){
            info_txt.text = formatId()
        }

    }

    function formatId(){


        var s = id_anchor + (" (%1 ; %2)").arg(pos.x).arg(pos.y)
        if (description.length > 0)
            s += " "+description

        if(radio != 0){
            var r = radio/1000
            s = s + r.toFixed(1)
        }

        return s;
    }

    Rectangle{
        border.color: "brown"
        color: "#00000000"    // ARGB fully transparent
        radius: width/2
        anchors.centerIn: parent
        width: parent.dist*2
        height: parent.dist*2

    }
    Label {
        rotation: 360-mapControls.rotationAction
        visible: showInfo
        anchors.right: root.left
        anchors.bottom: root.top
        id:info_txt
        text: formatId()
        padding:5
        background: Rectangle{
            border.color: "black"

        }
        onVisibleChanged: {
            anchors.margins =0
            if(visible ) {
                if((root.x + x ) < 0){
                    anchors.right = undefined
                    anchors.left = root.left
                    if(zoneMouse.containsMouse)
                           anchors.leftMargin = 20


                }else{
                    anchors.left = undefined
                    anchors.righ = root.left
                }

                if((root.y + y ) < 0){
                    anchors.bottom = undefined
                    anchors.top = root.top

                }else{
                    anchors.top = undefined
                    anchors.bottom = root.top

                }
            }
        }
    }
    Rectangle{
        id:info
        anchors.centerIn: parent
        width: 150
        height:10
        color: "transparent"
       MouseArea{
            id : zoneMouse

            hoverEnabled: true
            anchors.fill: parent
            onContainsMouseChanged: info_txt.visible = showInfo || containsMouse
            onClicked: root.clicked(id_anchor,pos)

        }
    }

    onShowInfoChanged:info_txt.visible = showInfo || zoneMouse.containsMouse
}

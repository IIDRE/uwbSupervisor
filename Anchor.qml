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
    property real  dist_weight: 1
    property alias print_info: zoneMouse.containsMouse
    property string description: ""
    property bool showInfo: false
    property real size: 5
    property bool showNlos : true

    signal clicked(string uid, point coord)

    radius: width/2

    color: "black"
    width: size
    height: width
    x:(pos.x-(width/2))
    y:(pos.y-(height/2))
    opacity: 0.9*root.dist_weight + 0.1;

//    x:pos.x
//    y:pos.y

    onRadioChanged: {
        console.log("radio :"+id_anchor+" "+radio)
        if(print_info){
            info_txt.text = formatId()
        }

    }

    function formatId(){


        var s = id_anchor + (" (%1 ; %2)").arg(pos.x).arg(pos.y)
        if (description.length > 0)
            s += " "+description

        if(radio != 0){
            var r = radio
            s = s + r.toFixed(1)
        }

        return s;
    }


    Rectangle{
        border.color: "brown"

        //poids visu par contours
        color: "#00000000"    // ARGB fully transparent
       // opacity: 0.8*root.dist_weight + 0.2;
        border.width: -9*root.dist_weight+10


        radius: width/2
        anchors.centerIn: parent
        width: parent.dist*2
        height: parent.dist*2


    }


    Label {
   //     rotation: 360-mapControls.rotationAction

        function getR_z(){
            var o = 360
            if(!mapControls.mirror){
                if(mapControls.rotationAction == 90 || mapControls.rotationAction == 270 ){
                     o = 180
                }
            }
            console.log("gerRZ " + o + " " + mapControls.rotationAction)
            return o - mapControls.rotationAction
        }

        visible: showInfo
        anchors.right: root.left
        anchors.bottom: root.top
        property real rot_angle_z: getR_z()
        property real rot_angle_y: mapControls.mirror ? 180:0
        property real rot_angle_x: -180


        id:info_txt
        text: formatId()//"rot : x:"+rot_angle_x +" y:"+rot_angle_y+" z:"+rot_angle_z//formatId()
        padding:5
        background: Rectangle{
            border.color: "black"

        }
        transform: [Rotation { origin.x: info_txt.width/2; origin.y: info_txt.height/2 ; axis { x: 0; y: 0; z: 1 } angle: info_txt.rot_angle_z}
            ,Rotation{ origin.x: info_txt.width/2; origin.y: info_txt.height/2 ; axis { x: 0; y: 1; z: 0 } angle: info_txt.rot_angle_y }
            ,Rotation{ origin.x: info_txt.width/2; origin.y: info_txt.height/2 ; axis { x: 1; y: 0; z: 0 } angle: info_txt.rot_angle_x }]

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

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
import iidre.uwb.anchor 1.0
import QtQuick.Window 2.11

RowLayout{
    id:zone_map
    property bool showGraph: false
    MapControls{
        //Layout.maximumWidth: 60
        id:mapControls
        onZoomAdjustSetChanged: {
            root.zoomActiv = false
            zoomAdjustSet = false;
            root.zoom = root.factor
            map.mouse_translate_X = 0
            map.mouse_translate_Y = 0
            map.setScale(Qt.point(0,0),root.factor)
            zoneMapMouse.mouseX_lastMove = 0
            zoneMapMouse.mouseY_lastMove = 0

        }


        onWinGraphShowChanged: zone_map.showGraph =winGraphShow

    }
    WinGraph{
        id:windows_graph
        visible: zone_map.showGraph// mapControls.winGraphShow
        title: qsTr("RTLS by IIDRE - anchors' data")
        // flags: Qt.Window
        //  visibility: Window.Windowed
        // on: console.log("quit")

    }
    WinGraphMobile{
        id:windows_graph_mobile
        visible: zone_map.showGraph// mapControls.winGraphShow
    }

    Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        color:"white"

         Rectangle{
            border{color: "grey";width:5}
            anchors.centerIn: parent;
            height: parent.height*0.95
            width:  parent.width*0.95
            Rectangle{
                id :root

                Rectangle{
                    id :rot_orig
                    x:parent.width/2
                    y:parent.height/2
                    height: 0
                    width: 0
                    z:100
                    color: "transparent"

                }

                transform: [Rotation { origin.x: rot_orig.x; origin.y: rot_orig.y; axis { x: 0; y: 0; z: 1 } angle: mapControls.rotationAction }
                    ,Rotation{ origin.x: rot_orig.x; origin.y: rot_orig.y; axis { x: 0; y: 1; z: 0 } angle: mapControls.mirror ? 180:0 }
                    ,Rotation{ origin.x: rot_orig.x; origin.y: rot_orig.y; axis { x: 1; y: 0; z: 0 } angle:  180 }]

                //rotation: mapControls.rotationAction
                property point pMapMax: controler_anchor_collection.pMax
                property point pMapMin: controler_anchor_collection.pMin

                property point pMapRef:Qt.point(0,0)
                property point pZoneRef:Qt.point(0,0)


                property real  factor: 1.0
                property real zoom: 0
                property bool zoomActiv: false

                anchors.centerIn: parent;
                height: parent.height*0.95
                width:  parent.width*0.95

                onPMapMaxChanged: updateScale()
                onPMapMinChanged: updateScale()
                onWidthChanged: updateScale()
                onHeightChanged: updateScale()

                Image {
                    property real rot: mapControls.rotationAction
                    onRotChanged: {
                        var t = height
                        height = width
                        width = t
                    }

                    id: mapImg
                    anchors.centerIn: parent
                    height: parent.height*0.99
                    width:  parent.width*0.99
                    source: "image://ImgMap/"+controler_file_manager.lastFile
                    onSourceChanged: console.log("image source change "+source)
                }


                onRotationChanged: {
                    console.log(rotation)
                }


                function updateScale(){

                    if(zoomActiv) return;

                    console.log("img:"+root.width+";"+root.height+" updateScale max X:"+pMapMax.x+" max Y:"+pMapMax.y+" min X"+pMapMin.x+" min Y:"+pMapMin.y+" "   )

                    var fx = Math.abs((pMapMax.x-pMapMin.x)/root.width)
                    var fy = Math.abs((pMapMax.y-pMapMin.y)/root.height)

                    factor = Math.max(fx,fy)

                    if(factor===0){
                        factor =1;
                    }
                    else{
                        factor = 1/factor
                    }


                    zoom = factor

                    pMapRef.x = ((pMapMax.x-pMapMin.x)/2)+pMapMin.x  //(Math.abs(pMapMax.x) - Math.abs(pMapMin.x))/2
                    pMapRef.y = ((pMapMax.y-pMapMin.y)/2)+pMapMin.y

                    pZoneRef.x = (root.width/2)
                    pZoneRef.y = (root.height/2)


                }

                MouseArea{

                    property bool moveMap: false
                    property real mouseX_origin: 0
                    property real mouseY_origin: 0

                    property real mouseX_lastMove: 0
                    property real mouseY_lastMove: 0

                    id:zoneMapMouse
                    anchors.fill:parent
                    hoverEnabled: mapControls.coordMouseSet
                    onWheel:{

                        if(root.pMapMax === root.pMapMin) return ;

                        root.zoomActiv = true
                        var x = (root.pMapMax.x - root.pMapMin.x)/2
                        var y = (root.pMapMax.y - root.pMapMin.y)/2

                        var z = root.zoom +  (wheel.angleDelta.y / 2000)

                        if(z>0)  root.zoom = z

                        console.log("set zoom factor : "+root.factor)
                        map.setScale()
                    }

                    onPressed: {

                        cursorShape=Qt.ClosedHandCursor
                        mouseX_origin = mouseX
                        mouseY_origin = mouseY
                        moveMap = true
                    }

                    onReleased: {
                        moveMap = false
                        cursorShape=Qt.ArrowCursor
                        mouseX_lastMove += mouseX_origin-mouseX
                        mouseY_lastMove += mouseY_origin-mouseY

                        mouseX_origin = 0
                        mouseY_origin = 0
                    }

                    onMouseXChanged: {
                        if(moveMap){
                            map.mouse_translate_X = mouseX_origin-mouseX+mouseX_lastMove
                            map.mouse_translate_Y = mouseY_origin-mouse.y+mouseY_lastMove
                        }
                    }


                    Rectangle{

                        id:cursorCood
                        width: 1
                        height: width
                        color: "transparent"
                        x:parent.mouseX
                        y:parent.mouseY

                        ToolTip {

                            parent: parent.handle
                            visible: zoneMapMouse.containsMouse
                            text:  qsTr("("+Math.ceil(map.getXmap(parent.x))
                                        +","
                                        +Math.ceil(map.getYmap(parent.y))+")")


                        }
                    }


                }

                Map{
                    id:map
                    pMapRef: root.pMapRef
                    pZoneRef: root.pZoneRef
                    zoom: root.zoom



                    Repeater{
                        id :anchorFromDevice
                        model:model_anchorFromDevice

                        delegate: Anchor{

                            pos :  Qt.point(model.X,model.Y)
                            dist: mapControls.distanceSet?model.Dist*root.zoom:0
                            id_anchor: model.UID
                            radio: model.Radio
                            scale : 1/root.zoom
                            showInfo: mapControls.infoSet
                            dist_weight: model.Weight
                            size:10
                        }
                    }

                    DialogAnchorControl{
                        id:dialogAnchorControl
                        visible: false;
                        maxHeight: root.height
                        maxWidht: root.width
                        onValidaded: {
                            controler_anchor_collection.update_anchor(idx,uid,description,type)
                            if(type === AnchorType.ANCHOR){
                                controler_device.setPosAnchor(uid,pos.x,pos.y,0)
                            }
                        }

                    }


                    Repeater{
                        id :anchorFromFile
                        model:model_anchorFromFile

                        delegate: Anchor{
                            idx:index
                            description: model.Descrip
                            color: "green"
                            id_anchor: model.UID

                            pos : Qt.point(model.X,model.Y)
                            scale : 1/root.zoom


                            onPosChanged: console.log("map pos:("+model.UID+")"+model.X+" "+model.Y)
                            onClicked: {
                                console.log("anchor "+uid+"config pos:"+pos.x+" "+pos.y+" type "+Type)
                                dialogAnchorControl.openDialogAnchor(index,uid,pos,Descrip,Type,pos)
                            }
                            showInfo: mapControls.infoSet
                        }
                    }

                    Mobile{
                        pos: Qt.point(controler_device.coord.x,controler_device.coord.y)
                        time: controler_device.time
                        color: "transparent"
                        border.color: "red"
                        id_mobile : controler_device.device_ID
                        border.width: 2
                        scale : 1/root.zoom
                        gain:controler_settings.filterValue*100
//                        size:0
                    }

                    Mobile{
                        pos: Qt.point(controler_device.coord.x,controler_device.coord.y)
                        time: controler_device.time
                        color: "transparent"
                        border.color: "blue"
                        id_mobile : controler_device.device_ID
                        border.width: 2
                        scale : 1/root.zoom
                        visible: controler_settings.duoPoint
                        gain:0
                    }

                    Repeater{
                        id :deviceFromAnchor
                        model:model_deviceFromAnchor

                        delegate: Mobile{
                            pos: Qt.point(model.X,model.Y)
                            time: controler_device.time
                            color: "transparent"
                            border.color: "red"
                            border.width: 2
                            id_mobile : model.UID
                            scale : 1/root.zoom
                            gain:controler_settings.filterValue*100
                            size:10
                        }

                    }
                }
            }
        }
    }
}

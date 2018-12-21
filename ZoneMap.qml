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
            map.setScale(Qt.point(0,0),root.factor)
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
            id :root
            rotation: mapControls.rotationAction
            property point pMapMax: controler_anchor_collection.pMax
            property point pMapMin: controler_anchor_collection.pMin

            property point pMapRef:Qt.point(0,0)
            property point pZoneRef:Qt.point(0,0)


            property real  factor: 1.0
            property real zoom: 0
            property bool zoomActiv: false

            border{color: "grey";width:5}
            anchors.centerIn: parent;
            height: parent.height*0.9
            width:  parent.width*0.9
            onPMapMaxChanged: updateScale()
            onPMapMinChanged: updateScale()
            onWidthChanged: updateScale()
            onHeightChanged: updateScale()

            Image {
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

                console.log("img:"+root.width+","+root.height+" updateScale max X:"+pMapMax.x+" max Y:"+pMapMax.y+" min X"+pMapMin.x+" min Y:"+pMapMin.y+" "   )

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

                pMapRef.x = (Math.abs(pMapMax.x) - Math.abs(pMapMin.x))/2
                pMapRef.y = (Math.abs(pMapMax.y) - Math.abs(pMapMin.y))/2

                pZoneRef.x = (root.width/2)
                pZoneRef.y = (root.height/2)


            }

            MouseArea{

                id:zoneMapMouse
                anchors.fill:parent
                hoverEnabled: mapControls.coordMouseSet
                onWheel:{

                    if(root.pMapMax === root.pMapMin) return ;

                    root.zoomActiv = true
                    var x = (root.pMapMax.x - root.pMapMin.x)/2
                    var y = (root.pMapMax.y - root.pMapMin.y)/2

                    var z = root.zoom +  (wheel.angleDelta.y / 4000)

                    if(z>0)  root.zoom = z

                    console.log("set zoom factor : "+root.factor)
                    map.setScale()
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
                    color: "transparent"
                    border.color: "red"
                    border.width: 2
                    scale : 1/root.zoom
                }


                Repeater{
                    id :deviceFromAnchor
                    model:model_deviceFromAnchor

                    delegate: Mobile{
                        pos: Qt.point(model.X,model.Y)
                        color: "transparent"
                        border.color: "green"
                        border.width: 2
                        scale : 1/root.zoom
                    }

                }
            }
        }
    }
}

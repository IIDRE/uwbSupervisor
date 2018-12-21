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

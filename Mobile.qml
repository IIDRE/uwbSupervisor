import QtQuick 2.0

Rectangle {
    property point pos: Qt.point(0,0)
    property real size : 5
    property string info: ""
    radius: width/2
    color: "chocolate"
    height: size
    width:  size

    x:(pos.x-(width/2))
    y:(pos.y-(height/2))


}

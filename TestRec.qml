import QtQuick 2.0

Rectangle{
    property point pos: Qt.point(0,0)
    transformOrigin: Item.Center
    color:"blue"
    width: 10
    height: width

    x:pos.x-width/2
    y:pos.y-height/2
}

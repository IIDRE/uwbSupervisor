import QtQuick 2.0

Rectangle {
    id:windows
    width:  400
    height: 400
    border.color: "black"
    border.width: 5

    MouseArea{
        anchors.fill:parent
        hoverEnabled:true
        onMouseXChanged: info.text = "mouse :" + mouseX + " ; " + mouseX
        onClicked:{
            map.angle = map.angle +10
            map.angle = map.angle %360
        }
    }

    Rectangle{
        property real angle: 0
        id:map
        anchors.centerIn: parent
        width:   300
        height:   300
        color: "blue"

        Text {
            id: info
            anchors.centerIn: parent
        }
        transform: Rotation { origin.x: map.width/2; origin.y: map.height/2; axis { x: 0; y: 0; z: 1 } angle: map.angle}

        Rectangle{
            id : a1
            width: 10
            height: 20
            x : 10
            y : 10
            color : "red"
            transform: Rotation { origin.x: a1.width/2; origin.y: a1.height/2; axis { x: 0; y: 0; z: 1 } angle: 360- map.angle}

        }

    }


}

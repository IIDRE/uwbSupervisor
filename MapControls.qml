import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0



ColumnLayout{
    Layout.maximumWidth: 30
    property bool infoSet: false
    MapControlButton{
        bp_state: infoSet
        onBp_stateChanged: infoSet=bp_state;
        icon_source: "images/icon_info_50.png"
        info:"Show items UID and coordinates"
    }

    property bool coordMouseSet: false
    MapControlButton{
        bp_state: coordMouseSet
        onBp_stateChanged: coordMouseSet=bp_state;
        icon_source: "images/icon_coord_50.png"
        info:"Show mouse cursor coordinates"
    }

    property bool distanceSet: false
    MapControlButton{
        bp_state: distanceSet
        onBp_stateChanged: distanceSet=bp_state;
        icon_source: "images/icon_distance_50.png"
        info:"Trace multilateration circles"
    }

    property bool zoomAdjustSet: false
    MapControlButton{
        bp_state: zoomAdjustSet
        onBp_stateChanged: {
            zoomAdjustSet=bp_state;
            bp_state = false
        }
        icon_source: "images/icon_zoom_50.png"
        info:"Adjust map"
    }

    property real rotationAction: 0
    MapControlButton{
        bp_state: false
        onBp_stateChanged: {

            if(bp_state){
                rotationAction += 90;
                rotationAction %= 360;
            }

            bp_state = false
        }
        icon_source: "images/icon_rotation_50.png"
        info:"Rotate map"
    }

    property bool winGraphShow: false
    MapControlButton{
        bp_state: winGraphShow
        onBp_stateChanged: winGraphShow=bp_state;
        icon_source: "images/icon_graph_50.png"
        info:"Trace graphs and trajectory"
    }

    Rectangle{
  //      color:"transparent"
        gradient: Gradient {
            GradientStop { position: 1.2; color:"#e44f3f"}
            GradientStop { position: 0 ;color: "#ffffff"}
        }
        Layout.fillHeight: true
        Layout.fillWidth: true
    }


}

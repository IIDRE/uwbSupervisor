import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

Window {
    id:root
    width: 1080
    height: 720
    Flickable {
        anchors.fill: parent
        contentHeight: graphs.itemHeight*graphs.count +20
        contentWidth: root.width
        Column{
            Repeater{
                id:graphs
                property real itemHeight: 300

                model:model_anchorPersistant
                delegate: Rectangle{
                    height: graphs.itemHeight
                    width: root.width
                    border.color: "black"
                    GraphDist{
                        title: model.UID
                        dist:model.Dist
                        radio:model.Radio
                    }
                }
            }
        }

    }
}

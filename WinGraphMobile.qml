import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtCharts 2.2

Window {
    id:root
    title: qsTr("RTLS by IIDRE - mobile's trajectory")
    width: 1080
    height: 720

    property real refreshTime_ms: 100
    property real timeMaxInWindows_ms: 30*1000
    property real gain : 2000

    Item{
        id:lowPassFilter
        property point lastPos: Qt.point(0,0)
        property real last_t: 0

        function fp(t,_pos){

            var _y=0.0;
            var _x=0.0;

            if(last_t > 0){
                var dt = t- last_t
                var g = dt/(root.gain + dt)
                _y = lastPos.y + g*(_pos.y - lastPos.y)
                _x = lastPos.x + g*(_pos.x - lastPos.x)
            }else{
                _y = _pos.y;
                _x = _pos.x;
            }
            lastPos=Qt.point(_x,_y)
            last_t = t

            return lastPos
        }
    }

    Timer {
        property real sessionCount: 0
        property variant series_array: []
        interval: root.refreshTime_ms;
        running: root.visible;
        onRunningChanged: {
            if(!root.visible){
                series.removeAllSeries();
            }

        }

        repeat: true
        onTriggered: {
            var s = series.createSeries(ChartView.SeriesTypeScatter,"",axeX,axeY)

            var p = lowPassFilter.fp(controler_device.time,Qt.point(controler_device.coord.x,controler_device.coord.y))


            s.append(p.x,p.y)
            series_array.push(s)

            if(series_array.length > root.timeMaxInWindows_ms/root.refreshTime_ms){
                s = series_array.shift();
                axeX.min = s.at(0).x
                axeY.min = s.at(0).y
                series.removeSeries(s);
            }

            var i;
            var minX,maxX,minY,maxY
            var stepColor = 1/series_array.length
            s = series_array[0];
            minX = maxX = s.at(0).x;
            minY = maxY = s.at(0).y;

            s.color = Qt.rgba(1,0,0,stepColor);
            for(i=1;i<series_array.length-1;i++){

                s = series_array[i];
                s.color = Qt.rgba(1,0,0,stepColor*i);
                minY = Math.min(s.at(0).y,minY)
                maxY = Math.max(s.at(0).y,maxY)

                minX = Math.min(s.at(0).x,minX)
                maxX = Math.max(s.at(0).x,maxX)
            }

            var GminX = minX - 20;
            var GmaxX = maxX + 20

            var GminY = minY - 20;
            var GmaxY = maxY + 20

            if(!series.isZoomed()){
                axeX.min=GminX
                axeX.max=GmaxX

                axeY.min=GminY
                axeY.max=GmaxY
            }


        }
    }



    ChartView {

        MouseArea{

            id:zoneMapMouse
            anchors.fill:parent
            hoverEnabled: mapControls.coordMouseSet
            onWheel:{
                if(wheel.angleDelta.y > 0)
                    series.zoomIn()
                else
                    series.zoomOut()

            }
            onClicked: series.zoomReset();
        }

        legend.visible:false

        id:series
        //title: "last positions"
        anchors.fill: parent
        antialiasing: true



        ValueAxis{
            id:axeX
            titleText: "X-axis (cm)"
        }


        ValueAxis{
            id:axeY
            titleText: "Y-axis (cm)"
        }
    }
}

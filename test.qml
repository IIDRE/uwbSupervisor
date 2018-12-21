import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtCharts 2.2

Window {
    id:root
    width: 1080
    height: 720

    property real  _Y: 0
    property real  _X: 0
    property real refreshTime_ms: 100
    property real timeMaxInWindows_ms: 10*1000
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
            root._X++;
            root._Y++;

 //posMobile.append(controler_device.coord.x,controler_device.coord.y)
            var s = series.createSeries(ChartView.SeriesTypeScatter,"",axeX,axeY)

            s.append(root._X,root._Y)
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
            axeX.min=minX
            axeX.max=maxX

            axeY.min=minY
            axeY.max=maxY
        }
    }



    ChartView {

        legend.visible:false

        id:series
        title: "last positions"
        anchors.fill: parent
        antialiasing: true



        ValueAxis{
            id:axeX
            onMinChanged: console.log("xmin "+min)
            titleText: "dist (cm)"
        }


        ValueAxis{
            id:axeY
            onMinChanged: console.log("ymin "+min)
            titleText: "dist (cm)"
        }
    }
}

import QtQuick 2.9
import QtCharts 2.2

ChartView {
    property real dist: 0
    property real radio: 0
    property real time_0: 0
    property real  time_limit: 30*1000

    function getSec(){
        return /*Math.ceil*/(new Date().getTime());
    }
    function toMsecsSinceEpoch(date) {
        var msecs = date.getTime();
        return msecs;
    }

    function addToList(serie , value){

        if(serie.count === 0){
            time_0 = getSec();
        }

      //  var i={'data':value,'time':getSec()}
        serie.append(toMsecsSinceEpoch(new Date()),value)
    }

    function removeFirstValue(serie){
        var count = serie.count
        var p0 = serie.at(0)
        var p1 = serie.at(count-1)

        //console.log("remove :"+p0+" "+p1)

        if(p1.x - p0.x > time_limit){
            serie.remove(0)
        }       
    }

    function getLimit(serie){
        var min = serie.at(0).y;
        var max = serie.at(0).y;
        var avg = serie.at(0).y;
        var count = serie.count

        for(var i=1;i<count;i++){

            min = Math.min(min,serie.at(i).y)
            max = Math.max(max,serie.at(i).y)
            avg = avg + serie.at(i).y;
        }


        serie.axisX.max = new Date(serie.at(count-1).x)
        serie.axisX.min = new Date(serie.axisX.max - time_limit)
        serie.avg = avg/i;

        serie.y_values_min = min
        serie.y_values_max = max
    }

    function updateList(serie, value){
        addToList(serie,value)
        removeFirstValue(serie)
        getLimit(serie)
    }

    onRadioChanged: {
        updateList(serieRadio,radio/1000)
    }

    onDistChanged: {
        updateList(serieDist,dist)
    }


    title: "Line"
    anchors.fill: parent
    antialiasing: true


    DateTimeAxis {
        id: axisX1
        format:"hh:mm:ss:zzz"
        tickCount: 1
       //  titleText: "time (s)"

    }

    LineSeries {
        property real avg: 0
        property real y_values_min: 0
        property real y_values_max: 0


        axisY:  ValueAxis{
            min:serieDist.y_values_min
            max:serieDist.y_values_max
            titleText: "dist (cm)"
        }
        axisX: axisX1
        id:serieDist
        name: "distance avg:"+Math.ceil(avg)
    }

    LineSeries {
        id:serieRadio
        property real y_values_min: 0
        property real y_values_max: 0
        property real avg: 0

        axisYRight:  ValueAxis{
            titleText: "pwr (dBm)"
            min:serieRadio.y_values_min
            max:serieRadio.y_values_max
        }

        axisX: axisX1
        name: "radio avg:"+Math.ceil(avg)

    }

}

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0


GridLayout{

    property string uid: ""
    property real dist: 0
    property real radio: 0
    property var listDist: []
    property var listRadio: []
    property var  limitRadio: {'min':0,'max':0,'avg':0}
    property var  limitDist: {'min':0,'max':0,'avg':0}

    function addToList(list , value){
        var i={'data':value,'time':new Date().getTime()/1000}
        list.push(i)
    }

    function removeFirstValue(list){
        if(list[list.length-1].time - list[0].time > 10)
            list.shift()
    }

    function getLimit(list){
        var min = list[0].data;
        var max = list[0].data;
        var avg = list[0].data;

        for(var i=1;i<list.length;i++){

            min = Math.min(min,list[i].data)
            max = Math.max(max,list[i].data)
            avg = avg + list[i].data;
        }
        avg = avg/i;

        var ret={'min':min,'max':max,'avg':avg}

        return ret
    }

    function updateList(list, value){
        addToList(list,value)
        removeFirstValue(list)
        return getLimit(list)
    }

    onRadioChanged: {
        limitRadio = updateList(listRadio,radio)
        //console.log("limit :"+UID+" "+listRadio.length+" "+limit.min+" "+limit.max)
    }

    onDistChanged: {
        limitDist = updateList(listDist,dist)
        //console.log("limit :"+UID+" "+listDist.length+" "+limit.min+" "+limit.max)
    }


    columns:  4
    Layout.fillWidth:  true
    Layout.fillHeight:   true
    Label{
        Layout.columnSpan: 4
        text:qsTr(UID)
    }
    Label{
        text:qsTr("dDist : "+Math.ceil((limitDist.max-limitDist.min)))
    }
    Label{
      //  text:qsTr("avgDist : "+Math.ceil(limitDist.avg))
          text:qsTr("Dist : "+dist/100)
    }
    Label{
        text:qsTr("dRadio : "+Math.ceil((limitRadio.max-limitRadio.min)/1000))
    }
    Label{
        //text:qsTr("avgRadio : "+Math.ceil(limitRadio.avg/1000))
        text:qsTr("Radio : "+Math.ceil(radio/1000))
    }
}


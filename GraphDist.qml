/*Copyright Iidre SAS 01 janvier 2019 tech@iidre.com

Ce logiciel est un programme informatique servant à superviser et configurer le réseau UWB du kit Iidre.

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILLtelle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.*/
import QtQuick 2.9
import QtCharts 2.2
import QtQuick.Controls 1.4

ChartView {
    Row{
        Button{
            text:"reset dist axis"
            onClicked: serieDist.updateAxis=true
        }
        Button{
            text:"reset radio axis"
            onClicked: serieRadio.updateAxis=true
        }
    }
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

        if(serie.updateAxis){
            serie.updateAxis = false;false
            serie.y_values_min = min
            serie.y_values_max = max
        }else{
            serie.y_values_min = Math.min(serie.y_values_min,min);
            serie.y_values_max = Math.max(serie.y_values_max,max);
        }


    }

    function updateList(serie, value){
        addToList(serie,value)
        removeFirstValue(serie)
        getLimit(serie)
    }

    onRadioChanged: {
        var r = radio;
        if(r != 0){
            if(r > 30)
                r = 30;
            console.log("radio "+r);
           updateList(serieRadio,r)
        }
    }

    onDistChanged: {
        if(dist > 0)
            updateList(serieDist,dist)
    }


    title: "Line"
    anchors.fill: parent
    antialiasing: true


    DateTimeAxis {
        id: axisX1
        format:"hh:mm:ss"
        tickCount: 1
        //  titleText: "time (s)"

    }

    LineSeries {
        id:serieDist
        property real avg: 0
        property real y_values_min: 0
        property real y_values_max: 0
        property bool updateAxis: true

        axisY:  ValueAxis{
            min:serieDist.y_values_min
            max:serieDist.y_values_max

            titleText: "<font color='"+labelsColor+"'>dist (cm)</font>"
            labelsColor: serieDist.color
        }
        axisX: axisX1

        name: "distance avg:"+Math.ceil(avg)
    }

    LineSeries {
        id:serieRadio
        property real y_values_min: 0
        property real y_values_max: 6
        property real avg: 0
        property bool updateAxis: true

        axisYRight:  ValueAxis{
            titleText: "<font color='"+labelsColor+"'>idiff</font>"//"idiff"//"pwr (dBm)"
            labelsColor: serieRadio.color
            min:serieRadio.y_values_min
            max:serieRadio.y_values_max
        }

        axisX: axisX1
        name: "radio avg:"+Math.ceil(avg)

    }

}


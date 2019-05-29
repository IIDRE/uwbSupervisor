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
    property int cpt: 0
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
        cpt++
        //console.log("limit :"+UID+" "+listDist.length+" "+limit.min+" "+limit.max)
    }


    columns:  4
    Layout.fillWidth:  true
    Layout.fillHeight:   true
    property int w_info: 100
    Label{
        Layout.columnSpan: 2
        text:qsTr(UID)
    }
    Label{
        Layout.columnSpan: 2
        text:qsTr("cpt : "+cpt)
    }
    Label{
        Layout.minimumWidth: w_info
        text:qsTr("dDist : "+Math.ceil((limitDist.max-limitDist.min)))
    }
    Label{

      //  text:qsTr("avgDist : "+Math.ceil(limitDist.avg))
        Layout.minimumWidth: w_info
        text:qsTr("Dist : "+dist/100)
    }
    Label{

        Layout.minimumWidth: w_info
        text:qsTr("dWeight : "+Math.ceil((limitRadio.max-limitRadio.min)*1000))
    }
    Label{
        Layout.minimumWidth: w_info
        //text:qsTr("avgRadio : "+Math.ceil(limitRadio.avg/1000))
        text:qsTr("Weight : "+Math.ceil(radio*1000))


    }
}


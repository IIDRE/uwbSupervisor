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
import QtQuick 2.0

Rectangle {
    id:root
    property point pos: Qt.point(0,0)
    property int  time : 0
    property real size : 5
    property string info: ""
    property real gain : 0.1

    Item{
        id:lowPassFilter_Y
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

    radius: width/2
    color: "chocolate"
    height: size
    width:  size

    x:(pos.x-(width/2))
    y:(pos.y-(height/2))

    onTimeChanged: {
        console.log("time changed :"+time)
        var p = lowPassFilter_Y.fp(time,pos);
        root.x =p.x
        root.y=p.y
    }


}

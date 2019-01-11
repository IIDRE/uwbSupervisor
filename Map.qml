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

Item {
    id:root
    property real zoom: 1
    property point pMapRef:Qt.point(0,0)
    property point pZoneRef:Qt.point(0,0)


    function getXmap(_x){
        return (_x/zoom) - _translate.x
    }

    function getYmap(_y){
        return (_y/zoom) - _translate.y
    }

    function getCoordMap(p){
        return Qt.point(
                     getXmap(p.x)
                    ,getYmap(p.y)
                    );
    }

    onZChanged: setScale()
    onPMapRefChanged: setScale()
    onPZoneRefChanged: setScale()


    function setScale(){

        _translate.x = (pZoneRef.x/zoom)-pMapRef.x
        _translate.y = (pZoneRef.y/zoom)-pMapRef.y

        console.log("set zoom  zoom "+zoom+" to "+_translate.x+";"+_translate.y)

        _scale.xScale = zoom
        _scale.yScale = zoom
    }



    transform: [
        Translate {id:_translate ; x:0; y:0}
        ,Rotation {id:rot}
        ,Scale {id:_scale}
    ]


    width: 0
    height: 0
    x:0
    y:0
}

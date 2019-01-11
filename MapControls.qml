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

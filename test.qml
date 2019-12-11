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
    property var listDist: ["2007", "2008", "2009", "2010", "2011", "2012"]
    Row{
        Button{
            text:"reset dist axis"
            onClicked: {
                console.log("reset dist0")
                serieValue.append(20)
                listDist.push("2020")
                serieX.categories = listDist
            }
        }
        Button{
            text:"reset radio axis"
            onClicked: {
                console.log("reset radio0")

            }
        }
    }
    property real dist: 0
    property real radio: 0


    onRadioChanged: {
    }

    onDistChanged: {
    }

    title: "Bar series"
    anchors.fill: parent

    antialiasing: true

    BarSeries {
        id: mySeries
        axisX: BarCategoryAxis {id:serieX; categories: listDist }
        BarSet {id:serieValue; label: "Bob"; values: [2, 2, 3, 4, 5, 6] }

    }
}



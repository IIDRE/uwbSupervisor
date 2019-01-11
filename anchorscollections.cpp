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
#include "anchorscollections.h"
#include <QQmlEngine>
#include <QString>



AnchorsList &AnchorsCollections::getAnchorFromFile()
{
    return AnchorFromFile;
}


void AnchorsCollections::clearSessionAnchor()
{
    QList<long> uids = sessionAnchor.keys();
    for(auto u:uids){
        if(!sessionAnchor[u]){
            AnchorFromDevice.removeAnchor(u);
        }
        sessionAnchor[u] = false;
    }
}

AnchorsCollections::AnchorsCollections(const QString &nameObj, QObject *parent)
    : Qml_object(nameObj,parent)
{


    connect(&tClearSession,&QTimer::timeout,this,&AnchorsCollections::timeOutSession);
    tClearSession.setSingleShot(false);
    tClearSession.setInterval(1000);
    tClearSession.start();


    allAnchor.insert("anchorFromDevice",&AnchorFromDevice);
    allAnchor.insert("anchorFromFile",&AnchorFromFile);
    allAnchor.insert("anchorPersistant",&AnchorPersistant);


    for(auto a:allAnchor){
        connect(a,&AnchorsList::LimitUpdated,this,&AnchorsCollections::LimitUpdated);
    }

    DeviceFromAnchor.isAnchor = false;
    allAnchor.insert("deviceFromAnchor",&DeviceFromAnchor);
}

void AnchorsCollections::LimitUpdated()
{

    QPoint pMax,pMin;


    bool init = false;
    for(auto a:allAnchor){

        if(!a->isAnchor) continue;

        if(a->limitIsValid()){
            if(!init){
                pMax = a->getPMax();
                pMin = a->getPMin();
                init =  true;
            }else{
                pMin.rx() = qMin(pMin.x(),a->getPMin().x());
                pMin.ry() = qMin(pMin.y(),a->getPMin().y());

                pMax.rx() = qMax(pMax.x(),a->getPMax().x());
                pMax.ry() = qMax(pMax.y(),a->getPMax().y());
            }
        }
    }


    setPMax(pMax);
    setPMin(pMin);

}

void AnchorsCollections::timeOutSession()
{
    clearSessionAnchor();
}



void AnchorsCollections::update_anchor(qint32 idx, QString uid, QString description, int type)
{
    bool bok;
    long _uid = uid.toLong(&bok,16);

    if(bok)
        AnchorFromFile.setValue(idx,_uid,static_cast<Anchor_type::TYPE>(type),description);
}




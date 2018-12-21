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




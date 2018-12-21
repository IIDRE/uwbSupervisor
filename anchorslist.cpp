#include "anchorslist.h"




QPoint AnchorsList::getPMax() const
{
    return pMax;
}

QPoint AnchorsList::getPMin() const
{
    return pMin;
}

AnchorsList::AnchorsList():Qml_AbstracList(this)
{
    addRole("UID",&AnchorsList::getUID);
    addRole("X",&AnchorsList::getX);
    addRole("Y",&AnchorsList::getY);
    addRole("Z",&AnchorsList::getZ);
    addRole("Radio",&AnchorsList::getRadio);
    addRole("Dist",&AnchorsList::getDist);
    addRole("Descrip",&AnchorsList::getDescrip);
    addRole("Type",&AnchorsList::getType);

    isAnchor = true;

}

void AnchorsList::removeAnchor(long UID)
{

    for(int i=0 ; i< list.length();i++){
        if(list[i].UID == UID){
            qDebug()<<Q_FUNC_INFO<<QString("remove: %1").arg(UID,0,16);
            remove(i);
            break;
        }
    }


    if(list.isEmpty()) {
        emit LimitUpdated();
        return;
    }

    QPoint pMin(list[0].X,list[0].Y);
    QPoint pMax = pMin;

    for(int i=1 ; i< list.length();i++){
        pMin.rx() = qMin(pMin.x(),list[i].X);
        pMin.ry() = qMin(pMin.y(),list[i].Y);

        pMax.rx() = qMax(pMax.x(),list[i].X);
        pMax.ry() = qMax(pMax.y(),list[i].Y);
    }

    this->pMax = pMax;
    this->pMin = pMin;
    emit LimitUpdated();

}



void AnchorsList::UpdateAnchor(const anchor_data &data)
{
    int i = list.indexOf(data);

    if(i<0){ //add
      //  qDebug()<<Q_FUNC_INFO<<QString("add : %1 %2 %3").arg(data.UID,0,16).arg(data.X).arg(data.Y);
        addData(data);
    }else{//update
       // qDebug()<<Q_FUNC_INFO<<" update :"<<QString::number(data.UID,16);
        list[i] = data;
        update(i);
    }


    QPoint pMax(this->pMax),pMin(this->pMin);
    bool bUpdateLimit = false;
    if (list.count() > 1){
        pMax.rx() = qMax(pMax.x(),data.X);
        pMax.ry() = qMax(pMax.y(),data.Y);

        pMin.rx() = qMin(pMin.x(),data.X);
        pMin.ry() = qMin(pMin.y(),data.Y);

        bUpdateLimit = (pMax != this->pMax || pMin != this->pMin);
    }
    else{
        pMax.setX(data.X);
        pMax.setY(data.Y);

        pMin.setX(data.X);
        pMin.setY(data.Y);

        bUpdateLimit = true;
    }

    if(bUpdateLimit){
        this->pMax = pMax;
        this->pMin = pMin;
        emit LimitUpdated();
    }


}

void AnchorsList::UpdateAnchor(long UID, int dist, int X, int Y, int Z, int radio, Anchor_type::TYPE anchorType, QString descrip)
{
    UpdateAnchor(anchor_data(UID,dist,X,Y,Z,radio,anchorType,descrip));
}

void AnchorsList::setValue(int idx,long UID, Anchor_type::TYPE anchorType, QString descrip)
{
    if(idx >= list.length()) return;

    list[idx].UID = UID;
    list[idx].descrip = descrip;
    list[idx].anchorType = anchorType;
    update(idx);
}



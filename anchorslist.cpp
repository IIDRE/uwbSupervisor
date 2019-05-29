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
    addRole("Weight",&AnchorsList::getWeight);

    isAnchor = true;

}

void AnchorsList::clearAnchorData(unsigned long UID){
    for(int i=0 ; i< list.length();i++){
        if(list[i].UID == UID){
            qDebug()<<Q_FUNC_INFO<<QString("clear: %1").arg(UID,0,16);
            list[i].dist=0;
            list[i].radio=0;
            break;
        }
    }
}

void AnchorsList::removeAnchor(unsigned long UID)
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

void AnchorsList::UpdateAnchor(unsigned long UID, int dist, int X, int Y, int Z, int radio,float weight, Anchor_type::TYPE anchorType, QString descrip)
{
    UpdateAnchor(anchor_data(UID,dist,X,Y,Z,radio,weight,anchorType,descrip));
}

void AnchorsList::setValue(int idx,long UID, Anchor_type::TYPE anchorType, QString descrip)
{
    if(idx >= list.length()) return;

    list[idx].UID = UID;
    list[idx].descrip = descrip;
    list[idx].anchorType = anchorType;
    update(idx);
}



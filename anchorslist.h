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
#ifndef ANCHORSLIST_H
#define ANCHORSLIST_H

#include "qml_object.h"
#include <QHash>

class Anchor_type:public QObject{
    Q_OBJECT

public:
    enum TYPE{
        ANCHOR///< is a real anchor
        ,POI///< is a point of interes of map
        ,UNKNOW
    };Q_ENUM(TYPE)

    static void register_to_qml(){
        qmlRegisterType<Anchor_type>("iidre.uwb.anchor",1,0,"AnchorType");
    }
};

struct anchor_data{
    unsigned long UID;
    int X,Y,Z;
    int radio;
    float weight;
    int dist;
    Anchor_type::TYPE anchorType;
    QString descrip;


    anchor_data(unsigned long UID=0, int dist=0, int X=0, int Y=0, int Z=0, int radio=0,float weight =0
            ,Anchor_type::TYPE anchorType = Anchor_type::UNKNOW,QString descrip=QString()):
        UID(UID)
      ,X(X),Y(Y),Z(Z)
      ,radio(radio)
      ,weight(weight)
      ,dist(dist)
      ,anchorType(anchorType)
      ,descrip(descrip)
    {}


    bool operator==(const anchor_data &A){
        return (UID == A.UID);
    }

    anchor_data & operator=(const anchor_data &A){
        if(this != &A){
            X = A.X;
            Y = A.Y;
            Z = A.Z;
            radio = A.radio;
            weight = A.weight;
            dist = A.dist;
            descrip = A.descrip;
            anchorType = A.anchorType;
        }
        return *this;
    }

};


CLASS_QML_ABSTRACT_LIST(AnchorsList,anchor_data){

    Q_OBJECT
    QVariant getUID(const anchor_data data)const{
        return QString::number(data.UID,16);
    }
    QVariant getX(const anchor_data data)const{
        return data.X;
    }

    QVariant getY(const anchor_data data)const{
        return data.Y;
    }

    QVariant getZ(const anchor_data data)const{
        return data.Z;
    }

    QVariant getRadio(const anchor_data data)const{
        return data.radio;
    }

    QVariant getWeight(const anchor_data data)const{
        return data.weight;
    }

    QVariant getDist(const anchor_data data)const{
        return data.dist;
    }
    QVariant getDescrip(const anchor_data data)const{
        return data.descrip;
    }
    QVariant getType(const anchor_data data)const{
        return data.anchorType;
    }

    QPoint pMax,pMin;


public:
    bool isAnchor;
    AnchorsList();

    ~AnchorsList(){}
    void removeAnchor(unsigned long UID);
    void clearAnchorData(unsigned long UID);
    void UpdateAnchor(const anchor_data &data);
    void UpdateAnchor(unsigned long UID, int dist, int X, int Y, int Z, int radio
                        , float weight, Anchor_type::TYPE anchorType = Anchor_type::ANCHOR, QString descrip=QString());

    void setValue(int idx,long UID,Anchor_type::TYPE anchorType = Anchor_type::ANCHOR,QString descrip=QString() );

    bool limitIsValid(){return ! list.isEmpty();}
    // Qml_AbstracList interface
    QPoint getPMax() const;

    const LIST &getList()const {return list;}
    QPoint getPMin() const;
    void clear(){removeAll();}
protected:
    bool dataIsValid(const anchor_data data) const{
        return (data.UID !=0);
    }
signals:
    void LimitUpdated();
};

#endif // ANCHORSLIST_H

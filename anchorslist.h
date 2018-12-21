#ifndef ANCHORSLIST_H
#define ANCHORSLIST_H

#include "qml_object.h"
#include <QHash>

class Anchor_type:public QObject{
    Q_OBJECT

public:
    enum TYPE{
        ANCHOR,///< is a real anchor
        POI///< is a point of interes of map
    };Q_ENUM(TYPE)

    static void register_to_qml(){
        qmlRegisterType<Anchor_type>("iidre.uwb.anchor",1,0,"AnchorType");
    }
};

struct anchor_data{
    long UID;
    int X,Y,Z;
    int radio;
    int dist;
    Anchor_type::TYPE anchorType;
    QString descrip;


    anchor_data(long UID=0, int dist=0, int X=0, int Y=0, int Z=0, int radio=0
            ,Anchor_type::TYPE anchorType = Anchor_type::ANCHOR,QString descrip=QString()):
        UID(UID)
      ,X(X),Y(Y),Z(Z)
      ,radio(radio)
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
    void removeAnchor(long UID);
    void UpdateAnchor(const anchor_data &data);
    void UpdateAnchor(long UID,int dist,int X,int Y,int Z,int radio
                        ,Anchor_type::TYPE anchorType = Anchor_type::ANCHOR,QString descrip=QString());

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

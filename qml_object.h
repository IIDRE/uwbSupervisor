#ifndef QML_INTERFACE_H
#define QML_INTERFACE_H

#include <QObject>
#include <QtQml>
#include <QQmlContext>
#include <QHash>
#include <QAbstractListModel>

/**
 * @brief The Qml_Object class
 * add a c++ objet to Qml context
 */
class Qml_object:public QObject{
    Q_OBJECT
    static const QString qml_object_prefix; ///<< definit le prefix de l'objet appelé par QML
protected:
    const QString nameObj;
public:
    Qml_object(const QString &nameObj, QObject *parent = nullptr);
    virtual ~Qml_object(){}
    void registerToQml(QQmlContext *ctx);
    virtual void registerChildrenToQml(QQmlContext *ctx){Q_UNUSED(ctx)}
    virtual void registerModelToQml(QQmlContext *ctx, const QString nameObj, QObject *obj);
    const QString &getName(){return nameObj;}
    static const QString formatName(const QString &name);

};


class Qml_objectClollection:private QHash<QString,Qml_object*>{
    QQmlContext *ctx;
public:
    Qml_objectClollection(QQmlContext *ctx):ctx(ctx){}
    ~Qml_objectClollection(){}

    template<class QML_OBJ>
    QML_OBJ *add(QML_OBJ *obj)
    {
        if(!obj) return nullptr;

        insert(obj->getName(),obj);

        obj->registerToQml(ctx);

        return obj;
    }



};


#define CLASS_QML_ABSTRACT_LIST(_name,_type) class _name : public Qml_AbstracList<_name,_type>

template <class child , class T>
class Qml_AbstracList : public QAbstractListModel{
    child *children;
protected:
    typedef T typeData;
    typedef QList<T> LIST;
    typedef QVariant (child::* getter)(const T) const;
    LIST list;
    QHash<int, QByteArray> roles;
    QList<getter> getterList;

    virtual bool dataIsValid(const T data)const  =0;

    void addRole(const QString &roleName,getter cb){
        int idx = getterList.length();
        roles.insert(idx+Qt::UserRole + 1,roleName.toLocal8Bit());
        getterList.append(cb);
    }

    void addData(T data){
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        list.append(data);
        endInsertRows();
    }

    void updateAll(){
        QModelIndex topLeft = index(0, 0);
        QModelIndex bottomRight = index(rowCount()-1, 0);
        if(topLeft.isValid() && bottomRight.isValid()){
            emit dataChanged(topLeft,bottomRight);
        }
    }

    void update(int idx){
        if(idx >= rowCount()) return;
        QModelIndex topLeft = index(idx, 0);
        QModelIndex bottomRight = index(idx, 0);
        if(topLeft.isValid() && bottomRight.isValid()){
            emit dataChanged(topLeft,bottomRight);
        }
    }

    void remove(int idx){
         if(idx >= rowCount()) return;
        beginRemoveRows(QModelIndex(),idx,idx);
        list.removeAt(idx);
        endRemoveRows();
    }


    void removeAll(){
        beginRemoveRows(QModelIndex(),0,rowCount()-1);
        list.clear();
        endRemoveRows();
    }


    int rowCount(const QModelIndex &parent= QModelIndex()) const{
        Q_UNUSED(parent);
        return list.length();
    }

    QVariant data(const QModelIndex &index, int role) const{
        if ((!index.isValid()) || (index.row() < 0) || (index.row() >= (this->rowCount())))
        {
            return QVariant();
        }


        if( role <= Qt::UserRole )  return QVariant();

        role -= (Qt::UserRole+1) ;


        auto dataFromList = list[index.row()];

        if(role >= getterList.length()) return QVariant();

        if(!dataIsValid(dataFromList)) return QVariant();


        return (children->*getterList[role])(dataFromList);

    }
    QHash<int, QByteArray> roleNames() const{
        return roles;
    }

public:
    Qml_AbstracList(child *children):children(children){}
    T getItem(int idx) const{
        if (idx < list.length()){
            return list[idx];
        }else return list[0];
    }
};

#endif // QML_INTERFACE_H

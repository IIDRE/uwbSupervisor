#include "qml_object.h"


const QString Qml_object::qml_object_prefix = "controler_";


Qml_object::Qml_object(const QString &nameObj, QObject *parent)
    :QObject(parent),nameObj(qml_object_prefix+nameObj)
{

}

void Qml_object::registerToQml(QQmlContext *ctx)
{
    ctx->setContextProperty(nameObj,this);
    registerChildrenToQml(ctx);
}

void Qml_object::registerModelToQml(QQmlContext *ctx, const QString nameObj, QObject *obj)
{
    ctx->setContextProperty("model_"+nameObj,obj);
}

const QString Qml_object::formatName(const QString &name)
{
    return qml_object_prefix+name;
}




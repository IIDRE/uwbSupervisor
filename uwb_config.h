#ifndef UWB_CONFIG_H
#define UWB_CONFIG_H

#include <QList>
#include <QQmlContext>
#include <QString>
#include "qml_object.h"
#include <QHash>

class uwb_config_item : public QObject{
    Q_OBJECT
    QList<qint32> list;

    Q_PROPERTY(int idx READ idx WRITE setIdx NOTIFY idxChanged)

    int m_idx;

public:
    uwb_config_item(QQmlContext *ctx,const QString name , std::initializer_list<qint32> args);
    virtual ~uwb_config_item(){}
    void setCurrentValue(qint32 value);

    int idx() const
    {
        return m_idx;
    }

    qint32 getCurrentValue(){
        return list[m_idx];
    }
public slots:
    void setIdx(int idx)
    {
        if (m_idx == idx)
            return;

        m_idx = idx;
        emit idxChanged(m_idx);
    }
signals:
    void idxChanged(int idx);
};

class uwb_config : public Qml_object
{
    Q_OBJECT
    Q_PROPERTY(bool forceConfigure READ forceConfigure WRITE setForceConfigure NOTIFY forceConfigureChanged)
    QQmlContext *ctx;
public:
    Q_INVOKABLE void valueChange(QString nameOfValue , int idx);

    QHash<QString,uwb_config_item*> setOfItems;

    uwb_config(const QString &nameObj, QQmlContext *ctx);
    void resetValue();
public:
    QString formatConfigCmd();
    bool forceConfigure() const
    {
        return m_forceConfigure;
    }

public slots:
    void setForceConfigure(bool forceConfigure)
    {
        if (m_forceConfigure == forceConfigure)
            return;

        m_forceConfigure = forceConfigure;
        emit forceConfigureChanged(m_forceConfigure);
    }

signals:
    void forceConfigureChanged(bool forceConfigure);

private:
    bool m_forceConfigure;
};

#endif // UWB_CONFIG_H

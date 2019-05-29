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
        if(m_idx <0 || m_idx>list.length()) return 0;

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

    void valueChangeByValue(QString nameOfValue, int value);

    void initTRxCode(int CHAN , int PRF );
    void initPlen(int BR);
    void initPac(int Plen);
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

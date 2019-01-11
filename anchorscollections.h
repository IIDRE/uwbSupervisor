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
#ifndef ANCHORSCOLLECTIONS_H
#define ANCHORSCOLLECTIONS_H

#include <QObject>

#include "device.h"
#include "qml_object.h"
#include <QHash>

#include "anchorslist.h"



class AnchorsCollections : public Qml_object
{
    Q_OBJECT


    QTimer tClearSession;
    AnchorsList AnchorFromDevice;
    AnchorsList AnchorFromFile;
    AnchorsList DeviceFromAnchor;

    AnchorsList AnchorPersistant;
    QHash<long,qint64> anchorPersistanTime;

    QHash<long,bool> sessionAnchor;

    QHash<QString,AnchorsList*> allAnchor;

    Q_PROPERTY(QPointF pMax READ pMax WRITE setPMax NOTIFY pMaxChanged)
    Q_PROPERTY(QPointF pMin READ pMin WRITE setPMin NOTIFY pMinChanged)

    QPointF m_pMax;
    QPointF m_pMin;

    void clearSessionAnchor();
public:
    explicit AnchorsCollections(const QString &nameObj , QObject *parent = nullptr);

signals:

    void pMaxChanged(QPointF pMax);

    void pMinChanged(QPointF pMin);

public slots:

    void onPosDeviceFromAnchorInComming(long UID,int X,int Y,int Z){
        DeviceFromAnchor.UpdateAnchor(UID,0,X,Y,Z,0);
    }

    void onDistInComming(long UID,int dist,int X,int Y,int Z,int radio){
        tClearSession.start();

        if(sessionAnchor[UID])
            clearSessionAnchor();

        sessionAnchor[UID]=true;
        AnchorFromDevice.UpdateAnchor(UID,dist,X,Y,Z,radio);

        qint64 t = QDateTime::currentMSecsSinceEpoch();
        if(t - anchorPersistanTime[UID] >= 300 ){
            anchorPersistanTime[UID]  =t;
            AnchorPersistant.UpdateAnchor(UID,dist,X,Y,Z,radio);
        }

    }

    void setPMax(QPointF pMax)
    {
        if (m_pMax == pMax)
            return;

        m_pMax = pMax;
        emit pMaxChanged(m_pMax);
    }

    void setPMin(QPointF pMin)
    {
        if (m_pMin == pMin)
            return;

        m_pMin = pMin;
        emit pMinChanged(m_pMin);
    }

private slots:
    void LimitUpdated();
    void timeOutSession();
    // Qml_object interface
public:
    Q_INVOKABLE void update_anchor(qint32 idx ,QString uid , QString description , int type);
    void registerChildrenToQml(QQmlContext *ctx){
        for(auto k : allAnchor.keys()){
            Qml_object::registerModelToQml(ctx,k,allAnchor[k]);
        }
        Anchor_type::register_to_qml();

    }
    QPointF pMax() const
    {
        return m_pMax;
    }
    QPointF pMin() const
    {
        return m_pMin;
    }
    AnchorsList &getAnchorFromFile();
};

#endif // ANCHORSCOLLECTIONS_H

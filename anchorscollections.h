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

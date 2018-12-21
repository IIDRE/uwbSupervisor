#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QHash>
#include <QTimer>
#include "atcommand.h"
#include "qml_object.h"
#include <QPointF>
#include <QStringListModel>
#include "uwb_config.h"



struct anchor{
    long time;
    int X,Y,Z;
    long ID;
};


class device : public Qml_object
{
public:
    typedef QHash<long,anchor> HASH_OF_ANCHOR;
    Q_PROPERTY(QPointF coord READ coord WRITE setCoord NOTIFY coordChanged)
    Q_PROPERTY(QStringList listOfID MEMBER m_listOfID NOTIFY listOfID_changed)

private:
    Q_OBJECT

    QTimer checkDevice;


    typedef bool (device::* cmd_from_device)(AtCommand &);

    QHash<QByteArray,cmd_from_device> MapOdDeviceRsp;
    HASH_OF_ANCHOR anchorPos;
    QStringList m_listOfID;

    int configListIdx;
    QList<QByteArray> configList;

    QString configDeviceCommand;

    QString version;
    long deviceID;


    int indexParseMsg;
    QByteArray allDataFromDevice;
    void resetAllData(){
        indexParseMsg=0;
        allDataFromDevice.clear();
    }



    bool rsp_device_ok(AtCommand &at);
    bool rsp_device_error(AtCommand &at);

    bool rsp_device_DIST(AtCommand &at);
    bool rsp_device_MPOS(AtCommand &at);
    bool rsp_device_ID(AtCommand &at);
    bool rsp_device_VER(AtCommand &at);
    bool rsp_device_POS(AtCommand &at);
    bool rsp_device_DPOS(AtCommand &at);
    bool rsp_device_CFG(AtCommand &at);
    bool rsp_device_TRACE(AtCommand &at);

    template<typename F>
    void addRsp(const QString str,F cb){
        MapOdDeviceRsp.insert(str.toUpper().toLatin1(),cb);
    }

    void sendSameConfig(){
        if(configListIdx < configList.length()){
            emit sendCommand(configList[configListIdx]);
            checkDevice.start();
        }else checkDevice.stop();
    }

    void sendNextConfig(){
        if(configListIdx < configList.length()-1){
            configListIdx++;
            emit sendCommand(configList[configListIdx]);
            checkDevice.start();
        }else checkDevice.stop();
    }
    QPointF m_coord;


public:
    explicit device(const QString &nameObQPointfj ,QObject *parent = nullptr);

    QHash<long, anchor> getAnchorPos() const;
    Q_INVOKABLE void setPosAnchor(QString UID,int X,int Y,int Z);
    Q_INVOKABLE void configDevice();

    void setPosAnchor(long UID,int X,int Y,int Z);
    void setPosAnchor(anchor &pos);

    QPointF coord() const
    {
        return m_coord;
    }

signals:
    void sendCommand(QByteArray cmd);
    //+DIST:10000B7A,285,40,-493,0,-83282
    void DistInComming(long UID,int dist,int X,int Y,int Z,int radio);
    void PosInComming(int X,int Y,int Z);
    void coordChanged(QPointF coord);
    void PosDeviceFromAnchor(long UID,int X,int Y,int Z);
    void listOfID_changed();

public slots:
    void onIncommingData(QByteArray incomingMessage);
    void onCnxStatusChanged(bool cnx);
    void onSendConfigCmd();
    void setCoord(QPointF coord)
    {
        if (m_coord == coord)
            return;

        m_coord = coord;
        emit coordChanged(m_coord);
    }

    // Qml_object interface

public:
    uwb_config *uwbConfig;


    // Qml_object interface
public:
    void registerChildrenToQml(QQmlContext *ctx){
        uwbConfig = new uwb_config("uwb_config",ctx);

    }
    void send_config();
};

#endif // DEVICE_H

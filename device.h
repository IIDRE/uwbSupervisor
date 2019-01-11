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

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
#include "device.h"
#include "setting.h"


QHash<unsigned long, anchor> device::getAnchorPos() const
{
    return anchorPos;
}

void device::setPosAnchor(QString UID, int X, int Y, int Z)
{
    bool bok;
    unsigned long _uid = UID.toULong(&bok,16);
    if(!bok || !_uid ) return;

    setPosAnchor(_uid,X,Y,Z);
    //qDebug() <<Q_FUNC_INFO<< m_listOfID.indexOf(QRegExp(UID, Qt::CaseInsensitive, QRegExp::W3CXmlSchema11), 0)<<UID;

}

void device::configDevice()
{
    qDebug()<<Q_FUNC_INFO;
    send_config();
    sendNextConfig();

}

void device::setPosAnchor(unsigned long UID, int X, int Y, int Z)
{
    QByteArray cmd = QString("AT+POS=%1,%2,%3,%4\r\n")
            .arg(UID,0,16).arg(X).arg(Y).arg(Z)
            .toLatin1();


    configList.append(cmd);

    if(!configPosInProgress){
        configPosInProgress = true;
        sendNextConfig();
    }
    emit busy(configPosInProgress);

}

void device::setPosAnchor(anchor &pos)
{
    setPosAnchor(pos.ID,pos.X,pos.Y,pos.Z);
}

bool device::rsp_device_ok(AtCommand &at)
{
    Q_UNUSED(at)
    qDebug()<<Q_FUNC_INFO;

    if(configPosInProgress){
        nbConfigPos++;
    }

    sendNextConfig();



    return true;
}

bool device::rsp_device_error(AtCommand &at)
{
    Q_UNUSED(at)
    qDebug()<<Q_FUNC_INFO;

   // sendSameConfig();
    sendNextConfig();
    return true;
}

//+DIST:[time],10000B7A,285,40,-493,0,-83282
bool device::rsp_device_DIST(AtCommand &at)
{
    anchor Anchor;
    int dist,radio;
    bool bok;
    float weight;

    enum{
        DIST_TIME=0,
        DIST_ID,
        DIST_DIST,
        DIST_X,
        DIST_Y,
        DIST_Z,
        DIST_RADIO,
        DIST_IDIFF,
        DIST_MC,
        DIST_LEN
    };

    if(at.numParams()>=DIST_LEN){
        Anchor.time = at.getParam(DIST_TIME).toLong(&bok);
        if(!bok) return false;

        Anchor.ID = at.getParam(DIST_ID).toULong(&bok,16);
        if(!bok) return false;

        dist = at.getParam(DIST_DIST).toInt(&bok);
        if(!bok) return false;

        Anchor.X = at.getParam(DIST_X).toInt(&bok);
        if(!bok) return false;

        Anchor.Y = at.getParam(DIST_Y).toInt(&bok);
        if(!bok) return false;

        Anchor.Z = at.getParam(DIST_Z).toInt(&bok);
        if(!bok) return false;

        radio= at.getParam(DIST_IDIFF).toInt(&bok);
        if(!bok) return false;

        float prNLOS=0.0;

        if (radio <= wparams.i1)
            prNLOS = 0;
        else if (radio < wparams.i2)
            prNLOS = (wparams.alpha)*radio + wparams.beta;
        else if(radio < 1000)
            prNLOS = wparams.prNlos_valueMax;
        else
            prNLOS = 1;


        weight = 1- prNLOS;
        qDebug()<<QString("dist : radio %1 W:%2").arg(radio).arg(weight);
        if(!anchorPos.contains(Anchor.ID)){
            m_listOfID.append(QString::number(Anchor.ID,16));
            m_listOfID.sort(Qt::CaseInsensitive);
            emit listOfID_changed();
        }

        anchorPos.insert(Anchor.ID,Anchor);
        emit DistInComming(Anchor.ID
                           ,dist
                           ,Anchor.X,Anchor.Y,Anchor.Z
                           ,radio,weight);
        return true;
    }
    return false;
}


bool device::rsp_device_MPOS(AtCommand &at)
{

    enum{
        MPOS_TIME=0
        ,MPOS_X
        ,MPOS_Y
        ,MPOS_Z
        ,MPOS_LEN
    };

    int X,Y,Z;
    long time;
    bool bok;

    if(at.numParams()>=MPOS_LEN){

        time= at.getParam(MPOS_TIME).toLong(&bok);
        if(!bok) return false;


        X = at.getParam(MPOS_X).toInt(&bok);
        if(!bok) return false;

        Y = at.getParam(MPOS_Y).toInt(&bok);
        if(!bok) return false;

        Z = at.getParam(MPOS_Z).toInt(&bok);
        if(!bok) return false;

        emit PosInComming( X,Y,Z,time);

        setCoord(QPointF(X,Y));
        setTime(time);
        return true;
    }
    return false;
}

bool device::rsp_device_ID(AtCommand &at)
{
    if(at.numParams() < 1) return false;

    QString id = at.getParam(0);
    setDevice_ID(id);

    qDebug()<<Q_FUNC_INFO<<"device id "<<id;
    return true;
}

bool device::rsp_device_VER(AtCommand &at)
{
    if(at.numParams()<= 0) return true;
    version = QString("%1 , %2 , %3").arg(at.getParam(0),at.getParam(1),at.getParam(2));
    qDebug()<<Q_FUNC_INFO<<"version : "<<version;
    return true;
}

bool device::rsp_device_POS(AtCommand &at)
{
    anchor Anchor;
    bool bok;

    if(at.numParams()>=4){

        //  Anchor.ID =static_cast<long>(0x00000000ffffffff & at.getParam(0).toLongLong(&bok,16));
        Anchor.ID =at.getParam(0).toULong(&bok,16);
        if(!bok)return false;


        Anchor.X = at.getParam(1).toInt(&bok);
        if(!bok) return false;


        Anchor.Y = at.getParam(2).toInt(&bok);
        if(!bok) return false;


        Anchor.Z = at.getParam(3).toInt(&bok);
        if(!bok) return false;


        if(!anchorPos.contains(Anchor.ID)){
            m_listOfID.append(QString::number(Anchor.ID,16));
            m_listOfID.sort(Qt::CaseInsensitive);
            emit listOfID_changed();
        }
        anchorPos.insert(Anchor.ID,Anchor);
        qDebug()<<Q_FUNC_INFO<<"add :"<<Anchor.ID;
        return true;
    }
    return false;
}

bool device::rsp_device_DPOS(AtCommand &at)
{
    enum{
        DPOS_TIME,
        DPOS_UID,
        DPOS_X,
        DPOS_Y,
        DPOS_Z,
        DPOS_ANCHOR_UID,
        DPOS_ANCHOR_X,
        DPOS_ANCHOR_Y,
        DPOS_ANCHOR_Z,
        DPOS_ANCHOR_DIST,
        DPOS_ANCHOR_FPWR,
        DPOS_ANCHOR_RXpwr,
        nb_DPOS
    };

    int X,Y,Z;
    long time;
    bool bok;
    ulong UID;
    anchor Anchor;
    int dist,radio,weight;

    static int test=-20;
    if(at.numParams()>=nb_DPOS){

        time= at.getParam(DPOS_TIME).toLong(&bok);
        if(!bok) return false;

        UID= at.getParam(DPOS_UID).toULong(&bok,16);
        if(!bok) return false;


        X = at.getParam(DPOS_X).toInt(&bok);
        if(!bok) return false;


        Y = at.getParam(DPOS_Y).toInt(&bok);
        if(!bok) return false;

        Z = at.getParam(DPOS_Z).toInt(&bok);
        if(!bok) return false;

        emit PosDeviceFromAnchor(UID,X,Y,Z);


        Anchor.time = time;

        Anchor.ID = at.getParam(DPOS_ANCHOR_UID).toULong(&bok,16);
        if(!bok) return false;

        dist = at.getParam(DPOS_ANCHOR_DIST).toInt(&bok);
        if(!bok) return false;

        Anchor.X = at.getParam(DPOS_ANCHOR_X).toInt(&bok);
        if(!bok) return false;

        Anchor.Y = at.getParam(DPOS_ANCHOR_Y).toInt(&bok);
        if(!bok) return false;

        Anchor.Z = at.getParam(DPOS_ANCHOR_Z).toInt(&bok);
        if(!bok) return false;

        radio= at.getParam(DPOS_ANCHOR_RXpwr).toInt(&bok);
        if(!bok) return false;

        weight= at.getParam(DPOS_ANCHOR_FPWR).toInt(&bok);
        if(!bok) return false;


        if(!anchorPos.contains(Anchor.ID)){
            m_listOfID.append(QString::number(Anchor.ID,16));
            emit listOfID_changed();
        }

        anchorPos.insert(Anchor.ID,Anchor);
        emit DistInComming(Anchor.ID
                           ,dist
                           ,Anchor.X,Anchor.Y,Anchor.Z
                           ,radio,weight/1000);

//        emit PosInComming( X,Y,Z,time);
//        setCoord(QPointF(X,Y));
        setTime(time);

        return true;
    }
    return false;

}

bool device::rsp_device_CFG(AtCommand &at)
{
    enum{
        CFG_PARAM_CHAN=0,
        CFG_PARAM_PRF,
        CFG_PARAM_TRCODE,
        CFG_PARAM_BR,
        CFG_PARAM_PLEN,
        CFG_PARAM_PAC,
        CFG_PARAM_PWR,
        NB_CFG_PARAM
    };

    int CHAN,
            TRCODE,
            PRF,
            BR,
            PLEN,
            PAC,
            PWR;

    qDebug()<<Q_FUNC_INFO;
    bool bok;

    if(at.getType() != AtCommand::CommandType::NOTIFICATION)
        return true;

    if(at.numParams()>= NB_CFG_PARAM){
        CHAN = at.getParam(CFG_PARAM_CHAN).toInt(&bok);
        if(!bok) return false;

        TRCODE = at.getParam(CFG_PARAM_TRCODE).toInt(&bok);
        if(!bok) return false;

        PRF = at.getParam(CFG_PARAM_PRF).toInt(&bok);
        if(!bok) return false;

        BR = at.getParam(CFG_PARAM_BR).toInt(&bok);
        if(!bok) return false;

        PLEN = at.getParam(CFG_PARAM_PLEN).toInt(&bok);
        if(!bok) return false;

        PAC = at.getParam(CFG_PARAM_PAC).toInt(&bok);
        if(!bok) return false;

        PWR = at.getParam(CFG_PARAM_PWR).toInt(&bok);
        if(!bok) return false;

        if(!uwbConfig->forceConfigure()){
            uwbConfig->setOfItems["CHAN"]->setCurrentValue(CHAN);
            uwbConfig->setOfItems["PRF"]->setCurrentValue(PRF);
            uwbConfig->initTRxCode(CHAN,PRF);
            uwbConfig->setOfItems["TRXcode"]->setCurrentValue(TRCODE);

            uwbConfig->setOfItems["BR"]->setCurrentValue(BR);
            uwbConfig->initPlen(BR);

            uwbConfig->setOfItems["PLEN"]->setCurrentValue(PLEN);
            uwbConfig->initPac(PLEN);

            uwbConfig->setOfItems["PAC"]->setCurrentValue(PAC);
        }else{
            send_config();
        }
        return true;
    }

    return false;
}


void device::send_config(){
    configDeviceCommand = uwbConfig->formatConfigCmd();


    qDebug()<<Q_FUNC_INFO<<"cfg : "<<configDeviceCommand;
    configList.append(configDeviceCommand.toLatin1());

    qDebug()<<Q_FUNC_INFO<<"cfg : "<<configDeviceCommand.toLatin1();
}


bool device::rsp_device_TRACE(AtCommand &at)
{
    QStringList trace;
    qDebug()<<Q_FUNC_INFO<<at.toData();

    if(at.numParams() <1) return false;
    int nb = at.getParam(0).toInt();

    for(int i=0 ; i<nb ; i++){
        trace.append(at.getParam(1+i));
    }

    //find MPOS
    if(!trace.contains("MPOS",Qt::CaseInsensitive)){
        auto s = QString("AT+TRACE=+MPOS");

        qDebug()<<Q_FUNC_INFO<<"trace : "<<s;
        configList.append(s.toLatin1());
        return true;
    }
    //find DIST

    if(!trace.contains("DIST",Qt::CaseInsensitive)){
        auto s = QString("AT+TRACE=+DIST");

        qDebug()<<Q_FUNC_INFO<<"trace : "<<s;
        configList.append(s.toLatin1());
        return true;
    }

    return true;
}

bool device::rsp_device_WPARAMS(AtCommand &at){
    enum{
        WPARAM_PARAM_PRM=0
        ,WPARAM_PARAM_i1
        ,WPARAM_PARAM_i2
        ,WPARAM_PARAM_NB
    };

    int min,i1,i2;
    qDebug()<<Q_FUNC_INFO;
    bool bok;
    if(at.numParams() < WPARAM_PARAM_NB) return false;


    min = at.getParam(WPARAM_PARAM_PRM).toInt(&bok);
    if(!bok) return false;

    i1 = at.getParam(WPARAM_PARAM_i1).toInt(&bok);
    if(!bok) return false;

    i2 = at.getParam(WPARAM_PARAM_i2).toInt(&bok);
    if(!bok) return false;


    wparams.i1 = i1;
    wparams.i2 = i2;

    wparams.prNlos_valueMax = 1-min/100.0;

    wparams.alpha = (wparams.prNlos_valueMax) /
                            (wparams.i2 - wparams.i1);
    wparams.beta = - wparams.alpha * wparams.i1;



    return true;
}


device::device(const QString &nameObj, QObject *parent) : Qml_object (nameObj,parent)
{



    configListIdx=0;
    checkDevice.setInterval(5000);
    checkDevice.setSingleShot(false);
    connect(&checkDevice,&QTimer::timeout,this,&device::onSendConfigCmd);

    configList.append("AT+VER?\r\n");
    configList.append("AT+ID?\r\n");
    configList.append("AT+POS?\r\n");
    configList.append("AT+CFG?\r\n");
    configList.append("AT+WPARAMS?\r\n");
    //configList.append("AT+TRACE?\r\n");

    indexParseMsg =0;

    addRsp("OK",&device::rsp_device_ok);
    addRsp("ERROR",&device::rsp_device_error);


    addRsp(Setting::getInstance()->getDistHeader(),&device::rsp_device_DIST);
   // addRsp(Setting::getInstance()->getPosHeader(),&device::rsp_device_MPOS);
     addRsp("MPOS",&device::rsp_device_MPOS);
   // addRsp("MPOS_WLS",&device::rsp_device_MPOS);
     addRsp("ID",&device::rsp_device_ID);
    addRsp("VER",&device::rsp_device_VER);
    addRsp("POS",&device::rsp_device_POS);
    addRsp("CFG",&device::rsp_device_CFG);
    addRsp("TRACE",&device::rsp_device_TRACE);
    addRsp("DPOS",&device::rsp_device_DPOS);
    addRsp("WPARAMS",&device::rsp_device_WPARAMS);

}


void device::onIncommingData(const QByteArray &incomingMessage){


    allDataFromDevice += incomingMessage;

    qWarning()<<Q_FUNC_INFO<<" "<<allDataFromDevice<<"("<<incomingMessage<<") idx:"<<indexParseMsg;

    if(configListIdx >= configList.length())
        checkDevice.start();

    if(!(incomingMessage.endsWith("\r\n") || incomingMessage.endsWith("\n\r") ))
        return;


    AtCommand atc;

    while(atc.parse(allDataFromDevice,indexParseMsg) == true)
    {
        const QByteArray cmd = atc.getCommand().toUpper().toLatin1();
        qDebug()<<Q_FUNC_INFO<<cmd;

        auto cb = MapOdDeviceRsp[cmd];

        if(cb) (this->*cb)(atc);
        else qWarning()<<"serport : Command  "<<atc.getCommand()<<" unknow";

    }
    resetAllData();
}

void device::onCnxStatusChanged(bool cnx)
{
    qDebug()<<Q_FUNC_INFO;
    if(cnx){
        configListIdx=0;
        emit sendCommand(configList[configListIdx]);
        checkDevice.start();
    }
    else {
        stopSendCmd();
        if(!uwbConfig->forceConfigure())
            uwbConfig->resetValue();
    }
}

void device::onSendConfigCmd()
{
    resetAllData();
    if(configListIdx<configList.length())
        emit sendCommand(configList[configListIdx]);
}


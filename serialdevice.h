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
#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include "qml_object.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVariant>
#include <QTimer>
#include <QThread>

#include "setting.h"

class baudrateList : public Qml_AbstracList<baudrateList,qint32>{

    Q_OBJECT

public:
    QVariant getBaud(const typeData baud)const {return baud;}
    baudrateList():Qml_AbstracList(this){

        addRole("baudrate", &baudrateList::getBaud);
        for(auto i :QSerialPortInfo::standardBaudRates()){
            if(i < 115200 ) continue;
            if(i >460800) break;

            addData(i);
        }

    }
    int getIdx(qint32 baud){
        return list.indexOf(baud);
    }
    ~baudrateList(){}

    // Qml_AbstracList interface
protected:
    bool dataIsValid(const typeData data)const {return (data > 0);}
};


struct serialPortInfo_extra : public QSerialPortInfo{
    QString extra;

    serialPortInfo_extra(const QString& extra):QSerialPortInfo(),extra(extra){}

    serialPortInfo_extra(const QSerialPortInfo& cpy)
        :QSerialPortInfo(cpy){}
    serialPortInfo_extra():QSerialPortInfo(){}

    bool serialIsNull()const{
        return isNull() && extra.isNull();
    }

};

class PortAvaibleList : public Qml_AbstracList<PortAvaibleList,serialPortInfo_extra>{

    Q_OBJECT

    QTimer watchSerial;

public:
    QVariant getInfo(const typeData sInfo)const{
       if(!sInfo.isNull())
            return QString("%1 (%2)").arg(sInfo.portName(),sInfo.description());

       return sInfo.extra;
    }

    int getNbPort(){
        return list.count();
    }

    PortAvaibleList():Qml_AbstracList(this){
        watchSerial.setInterval(1000);
        watchSerial.setSingleShot(false);

        connect(&watchSerial,&QTimer::timeout,this,&PortAvaibleList::onUpdateList);

        addRole("info",&PortAvaibleList::getInfo);

        for(auto i :QSerialPortInfo::availablePorts()){
            qDebug()<<i.portName();
            qDebug()<<i.description();
            qDebug()<<i.manufacturer();
            qDebug()<<i.serialNumber();
            qDebug()<<i.systemLocation();

            addData(i);
        }
        addData(serialPortInfo_extra("file"));
        watchSerial.start();

    }

    ~PortAvaibleList(){}

    // Qml_AbstracList interface
protected:
    bool dataIsValid(const serialPortInfo_extra data) const{return !data.serialIsNull();}
private slots:
    void onUpdateList(){


        auto l = QSerialPortInfo::availablePorts();

     //   if(l.length() == list.length()) return;


        beginResetModel();
        list.clear();

        for(auto i :l){
            list.append(i);
        }


        if(!Setting::getInstance()->fileSimulation().isEmpty())
            list.append((serialPortInfo_extra("Open file")));

        endResetModel();

    }

};


class serialDevice:public Qml_object
{
    Q_OBJECT
    QSerialPort serialPort;
    baudrateList BaudrateList;
    PortAvaibleList portAvaibleList;
    QFile file;
    QTextStream *data;
    Q_PROPERTY(bool cnxStatus READ cnxStatus WRITE setCnxStatus NOTIFY cnxStatusChanged)
    Q_PROPERTY(QString cnxInfo READ cnxInfo WRITE setCnxInfo NOTIFY cnxInfoChanged)
    bool m_cnxStatus;

    QString m_cnxInfo;


public:
    Q_INVOKABLE void connectToSerialPort(int idxPort , int idxBaudRate);
    Q_INVOKABLE void diconnectSerialPort(){
        if(! serialPort.isOpen()) return;
        serialPort.close();
        setCnxStatus(false);
    }
    Q_INVOKABLE int getIdxBaudrate(qint32 baud){
        int idx = BaudrateList.getIdx(baud);
        return idx>0?idx:0;
    }

    serialDevice(const QString qmlControlerName);

    // Qml_object interface

    void registerChildrenToQml(QQmlContext *ctx);

    bool cnxStatus() const
    {
        return m_cnxStatus;
    }

    QString cnxInfo() const
    {
        return m_cnxInfo;
    }

private slots:
    void onSerialPortError(QSerialPort::SerialPortError portError);


    void setCnxStatus(bool cnxStatus)
    {
        if (m_cnxStatus == cnxStatus)
            return;

        m_cnxStatus = cnxStatus;
        emit cnxStatusChanged(m_cnxStatus);
    }
    void serPortDataReady(){
        const QByteArray &buffer= serialPort.readAll();
        qDebug()<<Q_FUNC_INFO<<buffer;

        if(data)
            (*data)<<buffer;

        emit incomingData(buffer);
    }

public slots:
    void sendToSerial(QByteArray data){
        serialPort.clear();
        int nbSend = 0;
        int totalToSend = data.length();
        int idx=0;
        qDebug()<<Q_FUNC_INFO<<"send "<<data;
        while(totalToSend > 0){


            nbSend =(totalToSend >= 10)?10:totalToSend;

            serialPort.write(&data.data()[idx],nbSend);
            serialPort.waitForBytesWritten();

            totalToSend-= nbSend;
            idx += nbSend;
        }


    }

    void setCnxInfo(QString cnxInfo)
    {
        if (m_cnxInfo == cnxInfo)
            return;

        m_cnxInfo = cnxInfo;
        emit cnxInfoChanged(m_cnxInfo);
    }

signals:
    void cnxStatusChanged(bool cnxStatus);
    void incomingData(QByteArray);
    void cnxInfoChanged(QString cnxInfo);
};

#endif // SERIALDEVICE_H

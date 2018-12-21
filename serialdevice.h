#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include "qml_object.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVariant>
#include <QTimer>


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


class PortAvaibleList : public Qml_AbstracList<PortAvaibleList,QSerialPortInfo>{

    Q_OBJECT

    QTimer watchSerial;

public:
    QVariant getInfo(const typeData sInfo)const{
        return QString("%1 (%2)").arg(sInfo.portName(),sInfo.description());
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
        watchSerial.start();

    }

    ~PortAvaibleList(){}

    // Qml_AbstracList interface
protected:
    bool dataIsValid(const QSerialPortInfo data) const{return !data.isNull();}
private slots:
    void onUpdateList(){


        auto l = QSerialPortInfo::availablePorts();

     //   if(l.length() == list.length()) return;


        beginResetModel();
        list.clear();

        for(auto i :l){
            list.append(i);
        }
        endResetModel();

    }

};


class serialDevice:public Qml_object
{
    Q_OBJECT
    QSerialPort serialPort;
    baudrateList BaudrateList;
    PortAvaibleList portAvaibleList;

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
        QByteArray buffer;

        buffer = serialPort.readAll();
        qDebug()<<Q_FUNC_INFO<<buffer;
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

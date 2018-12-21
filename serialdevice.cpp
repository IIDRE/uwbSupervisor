#include "serialdevice.h"



void serialDevice::connectToSerialPort(int idxPort, int idxBaudRate)
{
    qDebug()<<Q_FUNC_INFO<<portAvaibleList.getItem(idxPort).systemLocation()<<BaudrateList.getItem(idxBaudRate);

    serialPort.setBaudRate(BaudrateList.getItem(idxBaudRate));
    serialPort.setPortName(portAvaibleList.getItem(idxPort).systemLocation());


    bool bo = serialPort.open(QSerialPort::ReadWrite);

    setCnxStatus(bo);

}

serialDevice::serialDevice(const QString qmlControlerName)
    :Qml_object (qmlControlerName)
{
    connect(&serialPort,&QSerialPort::errorOccurred,this,&serialDevice::onSerialPortError);
    connect(&serialPort,&QSerialPort::readyRead,this,&serialDevice::serPortDataReady);

    setCnxStatus(false);
}

void serialDevice::registerChildrenToQml(QQmlContext *ctx)
{
    Qml_object::registerModelToQml(ctx,"baudrate",&BaudrateList);
    Qml_object::registerModelToQml(ctx,"portAvaible",&portAvaibleList);
}

void serialDevice::onSerialPortError(QSerialPort::SerialPortError portError)
{
   if(portError == QSerialPort::NoError){
       setCnxInfo("");
       return;
    }
    qWarning()<<"SERIAL PORT ERROR [" << serialPort.errorString() << "]. CLOSING";
    setCnxInfo(serialPort.errorString());
    diconnectSerialPort();
}



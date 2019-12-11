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
#include "serialdevice.h"



void serialDevice::connectToSerialPort(int idxPort, int idxBaudRate)
{

    auto const &port = portAvaibleList.getItem(idxPort);

    if(!port.isNull()){
        qDebug()<<Q_FUNC_INFO<<portAvaibleList.getItem(idxPort).systemLocation()<<BaudrateList.getItem(idxBaudRate);

        serialPort.setBaudRate(BaudrateList.getItem(idxBaudRate));
        serialPort.setPortName(portAvaibleList.getItem(idxPort).systemLocation());


        bool bo = serialPort.open(QSerialPort::ReadWrite);

        setCnxStatus(bo);
        delete timerSimulation;
        delete fileSimulation;
        timerSimulation = nullptr;
        fileSimulation = nullptr;
    }else{
        qDebug()<<"open other "<<port.extra<<" file: "<<Setting::getInstance()->fileSimulation();
        fileSimulation = new QFile(Setting::getInstance()->fileSimulation());

        if (!fileSimulation->open(QIODevice::ReadOnly | QIODevice::Text)){
            qWarning()<<"Impossible d'ouvrir le fichier";
            onSerialPortError (QSerialPort::DeviceNotFoundError);
            return;
        }
        timerSimulation = new QTimer(this);
        timerSimulation->setSingleShot(true);
        connect(timerSimulation, &QTimer::timeout, this, &serialDevice::readDataFromFile);
        timerSimulation->start(100);  // Used to configure timer
        setCnxStatus(true);

    }

}

serialDevice::serialDevice(const QString qmlControlerName)
    :Qml_object (qmlControlerName)
{


     QString filename=QString("%1.serial.log").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    file .setFileName(filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)){
        data = new QTextStream(&file);
    }else{
        qWarning()<<"Impossible d'ouvrir le fichier";
        data = nullptr;
    }



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
    qDebug()<<"SERIAL PORT ERROR [" << serialPort.errorString() << "]. CLOSING";
    qCritical()<<serialPort.errorString();
    setCnxInfo(serialPort.errorString());
    diconnectSerialPort();
}



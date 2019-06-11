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
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QQuickView>

#include "img_provider.h"

#include "qml_object.h"
#include "serialdevice.h"
#include "device.h"

#include "file_manager.h"
#include "anchorscollections.h"

#include "message_system.h"
#include "setting.h"
#include "test.h"




int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/logoUWB_small.ico"));




    QQmlApplicationEngine engine;



    Qml_objectClollection qmlCollection(engine.rootContext());


    qmlCollection.add(message_system::getInstance());
    qmlCollection.add(Setting::getInstance());

    //ajout image provider
    img_provider ImgMap;
    engine.addImageProvider("ImgMap",&ImgMap);

    auto Device = qmlCollection.add(new device("device"));
    auto serial_device =  qmlCollection.add(new serialDevice("serial_device"));
    auto anchor_collection = qmlCollection.add(new AnchorsCollections("anchor_collection"));
    auto FileManager = qmlCollection.add(new file_manager("file_manager",*Device,*anchor_collection,ImgMap));


    QObject::connect(serial_device,&serialDevice::incomingData,Device,&device::onIncommingData);
    QObject::connect(Device,&device::sendCommand,serial_device,&serialDevice::sendToSerial);
    QObject::connect(serial_device,&serialDevice::cnxStatusChanged,Device,&device::onCnxStatusChanged);


    QObject::connect(Device,&device::DistInComming,anchor_collection,&AnchorsCollections::onDistInComming);
    QObject::connect(Device,&device::PosDeviceFromAnchor,anchor_collection,&AnchorsCollections::onPosDeviceFromAnchorInComming);


    QObject::connect(serial_device,&serialDevice::cnxStatusChanged,FileManager,&file_manager::onCnxStatusChanged);
    QObject::connect(Device,&device::DistInComming,FileManager,&file_manager::onDistInComming);
    QObject::connect(Device,&device::PosInComming,FileManager,&file_manager::onPosInComming);




    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

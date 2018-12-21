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


    auto Device = qmlCollection.add(new device("device"));

    auto s =  qmlCollection.add(new serialDevice("serial_device"));
    QObject::connect(s,&serialDevice::incomingData,Device,&device::onIncommingData);
    QObject::connect(Device,&device::sendCommand,s,&serialDevice::sendToSerial);
    QObject::connect(s,&serialDevice::cnxStatusChanged,Device,&device::onCnxStatusChanged);

    auto Ac = qmlCollection.add(new AnchorsCollections("anchor_collection"));
    QObject::connect(Device,&device::DistInComming,Ac,&AnchorsCollections::onDistInComming);
    QObject::connect(Device,&device::PosDeviceFromAnchor,Ac,&AnchorsCollections::onPosDeviceFromAnchorInComming);


//ajout image provider
    img_provider ImgMap;
    engine.addImageProvider("ImgMap",&ImgMap);



    auto f = qmlCollection.add(new file_manager("file_manager",*Device,*Ac,ImgMap));
    QObject::connect(s,&serialDevice::cnxStatusChanged,f,&file_manager::onCnxStatusChanged);
    QObject::connect(Device,&device::DistInComming,f,&file_manager::onDistInComming);
    QObject::connect(Device,&device::PosInComming,f,&file_manager::onPosInComming);




    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

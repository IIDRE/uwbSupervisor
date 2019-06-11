#include "message_system.h"

message_system *message_system::instance = nullptr;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
#ifdef QT_DEBUG
        fprintf(stderr, "Debug: %s \n", localMsg.constData());
        break;

#endif
    case QtInfoMsg:
        fprintf(stderr, "Info: %s \n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", localMsg.constData());
        message_system::getInstance()->sendCritalMessage(msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", localMsg.constData());
        break;
    }
}

message_system::message_system():Qml_object ("message_system")
{
    qInstallMessageHandler(myMessageOutput);
}

message_system *message_system::getInstance()
{
    if(!message_system::instance){
        message_system::instance = new message_system();
    }

    return message_system::instance;
}

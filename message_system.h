#ifndef MESSAGE_SYSTEM_H
#define MESSAGE_SYSTEM_H


#include <QtGlobal>
#include <qml_object.h>

class message_system:public Qml_object
{
    Q_OBJECT
    static message_system *instance;
    message_system();

public:
    static message_system *getInstance ();
    ~message_system(){}
    void sendCritalMessage(QString msg){emit critalMessage(msg);}
signals:
    void critalMessage(QString msg);
};

#endif // MESSAGE_SYSTEM_H

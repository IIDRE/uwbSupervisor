#include "test.h"
#include <QDebug>

test::test(QObject *parent) : QObject(parent)
{

}

void test::DistInComming(long UID, int dist, int X, int Y, int Z, int radio)
{
    qDebug()<<Q_FUNC_INFO<<QString("%1 %2 %3 %4 %5 %6")
              .arg(UID).arg(dist).arg(X).arg(Y).arg(Z).arg(radio);
}

void test::PosInComming(int X, int Y, int Z)
{
    qDebug()<<Q_FUNC_INFO<<QString("%1 %2 %3")
              .arg(X).arg(Y).arg(Z);
}

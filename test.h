#ifndef TEST_H
#define TEST_H

#include <QObject>

class test : public QObject
{
    Q_OBJECT
public:
    explicit test(QObject *parent = nullptr);

signals:

public slots:
    void DistInComming(long UID,int dist,int X,int Y,int Z,int radio);
    void PosInComming(int X,int Y,int Z);
};

#endif // TEST_H

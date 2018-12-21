#ifndef MAT_FILE_H
#define MAT_FILE_H

#include <QFile>
#include <QTextStream>
#include <QPoint>
#include <QList>

struct dataAnchor_matFile{
    qint64 UID;
    qint64 coord_x;
    qint64 coord_y;
    qint64 coord_z;
    qint64 dist;
    qint64 rxPwr;

    dataAnchor_matFile(){UID=coord_x=coord_y=coord_z=dist=rxPwr=0;}
    dataAnchor_matFile(qint64 mUID, qint64 mX, qint64 mY, qint64 mZ, qint64 mdist, qint64 mrxPwr){
        UID=mUID;
        coord_x=mX;
        coord_y=mY;
        coord_z=mZ;
        dist=mdist;
        rxPwr=mrxPwr;
    }
};


typedef  QList<dataAnchor_matFile> listAnchorDataMatFile;

class mat_file:public QFile
{
    bool fileOpen;
    QString nameFile;
    QString nameObj;
    int rows;
    QTextStream data;
    qint64 beginTmstp;

    const static int nbAnchorMax=10;
public:
    ~mat_file();
    mat_file(const QString nameFile, const QString name_obj);

    bool addRow(QPoint MPOS, qint64 z, const listAnchorDataMatFile &list);
    bool addRow(qint64 x, qint64 y, qint64 z, const listAnchorDataMatFile &list);
};


#endif // MAT_FILE_H

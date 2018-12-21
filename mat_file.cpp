#include "mat_file.h"
#include <QDateTime>
#include <QtGlobal>

mat_file::~mat_file()
{
    if(!fileOpen) return;

    data << "\n\n";
    data.flush();
    seek(0);
    data << "# Created by UWBsupervisor " << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy \n");
    data << "# name: " << nameObj << "\n";
    data << "# type: matrix \n";
    data << QString("# rows: %1\n").arg(rows,6,10,QChar('0'));
    data << QString("# columns: %1\n").arg(nbAnchorMax*6 + 5 ,3,10,QChar('0'));
}

mat_file::mat_file(const QString name,const QString name_obj=QString()):QFile(name+".mat"),nameFile(name),nameObj(name_obj),data(this)
{
    rows=0;
    beginTmstp = QDateTime::currentMSecsSinceEpoch();

    if(name_obj.size()==0)
        nameObj=nameFile;

    if( !(open(QIODevice::ReadWrite | QIODevice::Text)) ){
        fileOpen=false;
        return;
    }
    fileOpen=true;

    data << "# Created by UWBsupervisor " << QDateTime::currentDateTime().toString("ddd MMM dd hh:mm:ss yyyy \n");
    data << "# name: " << nameObj << "\n";
    data << "# type: matrix \n";
    data << "# rows: xxxxxx\n";
    data << "# columns: xxx\n";
}

bool mat_file::addRow(qint64 x, qint64 y, qint64 z, const listAnchorDataMatFile &list){
    return addRow(QPoint(x,y), z, list);
}

bool mat_file::addRow(QPoint MPOS, qint64 z, const listAnchorDataMatFile &list)
{
    if(!fileOpen) return false;

    data << " " << QDateTime::currentMSecsSinceEpoch()-beginTmstp << " " << MPOS.x() << " " << MPOS.y() << " " << z << " " << list.count() ;
    for(auto i:list){
        data << " " << i.UID << " " << i.coord_x << " " << i.coord_y << " " << i.coord_z << " " << i.dist << " " << i.rxPwr ;
    }
    for(int i=list.count() ; i< nbAnchorMax ; i++){
        data << " 0 0 0 0 0 0";
    }
    data <<"\n";
    rows++;

    return true;
}

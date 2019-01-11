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

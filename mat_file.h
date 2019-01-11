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

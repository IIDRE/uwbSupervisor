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
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "qml_object.h"
#include "device.h"
#include "anchorscollections.h"
#include "mat_file.h"
#include "libdl/dl_creationadapter.h"
#include "img_provider.h"


class file_type{
public:

    typedef enum{
        JSON
        ,DXF
        ,IMG//jpg png
        ,OTHER

    }TYPE;
private:
    QString error;
    TYPE type;
public:
    file_type(QUrl file):type(TYPE::OTHER){
        const QString &fileName=file.fileName();
        const QStringList &lStr = fileName.split('.');

        if(lStr.size()<1) {
            error = "file"+ fileName+" is not formated";
            return;
        }

        const QString &extention = lStr.last();

        auto ext=extention.toLower();

        if(ext == "dxf"){
            type=TYPE::DXF;
        }
        else if(ext == "json"){
            type=TYPE::JSON;
        }
        else if(ext == "jpg" || ext == "png" || ext == "bmp"){
            type=TYPE::IMG;
        }
        else{
            error ="file"+ fileName+" is not formated" ;
            return;
        }

    }
    QString getError() const{return error;}
    //TYPE getType() const {return type;}
    TYPE operator()(void) const{return type;}
};


class file_manager : public Qml_object
{
    Q_OBJECT
    device &Device;
    AnchorsCollections &Anchors;
    img_provider &ImgProvider;



    struct file_manager_matFile{
        listAnchorDataMatFile lAnchor4MatFile;
        mat_file *log;
        QHash<unsigned long,bool>  anchorSession;

        void addAnchor(unsigned long UID,int dist,int X,int Y,int Z,int radio){
            if(anchorSession[UID]){
                //new session
                write(0,0,0);
                lAnchor4MatFile.clear();
                anchorSession.clear();
            }
            anchorSession[UID]=true;
            lAnchor4MatFile.append(dataAnchor_matFile(UID,X,Y,Z,dist,radio));
        }

        file_manager_matFile(){
            log=nullptr;
        }

        QString open(){
            close();
            QString nameLogMatFile("UWB_supervisor_"+QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
            log = new mat_file(nameLogMatFile+QDateTime::currentDateTime().toString("yyyyMMddhhmmss"),"dataFromUWB");
            return nameLogMatFile;
        }
        void close(){
            delete  log;
            log = nullptr;
        }
        void write(int X,int Y, int Z){
            if(log)
                log->addRow(X,Y,Z,lAnchor4MatFile);
        }


    };

    file_manager_matFile logMatFile;

    static const QString JSON_ID;
    static const QString JSON_POS_X;
    static const QString JSON_POS_Y;
    static const QString JSON_POS_Z;

    static const QString JSON_ANCHOR;
    static const QString JSON_MAP;
    static const QString JSON_DESCRIPTION;


    QJsonArray toJson(const device::HASH_OF_ANCHOR &h);


    QString m_lastFile;

    QUrl lastURL;
    void setLastURLS(QUrl url){
        lastURL=url;
        setLastFile(url.fileName());
    }
    QString m_lastEvent;


    struct dxf_reader : public DL_CreationAdapter {
        int uid;
        AnchorsList &anchors;
        int nbImport=0;
        int toCm(double m){
            return static_cast<int>(m*100);
        }
        dxf_reader(QUrl urlfile,file_manager *parent,AnchorsList &anchors);
        void addPoint(const DL_PointData &data);
    };


    bool saveJson(QUrl file, QJsonObject jo);
    bool readJson(QUrl file, QJsonDocument &jsdoc);
    bool m_outputOctave;

    bool openMapDXF(QUrl file);
    bool openMapIMG(QUrl file);
    bool openMapJSON(QUrl file);
public:
    explicit file_manager(QString name, device &Device, AnchorsCollections &Anchors, img_provider &ImgProvider, QObject *parent = nullptr);
    Q_INVOKABLE void importDataFromFlash(QUrl file);
    Q_INVOKABLE void exportDataToFlash(QUrl file);
    Q_INVOKABLE void openMap(QUrl file);
    Q_INVOKABLE void saveMap(QUrl file);




    Q_PROPERTY(QString lastFile READ lastFile WRITE setLastFile NOTIFY lastFileChanged)
    Q_PROPERTY(QString lastEvent READ lastEvent WRITE setLastEvent NOTIFY lastEventChanged)
    Q_PROPERTY(bool outputOctave READ outputOctave WRITE setOutputOctave NOTIFY outputOctaveChanged)

    QString lastFile() const
    {
        return m_lastFile;
    }



    QString lastEvent() const
    {
        return m_lastEvent;
    }

    bool outputOctave() const
    {
        return m_outputOctave;
    }

signals:

    void lastFileChanged(QString lastFile);

    void lastEventChanged(QString lastEvent);

    void outputOctaveChanged(bool outputOctave);

public slots:
    void onCnxStatusChanged(bool cnxStatus);
    void setLastFile(QString lastFile);
    void onDistInComming(unsigned long UID, int dist, int X, int Y, int Z, int radio);
    void onPosInComming(int X, int Y, int Z, long time);
    void setLastEvent(QString lastEvent);
    void setOutputOctave(bool outputOctave)
    {
        if (m_outputOctave == outputOctave)
            return;

        m_outputOctave = outputOctave;
        emit outputOctaveChanged(m_outputOctave);
    }
};

#endif // FILE_MANAGER_H

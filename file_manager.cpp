#include "file_manager.h"
#include "libdl/dl_dxf.h"
#include <QStringList>


const QString file_manager::JSON_ID="ID";
const QString file_manager::JSON_POS_X="X";
const QString file_manager::JSON_POS_Y="Y";
const QString file_manager::JSON_POS_Z="Z";
const QString file_manager::JSON_DESCRIPTION="DESCRIPTION";

const QString file_manager::JSON_ANCHOR="ANCHORS";
const QString file_manager::JSON_MAP="MAP";


void file_manager::onCnxStatusChanged(bool cnxStatus){
    if(!cnxStatus){
        logMatFile.close();
        setLastEvent("");
    }else{
        setLastEvent("Generated mat-file: "+logMatFile.open());
    }
}

void file_manager::setLastFile(QString lastFile)
{
    if (m_lastFile == lastFile)
        return;

    m_lastFile = lastFile;
    emit lastFileChanged(m_lastFile);
}

void file_manager::onDistInComming(long UID, int dist, int X, int Y, int Z, int radio)
{
    logMatFile.addAnchor(UID,dist,X,Y,Z,radio);
}

void file_manager::onPosInComming(int X, int Y, int Z)
{
    logMatFile.write(X,Y,Z);
}

void file_manager::setLastEvent(QString lastEvent)
{
    if (m_lastEvent == lastEvent)
        return;

    m_lastEvent = lastEvent;
    emit lastEventChanged(m_lastEvent);
}

QJsonArray file_manager::toJson(const device::HASH_OF_ANCHOR &h)
{
    QJsonArray Ja;
    for(auto i:h){
        QJsonObject jo;
        jo[JSON_ID] = QString::number(i.ID,16);
        jo[JSON_POS_X] = static_cast<int>(i.X);
        jo[JSON_POS_Y] = static_cast<int>(i.Y);
        jo[JSON_POS_Z] = static_cast<int>(i.Z);

        Ja.append(jo);
    }
    return Ja;
}

file_manager::file_manager(QString name, device &Device, AnchorsCollections &Anchors, img_provider &ImgProvider, QObject *parent)
    : Qml_object (name,parent),Device(Device),Anchors(Anchors),ImgProvider(ImgProvider)
{
    setOutputOctave(true);
}

bool file_manager::saveJson(QUrl file,QJsonObject jo){
    qDebug()<<Q_FUNC_INFO<<" "<<file.toLocalFile();

    QFile saveFile(file.toLocalFile());
    if(saveFile.open(QIODevice::ReadWrite
                     | QIODevice::Truncate
                     | QIODevice::Text) == false)
    {
        return false;
    }

    QJsonDocument jsdoc = QJsonDocument(jo);
    qDebug()<<"saving data: " << jsdoc.toJson();
    saveFile.write(jsdoc.toJson());

    return true;
}

void file_manager::importDataFromFlash(QUrl file)
{

    setLastURLS(file);
    QJsonObject jo;
    jo[JSON_ANCHOR]=toJson(Device.getAnchorPos());
    if(saveJson(file,jo)) setLastURLS(file);
}


bool file_manager::readJson (QUrl file,QJsonDocument &jsdoc){


    QFile loadFile(file.toLocalFile());
    if(loadFile.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }
    QByteArray savedData = loadFile.readAll();

    jsdoc = QJsonDocument::fromJson(savedData);

    if(jsdoc.isEmpty())
    {
        qWarning()<<"THE LOADED CONFIGURATION DOCUMENT IS NOT VALID : file is empty";
        return false;
    }

    if(!jsdoc.isObject()){
        qWarning()<<"THE LOADED CONFIGURATION DOCUMENT IS NOT VALID : file is not json object";
        return false;
    }

    return true;
}

bool file_manager::openMapJSON(QUrl file)
{
    qDebug()<<Q_FUNC_INFO<<" open json";

    QHash<Anchor_type::TYPE,QString> type;
    type.insert(Anchor_type::ANCHOR,JSON_ANCHOR);
    type.insert(Anchor_type::POI,JSON_MAP);


    QJsonDocument jsdoc;
    if(!readJson(file,jsdoc)) return false;

    QList<Anchor_type::TYPE> lType = type.keys();

    Anchors.getAnchorFromFile().clear();
    for(auto t : lType){
        const QString &strType = type[t];
        QJsonArray jAnchor = jsdoc.object()[strType].toArray();
        if(jAnchor.isEmpty())
        {
            qWarning()<<"THE LOADED CONFIGURATION DOCUMENT IS NOT VALID : no found "<<strType<<" array";
            continue;
        }


        for(auto a : jAnchor){
            anchor d;
            if(!a.isObject()){
                qWarning()<<"THE LOADED CONFIGURATION DOCUMENT IS NOT VALID "<<__LINE__;
                continue ;
            }
            auto o = a.toObject();

            auto strID =o[JSON_ID].toString();
            bool bOK;


            d.ID = strID.simplified().toLongLong(&bOK,16);

            if(!bOK) continue;

            d.X = o[JSON_POS_X].toInt();
            d.Y = o[JSON_POS_Y].toInt();
            d.Z = o[JSON_POS_Z].toInt();

            Anchors.getAnchorFromFile().UpdateAnchor(d.ID,0,d.X,d.Y,d.Z,0,t);

        }
    }
    return true;
}

bool file_manager::openMapDXF(QUrl file)
{
    qDebug()<<Q_FUNC_INFO<<" open dxf";

    dxf_reader readerDXF(file,this,Anchors.getAnchorFromFile());
    return true;
}

bool file_manager::openMapIMG(QUrl file)
{
    QImage img(file.toLocalFile());
    ImgProvider.ChangeImg(img);
    return true;
}

void file_manager::exportDataToFlash(QUrl file){
    qDebug()<<Q_FUNC_INFO<<" "<<file.toLocalFile();
    QJsonDocument jsdoc;

    if(!readJson(file,jsdoc)) return;

    QJsonArray jAnchor = jsdoc.object()[JSON_ANCHOR].toArray();
    if(jAnchor.isEmpty())
    {
        qWarning()<<"THE LOADED CONFIGURATION DOCUMENT IS NOT VALID : no found ANCHOR array";
        return;
    }


    for(auto a : jAnchor){
        anchor d;
        if(!a.isObject()){
            qWarning()<<"THE LOADED CONFIGURATION DOCUMENT IS NOT VALID "<<__LINE__;
            continue ;
        }
        auto o = a.toObject();

        auto strID =o[JSON_ID].toString();
        bool bOK;

        d.ID = strID.toLong(&bOK,16);

        if(!bOK) continue;

        d.X = o[JSON_POS_X].toInt();
        d.Y = o[JSON_POS_Y].toInt();
        d.Z = o[JSON_POS_Z].toInt();


        Device.setPosAnchor(d);

    }
    setLastURLS(file);
}

void file_manager::openMap(QUrl file)
{
    qDebug()<<Q_FUNC_INFO<<" "<<file.toLocalFile();

    file_type type(file);

    switch(type()){
    case file_type::TYPE::DXF :openMapDXF(file);break;
    case file_type::TYPE::JSON :openMapJSON(file);break;
    case file_type::TYPE::IMG: openMapIMG(file);break;
    case file_type::TYPE::OTHER :{
        setLastEvent(type.getError());
        return;
    }
    }
    setLastURLS(file);
}

void file_manager::saveMap(QUrl file)
{
    qDebug()<<Q_FUNC_INFO<<" "<<file.toLocalFile();

    QJsonArray jAnchor;
    QJsonArray jMap;

    for(auto a : Anchors.getAnchorFromFile().getList()){

        QJsonObject jo;
        jo[JSON_ID] = QString::number(a.UID,16);
        jo[JSON_POS_X] = static_cast<int>(a.X);
        jo[JSON_POS_Y] = static_cast<int>(a.Y);
        jo[JSON_POS_Z] = static_cast<int>(a.Z);
        jo[JSON_DESCRIPTION] = a.descrip;


        if(a.anchorType == Anchor_type::ANCHOR){
            jAnchor.append(jo);
        }else{
            //anchor type map
            jMap.append(jo);
        }
    }

    QJsonObject jo;
    jo[JSON_ANCHOR] = jAnchor;
    jo[JSON_MAP] = jMap;

    if(saveJson(file,jo))   setLastURLS(file);

}

file_manager::dxf_reader::dxf_reader(QUrl urlfile, file_manager *parent, AnchorsList &anchors)
    :anchors(anchors)
{
    anchors.clear();
    uid = 1;
    // Load DXF file into memory:
    char *file = urlfile.toLocalFile().toLocal8Bit().data();

    DL_Dxf  dxf;

    if (!dxf.in(file,this)) { // if file open failed
        parent->setLastEvent(QString(file)+" could not be opened");
    }
    else{
        parent->setLastURLS(urlfile);
    }
}

void file_manager::dxf_reader::addPoint(const DL_PointData &data)
{
    qDebug()<<Q_FUNC_INFO<<QString("X: %1 y: %2 z: %3").arg(data.x).arg(data.y).arg(data.z);
    anchors.UpdateAnchor(uid++,0,toCm(data.x),toCm(data.y),toCm(data.z),0,Anchor_type::POI);
}



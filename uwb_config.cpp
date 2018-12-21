#include "uwb_config.h"

void uwb_config::valueChange(QString nameOfValue, int idx)
{
    qDebug()<<Q_FUNC_INFO<<" "<<nameOfValue<< " idx:"<<idx;

    //    auto i = setOfItems["TRXcode"];
    //    delete i;
    //    setOfItems["TRXcode"] = new uwb_config_item(ctx,"TRXcode",{1,2,3,40});

    // Controle de la cohérence du TRXcode en fct du couple [CHAN;PRF]
    if( nameOfValue=="CHAN"){
        if( idx==0 && setOfItems["PRF"]->idx()==0 ) //CHAN1-PRF16
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{1,2}));
        if( (idx==1 || idx==4) && setOfItems["PRF"]->idx()==0 ) //CHAN2-PRF16 ou CHAN5-PRF16
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{3,4}));
        if( idx==2 && setOfItems["PRF"]->idx()==0 ) //CHAN3-PRF16
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{5,6}));
        if( (idx==3 || idx==5) && setOfItems["PRF"]->idx()==0 ) //CHAN4-PRF16 ou CHAN7-PRF16
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{7,8}));

        if( (idx==0 || idx==1 || idx==2 || idx==4) && setOfItems["PRF"]->idx()==1 ) //CHAN1-PRF64 ou CHAN2-PRF64 ou CHAN3-PRF64 ou CHAN5-PRF64
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{9,10,11,12}));
        if( (idx==3 || idx==5) && setOfItems["PRF"]->idx()==1 ) //CHAN4-PRF64 ou CHAN7-PRF64
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{17,18,19,20}));
    }
    else if (nameOfValue=="PRF") {
        if( idx==0 && setOfItems["CHAN"]->idx()==0)//PRF16-CHAN1
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{1,2}));
        if( idx==0 && (setOfItems["CHAN"]->idx()==1 || setOfItems["CHAN"]->idx()==4) )//PRF16-CHAN2 ou PRF16-CHAN5
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{3,4}));
        if( idx==0 && setOfItems["CHAN"]->idx()==2)//PRF16-CHAN3
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{5,6}));
        if( idx==0 && (setOfItems["CHAN"]->idx()==3 || setOfItems["CHAN"]->idx()==5) )//PRF16-CHAN4 ou PRF16-CHAN7
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{7,8}));

        if( idx==1 && (setOfItems["CHAN"]->idx()==0 || setOfItems["CHAN"]->idx()==1 || setOfItems["CHAN"]->idx()==2 || setOfItems["CHAN"]->idx()==4 ) )//PRF64-CHAN2
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{9,10,11,12}));
        if( idx==1 && (setOfItems["CHAN"]->idx()==3 || setOfItems["CHAN"]->idx()==5 ) )//PRF64-CHAN4 ou PRF64-CHAN7
            setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{17,18,19,20}));
    }
    else if (nameOfValue=="BR") {
        if( idx==0 )//850kbps
            setOfItems.insert("PLEN",new uwb_config_item(ctx,"PLEN",{256,512,1024}));
        if( idx==1 )//6800kbps
            setOfItems.insert("PLEN",new uwb_config_item(ctx,"PLEN",{64,128,256}));
    }
    else if (nameOfValue=="PLEN") {
        if( idx==0 || idx==1)//64 ou 128
            setOfItems.insert("PAC",new uwb_config_item(ctx,"PAC",{8}));
        if( idx==2 )//256 ou 512
            setOfItems.insert("PAC",new uwb_config_item(ctx,"PAC",{16}));
        if( idx==4 )//1024
            setOfItems.insert("PAC",new uwb_config_item(ctx,"PAC",{32}));
    }
}

uwb_config::uwb_config(const QString &nameObj,QQmlContext *ctx)
    :Qml_object(nameObj),ctx(ctx)
{

    setOfItems.insert("CHAN",new uwb_config_item(ctx,"CHAN",{1,2,3,4,5,7}));
    setOfItems.insert("PRF",new uwb_config_item(ctx,"PRF",{16,64}));
    setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{}));
    setOfItems.insert("BR",new uwb_config_item(ctx,"BR",{850,6800}));
    setOfItems.insert("PAC",new uwb_config_item(ctx,"PAC",{}));
    setOfItems.insert("PLEN",new uwb_config_item(ctx,"PLEN",{}));

    //    setOfItems.insert("TRXcode",new uwb_config_item(ctx,"TRXcode",{1,2,3,4,5,6,7,8,9,10,11,12,17,18,19,20}));
    //    setOfItems.insert("BR",new uwb_config_item(ctx,"BR",{110,850,6800}));
    //    setOfItems.insert("PAC",new uwb_config_item(ctx,"PAC",{8,16,32,64}));
    //    setOfItems.insert("PLEN",new uwb_config_item(ctx,"PLEN",{64,128,256,512,1024,1536,2048,4096}));

    resetValue();
    registerToQml(ctx);
}

void uwb_config::resetValue()
{
    for(auto a:setOfItems){
        a->setIdx(-1);
    }
}

QString uwb_config::formatConfigCmd()
{
    auto s = QString("AT+CFG=%1,%2,%3,%4,%5,%6,33\r\n")
            .arg(setOfItems["CHAN"]->getCurrentValue())
            .arg(setOfItems["PRF"]->getCurrentValue())
            .arg(setOfItems["TRXcode"]->getCurrentValue())
            .arg(setOfItems["BR"]->getCurrentValue())
            .arg(setOfItems["PLEN"]->getCurrentValue())
            .arg(setOfItems["PAC"]->getCurrentValue());

    qDebug()<<Q_FUNC_INFO<<"cfg : "<<s;
    return s;
}

uwb_config_item::uwb_config_item(QQmlContext *ctx, const QString name, std::initializer_list<qint32> args)
    :list(args){
    QStringList listStr;
    for(auto i:list){
        listStr<<QString::number(i);
    }

    ctx->setContextProperty("model_uwb_"+name,QVariant::fromValue(listStr));
    ctx->setContextProperty("model_uwb_obj_"+name,this);
}

void uwb_config_item::setCurrentValue(qint32 value)
{
    setIdx(list.lastIndexOf(value));
}

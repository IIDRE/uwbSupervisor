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

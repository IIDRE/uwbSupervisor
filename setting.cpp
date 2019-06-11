#include "setting.h"


Setting * Setting ::instance=nullptr;

Setting::Setting(const QString fileName , const QString qmlName)
    :Qml_object (qmlName)
    ,settingFile(fileName,QSettings::IniFormat)
    ,m_duoPoint(false)
    ,m_filterValue(0)
    ,m_fileSimulation()
{
    setDuoPoint(get("filter_duo",m_duoPoint).toBool());
    setFilterValue(get("filter_value",m_filterValue).toInt());
    setFileSimulation(get("simulation_file",m_fileSimulation).toString());
}

QVariant Setting::get(const QString &_key, QVariant def)
{
    QVariant value = settingFile.value(_key);

    if(!value.isValid())
    {
        qInfo()<<Q_FUNC_INFO<<"value for key "<<_key<<" not valid we set default value ";
        settingFile.setValue(_key,def);//value not valid, we take the default value and write the value on the ini file
        return def;
    }
    return value;//return read value in the ini file}
}

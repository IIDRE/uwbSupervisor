#ifndef SETTING_H
#define SETTING_H

#include <QSettings>
#include <QDebug>
#include "qml_object.h"


class Setting : public Qml_object
{
    Q_OBJECT
public:
    Q_PROPERTY(bool duoPoint READ duoPoint WRITE setDuoPoint NOTIFY duoPointChanged)
    Q_PROPERTY(int filterValue READ filterValue WRITE setFilterValue NOTIFY filterValueChanged)
    Q_PROPERTY(QString fileSimulation READ fileSimulation WRITE setFileSimulation NOTIFY fileSimulationChanged)

    static Setting * getInstance(){
        if(!instance){
            instance = new Setting("settings.ini","settings");
        }
        return instance;
    }
    bool duoPoint() const
    {
        return m_duoPoint;
    }
    int filterValue() const
    {
        return m_filterValue;
    }

    QString fileSimulation() const
    {
        return m_fileSimulation;
    }

public slots:
    void setDuoPoint(bool duoPoint)
    {
        if (m_duoPoint == duoPoint)
            return;

        m_duoPoint = duoPoint;
        emit duoPointChanged(m_duoPoint);
    }
    void setFilterValue(int filterValue)
    {
        if (m_filterValue == filterValue)
            return;

        m_filterValue = filterValue;
        emit filterValueChanged(m_filterValue);
    }

    void setFileSimulation(QString fileSimulation)
    {
        if (m_fileSimulation == fileSimulation)
            return;

        m_fileSimulation = fileSimulation;
        emit fileSimulationChanged(m_fileSimulation);
    }

signals:
    void duoPointChanged(bool duoPoint);
    void filterValueChanged(int filterValue);

    void fileSimulationChanged(QString fileSimulation);

private:
    QSettings settingFile;
    Setting(const QString fileName , const QString qmlName);
    ~Setting(){}

    bool m_duoPoint;
    int m_filterValue;

    static Setting * instance;

    QVariant get(const QString &_key, QVariant def);
    QString m_fileSimulation;
};

#endif // SETTING_H

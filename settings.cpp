#include "settings.h"
#include <QIODevice>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

Settings::Settings(QObject *parent) : QObject(parent)
{
    settingFilePath = QDir::currentPath()+"/settings.json";
    dir.setPath(settingFilePath);
    //qDebug()<<dir<<endl;
}

bool Settings::loadLocalSettingFile()
{
    if (!QFile::exists(settingFilePath))
    {
        //qDebug()<<"Settings file not exists!"<<endl;
        return false;
    }

    settings_file = new QFile(settingFilePath);
    if (!settings_file->open(QIODevice::ReadOnly))
    {
        delete settings_file;
        settings_file = nullptr;
        emit signalSettingsError(tr("Fail to read setting file!"));
        return false;
    }
    QString settings_json = settings_file->readAll();
    settings_file->close();
    delete settings_file;
    settings_file = nullptr;

    QJsonParseError error;
    QJsonDocument settings_document = QJsonDocument::fromJson(settings_json.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError)
    {
        if(settings_document.isObject())
        {
            QJsonObject obj_settings = settings_document.object();
            if(obj_settings.contains("musicSavePath"))
            {
                if(obj_settings["musicSavePath"].isString())
                    musicSavePath = obj_settings["musicSavePath"].toString();
                else
                {
                    //qDebug()<<"Settings file parse failed!"<<endl;
                    return false;
                }
            }
            else
                return false;
            if(obj_settings.contains("isAlwaysDirectDownload"))
            {
                if(obj_settings["isAlwaysDirectDownload"].isBool())
                    AlwaysDirectDownload = obj_settings["isAlwaysDirectDownload"].toBool();
                else
                    return false;
            }
            else
                return false;
            if(obj_settings.contains("autoHide"))
            {
                if(obj_settings["autoHide"].isBool())
                    autoHide = obj_settings["autoHide"].toBool();
                else
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
    return true;
}

bool Settings::loadSettings(QString &musicDownloadDir, bool &isAlwaysDirectDownload, bool &autoHide)
{
    if(!loadLocalSettingFile())
    {
        //qDebug()<<"Settings file load failed!"<<endl;
        this->AlwaysDirectDownload = true;
        this->autoHide = false;
        this->musicSavePath = QDir::currentPath()+"/music/";
    }

    musicDownloadDir = this->musicSavePath;
    isAlwaysDirectDownload = this->AlwaysDirectDownload;
    autoHide = this->autoHide;

    //qDebug()<<musicDownloadDir<<endl;
    //qDebug()<<isAlwaysDirectDownload<<endl;
    //qDebug()<<autoHide<<endl;

    dir.setPath(musicSavePath);
    if(!dir.exists())
    {
        //qDebug()<<filepath+"/music"<<endl;
        if(!dir.mkdir(dir.path()))
        {
            emit signalSettingsError(tr("Fail to create music download directory!"));
            return false;
        }
    }
    return true;
}

bool Settings::saveSettings(QString &musicDownloadDir, bool &isAlwaysDirectDownload, bool &autoHide)
{
    if (QFile::exists(settingFilePath))
    {
        QFile::remove(settingFilePath);
    }

    settings_file = new QFile(settingFilePath);
    if (!settings_file->open(QIODevice::WriteOnly))
    {
        emit signalSettingsError(tr("Fail to save settings!"));
        return false;
    }

    QJsonObject obj_settings;
    obj_settings.insert("musicSavePath", musicDownloadDir);
    obj_settings.insert("isAlwaysDirectDownload", isAlwaysDirectDownload);
    obj_settings.insert("autoHide", autoHide);

    QJsonDocument settings_document;
    settings_document.setObject(obj_settings);
    QByteArray byteArray = settings_document.toJson(QJsonDocument::Compact);
    settings_file->write(byteArray);
    settings_file->flush();
    settings_file->close();
    delete settings_file;
    settings_file = 0;

    dir.setPath(musicDownloadDir);
    if(!dir.exists())
    {
        //qDebug()<<filepath+"/music"<<endl;
        if(!dir.mkdir(dir.path()))
        {
            emit signalSettingsError(tr("Fail to create music download directory!"));
            return false;
        }
    }
    return true;
}

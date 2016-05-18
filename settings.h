#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDir>
#include <QFile>

class Settings : public QObject
{
    Q_OBJECT
private:
    QString settingFilePath;
    QString musicSavePath;
    bool AlwaysDirectDownload;
    bool autoHide;
    QDir dir;
    QFile *settings_file;

    bool loadLocalSettingFile();
public:
    explicit Settings(QObject *parent = 0);

    bool loadSettings(QString &musicDownloadDir, bool &isAlwaysDirectDownload, bool &autoHide);
    bool saveSettings(QString &musicDownloadDir, bool &isAlwaysDirectDownload, bool &autoHide);
signals:
    void signalSettingsError(QString error);
public slots:
};

#endif // OPTIONLOADER_H

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFile>
#include <QSettings>
#include "constant.h"

struct Configs
{
    QString registerCode;   //注册码

    QString downloadDirectory;  //音乐下载目录
    bool autoDownload = false;  //解析后自动下载
    bool autoHide = false;  //启动时自动隐藏
    bool showGuidanceOnStart = true;
    int quality = 0;
    bool useJsonParser = true;

};

class Settings
{
private:
    int status = 0; //0:正常, 1:读取失败,使用默认配置 2:写入失败 3:应用配置失败

    Settings();
    Settings(const Settings&) {}
    Settings& operator =(const Settings&) {return *this;}

    void mkDownloadDir();   //创建下载目录
public:
    Configs *configs;

    ~Settings();

    void setStatus(int status);
    int getStatus() const;

    static Settings& getSettings()
    {
        static Settings settings;
        return settings;
    }

    /*
     * Description：导入配置信息
     * Output: 配置是否成功载入
     */
    void load();
    /*
     * Description：保存配置信息至本地配置文件
     * Output: 配置是否成功保存
     */
    void save();
};

#endif // OPTIONLOADER_H

#include "settings.h"
#include <QDir>

Settings::Settings()
{
    configs = new Configs;
}

Settings::~Settings()
{

}

void Settings::setStatus(int status)
{
    this->status = status;
}

int Settings::getStatus() const
{
    return status;
}

void Settings::mkDownloadDir()
{
    QDir dir;
    //如果音乐下载目录不存在则创建
    dir.setPath(configs->downloadDirectory);
    if(!dir.exists())
    {
        //如果无法创建则发出错误信息
        if(!dir.mkpath(dir.path()))
        {
            this->status = 3;
        }
    }
}

//读取配置文件并将配置信息同步到主窗口配置变量中，并使配置生效
void Settings::load()
{
    QSettings settings(".163config", QSettings::IniFormat);
    if(settings.contains("exists"))
    {
        configs->registerCode = settings.value("registercode").toString();
        configs->autoDownload = settings.value("autodownload").toBool();
        configs->autoHide = settings.value("autohide").toBool();
        configs->downloadDirectory = settings.value("downloaddirectory").toString();
        configs->showGuidanceOnStart = settings.value("showguidanceonstart").toBool();
        configs->quality = settings.value("quality").toInt();
        configs->useJsonParser = settings.value("usejsonparser").toBool();
    }
    else
    {
        configs->autoDownload = true;   //默认配置为开启后台自动下载
        configs->autoHide = false;  //关闭启动时自动隐藏
        //音乐下载目录为当前软件目录下的music目录
        configs->downloadDirectory = QDir::currentPath()+"/music/";
        configs->showGuidanceOnStart = true; //启动时显示提示
        configs->quality = 0;  //默认下载高品质音乐
        configs->useJsonParser = true;  //默认通过json解析
        this->status = 1;
    }

    mkDownloadDir();
}

//保存配置信息到本地
void Settings::save()
{
    QSettings settings(".163config", QSettings::IniFormat);
    if(settings.isWritable())
    {
        settings.setValue("exists", true);
        settings.setValue("registercode", configs->registerCode);
        settings.setValue("autodownload", configs->autoDownload);
        settings.setValue("autohide", configs->autoHide);
        settings.setValue("downloaddirectory", configs->downloadDirectory);
        settings.setValue("showguidanceonstart", configs->showGuidanceOnStart);
        settings.setValue("quality", configs->quality);
        settings.setValue("usejsonparser", configs->useJsonParser);
        settings.sync();
    }
    else
    {
        this->status = 2;
    }

    mkDownloadDir();
}

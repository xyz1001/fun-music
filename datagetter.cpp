#include "datagetter.h"

DataGetter::DataGetter(QObject *parent) : QObject(parent)
{
    music_info = new QMap<QString, QString>;
    jsonGetter = nullptr;
    jsonHandler = nullptr;
    htmlGetter = nullptr;
    htmlHandler = nullptr;
    pictureGetter = nullptr;
    mp3Getter = nullptr;
}

DataGetter::~DataGetter()
{
    delete music_info;
}

void DataGetter::getPicture(QUrl picUrl)
{
    if(pictureGetter)
    {
        delete pictureGetter;
        pictureGetter = nullptr;
    }
    pictureGetter = new PictureGetter(picUrl, this);
    //转发显示错误信息信号
    connect(pictureGetter, &PictureGetter::signalError, this, &DataGetter::signalError);
    //转发图片下载完成信号
    connect(pictureGetter, &PictureGetter::signalPictureDownloaded, this, &DataGetter::signalPictureGotten);
    //获取图片
    pictureGetter->getPicture();
}

void DataGetter::downloadMP3(QUrl mp3Url, QString filepath, QWidget *widget)
{
    //qDebug()<<"prrepare downloading..."<<endl;
    if(mp3Getter)
    {
        delete mp3Getter;
        mp3Getter = nullptr;
    }
    mp3Getter = new MP3Getter(mp3Url, filepath, widget, this);
    //信号转发
    connect(mp3Getter, &MP3Getter::signalError, this, &DataGetter::signalError);
    connect(mp3Getter, &MP3Getter::signalInfo, this, &DataGetter::signalInfo);
    connect(mp3Getter, &MP3Getter::signalDownloadProgress, this, &DataGetter::signalDownloadProgress);
    connect(mp3Getter, &MP3Getter::signalMP3Downloaded, this, &DataGetter::signalMP3downloaded);
    //开始下载mp3
    mp3Getter->getMP3();
}


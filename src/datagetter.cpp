#include "datagetter.h"
#include <QMessageBox>

DataGetter::DataGetter(MusicInfo *musicInfo, QWidget *widget, QObject *parent) : QObject(parent)
{
    this->musicInfo = musicInfo;
    this->row = row;
    this->widget = widget;

    pictureGetter = nullptr;
    mp3Getter = nullptr;
}

DataGetter::~DataGetter()
{

}

void DataGetter::updateDownloadInfo()
{
    quality = Settings::getSettings().configs->quality <= musicInfo->downloadInfo.size()-1?
                Settings::getSettings().configs->quality : musicInfo->downloadInfo.size()-1;
    downloadPath = QString("%1%2 - %3.%4")
            .arg(Settings::getSettings().configs->downloadDirectory)
            .arg(musicInfo->artist)
            .arg(musicInfo->name)
            .arg(musicInfo->downloadInfo.at(quality).extension);
}

void DataGetter::startHandle()
{
    emit signalTableWidgetMusicInfoUpdate(musicInfo->row, STATUS, tr("Parsing"));
    getMusicInfo();
}

void DataGetter::getPicture()
{
    if(pictureGetter)
    {
        delete pictureGetter;
        pictureGetter = nullptr;
    }
    pictureGetter = new PictureGetter(musicInfo->picUrl, this);

    //转发图片下载完成信号
    connect(pictureGetter, &PictureGetter::signalPictureDownloaded, this, &DataGetter::onPictureGotten);
    connect(pictureGetter, &PictureGetter::errorOccurred, this, &DataGetter::handleError);
    //获取图片
    pictureGetter->start();
}

void DataGetter::downloadMP3()
{
    emit signalTableWidgetMusicInfoUpdate(musicInfo->row, STATUS, tr("Downloading"));
    //qDebug()<<"prrepare downloading..."<<endl;
    if(mp3Getter)
    {
        delete mp3Getter;
        mp3Getter = nullptr;
    }

    updateDownloadInfo();
    mp3Getter = new MP3Getter(musicInfo->downloadInfo.at(quality).mp3Url, downloadPath, this);
    //信号转发
    connect(mp3Getter, &MP3Getter::errorOccurred, this, &DataGetter::handleError);
    connect(mp3Getter, &MP3Getter::signalInfo, this, &DataGetter::signalInfo);
    connect(mp3Getter, &MP3Getter::signalDownloadProgress, this, &DataGetter::updateDownloadProgress);
    connect(mp3Getter, &MP3Getter::signalMP3Downloaded, this, &DataGetter::onMp3Downloaded);

    //如果歌曲已存在则弹窗询问是否覆盖
    if (QFile::exists(downloadPath))
    {
        if (QMessageBox::question(widget, tr("File exists"),
                                  tr("There already exists a file called %1 in "
                                     "the current directory. Overwrite?").arg(downloadPath),
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
                == QMessageBox::No)
        {
            updateDownloadProgress(QFile(downloadPath).size(), QFile(downloadPath).size());
            onMp3Downloaded();
            return;
        }
        else
        {
            QFile::remove(downloadPath);
        }
    }
    //开始下载mp3
    mp3Getter->start();
}

void DataGetter::musicInfoGotten()
{
    emit signalMusicInfoGotten(musicInfo);
    updateDownloadInfo();
    musicInfo->status = 1;
    emit signalTableWidgetMusicInfoUpdate(musicInfo->row, MUSIC_NAME, musicInfo->name);
    if(musicInfo->downloadInfo.at(quality).size != 0)
    {
        QString size = QString("%1M").arg(musicInfo->downloadInfo.at(quality).size * 10 / (1024*1024) * 1.0/10);
        emit signalTableWidgetMusicInfoUpdate(musicInfo->row, MUSIC_SIZE,
                                              size);
        QString duration = QString("%1:%2")
                .arg(musicInfo->duration/60000)
                .arg(musicInfo->duration%60000/1000);
        emit signalTableWidgetMusicInfoUpdate(musicInfo->row, MUSIC_DURATION, duration);
    }
    getPicture();


    if(Settings::getSettings().configs->autoDownload)
    {
        downloadMP3();
    }
    else
        emit signalTableWidgetMusicInfoUpdate(musicInfo->row, STATUS, tr("Parse Successfully"));
}

void DataGetter::onPictureGotten(QPixmap *pixmap)
{
    musicInfo->pic = pixmap;
    emit signalPictureGotten(musicInfo);
    delete pictureGetter;
    pictureGetter = nullptr;
    if(!(Settings::getSettings().configs->autoDownload))
        emit signalFinished();
}

void DataGetter::onMp3Downloaded()
{
    QString filename = QString("%2 - %3.%4")
            .arg(musicInfo->artist)
            .arg(musicInfo->name)
            .arg(musicInfo->downloadInfo.at(quality).extension);
    emit signalTableWidgetMusicInfoUpdate(musicInfo->row, STATUS, tr("Download Successfully"));
    emit signalInfo(filename + tr("download Successfully"));
    musicInfo->status = 2;
    delete mp3Getter;
    mp3Getter = nullptr;
    emit signalFinished();
}

void DataGetter::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    musicInfo->downloadProgressBar->setMaximum(totalBytes);
    musicInfo->downloadProgressBar->setValue(bytesRead);
    if(musicInfo->duration == 0 && musicInfo->downloadInfo.at(0).size == 0)
    {
        musicInfo->downloadInfo[0].size = totalBytes;
        QString size = QString("%1M").arg(totalBytes * 10 / (1024*1024) * 1.0/10);
        emit signalTableWidgetMusicInfoUpdate(musicInfo->row, MUSIC_SIZE, size);
    }
}

void DataGetter::handleError(QString error, int errorType)
{
    Q_UNUSED(error);
    switch (errorType)
    {
    case DATA_GETTER_ERROR:
        musicInfo->status = 0;
        break;
    case JSON_GETTER_ERROR:
        musicInfo->status = -1;
        emit signalError(musicInfo->musicID + tr("parse unsuccessfully"));
        emit signalTableWidgetMusicInfoUpdate(musicInfo->row, STATUS, tr("Parse Unsuccessfully"));
        break;
    case PICTURE_GETTER_ERROR:
        delete pictureGetter;
        pictureGetter = nullptr;
        break;
    case MP3_GETTER_ERROR:
        musicInfo->status = -2;
        emit signalError(musicInfo->name + tr("download unsuccessfully"));
        emit signalTableWidgetMusicInfoUpdate(musicInfo->row, STATUS, tr("Download Unsuccessfully"));
        delete mp3Getter;
        mp3Getter = nullptr;
        break;
    default:
        break;
    }
}


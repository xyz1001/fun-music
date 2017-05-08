#include "musicinfogetter.h"
#include <QDebug>
#include <QRegExp>

MusicInfoGetter::MusicInfoGetter(MusicInfo *musicInfo, QObject *parent)
    : QObject(parent)
{
    this->musicInfo = musicInfo;
}

void MusicInfoGetter::getMusicInfo()
{
    QString url = QString("http://music.163.com/api/song/detail/?id=%1&ids=[%2]")
            .arg(musicInfo->musicID)
            .arg(musicInfo->musicID);
    jsonGetter = new JsonGetter(QUrl(url), this);
    connect(jsonGetter, &JsonGetter::JsonGotton, this, &MusicInfoGetter::onDetailJsonGotton);
    connect(jsonGetter, &JsonGetter::GetFalied, this, &MusicInfoGetter::onGetJsonFailed);
}

void MusicInfoGetter::onDetailJsonGotton(QString json)
{
    QString pattern = "(\"dfsId\":)(\\d+)";
    QRegExp rx(pattern);
    json.replace(rx, "\\1\"\\2\"");

    jsonParser = new DetailJsonParser(json, musicInfo, this);
    if(musicInfo->status != -1)
    {
        if(musicInfo->status == 0)
        {
            delete jsonGetter;
            QString url = QString("http://music.163.com/api/album/%1")
                    .arg(musicInfo->albumId);
            jsonGetter = new JsonGetter(QUrl(url), this);
            disconnect(jsonGetter, &JsonGetter::JsonGotton,
                       this, &MusicInfoGetter::onDetailJsonGotton);
            connect(jsonGetter, &JsonGetter::JsonGotton,
                    this, &MusicInfoGetter::onAlbumJsonGotton);
        }
        else
        {
            foreach (auto i, musicInfo->downloadInfo)
            {
                qDebug()<<i.mp3Url;
            }
        }
    }

}

void MusicInfoGetter::onAlbumJsonGotton(QString json)
{
    QString pattern = "(\"dfsId\":)(\\d+)";
    QRegExp rx(pattern);
    json.replace(rx, "\\1\"\\2\"");

    delete jsonParser;
    jsonParser = new AlbumJsonParser(json, musicInfo, this);
    if(musicInfo->status == 1)
    {
        foreach (auto i, musicInfo->downloadInfo)
        {
            qDebug()<<i.mp3Url;
        }
    }
    else
        musicInfo->status = -1;
}

void MusicInfoGetter::onGetJsonFailed()
{

}


#include "handlerwithjson.h"
#include <QDebug>
#include <QRegExp>

HandlerWithJson::HandlerWithJson(MusicInfo *musicInfo, QWidget *widget, QObject *parent)
    : DataGetter(musicInfo, widget, parent)
{

}

void HandlerWithJson::getMusicInfo()
{
    QString url = QString("http://music.163.com/api/song/detail/?id=%1&ids=[%2]")
            .arg(musicInfo->musicID)
            .arg(musicInfo->musicID);
    jsonGetter = new JsonGetter(QUrl(url), this);
    connect(jsonGetter, &JsonGetter::JsonGotton, this, &HandlerWithJson::onDetailJsonGotton);
    connect(jsonGetter, &JsonGetter::errorOccurred, this, &HandlerWithJson::handleError);
    jsonGetter->start();
}

void HandlerWithJson::onDetailJsonGotton(QString json)
{
    QString pattern = "(\"dfsId\":)(\\d+)";
    QRegExp rx(pattern);
    json.replace(rx, "\\1\"\\2\"");

    jsonParser = new DetailJsonParser(json, musicInfo, this);
    jsonParser->parse();
    if(musicInfo->status != -1)
    {
        if(musicInfo->status == 0)
        {
            delete jsonGetter;
            QString url = QString("http://music.163.com/api/album/%1")
                    .arg(musicInfo->albumId);
            jsonGetter = new JsonGetter(QUrl(url), this);
            disconnect(jsonGetter, &JsonGetter::JsonGotton,
                       this, &HandlerWithJson::onDetailJsonGotton);
            connect(jsonGetter, &JsonGetter::JsonGotton,
                    this, &HandlerWithJson::onAlbumJsonGotton);
            connect(jsonGetter, &JsonGetter::errorOccurred,
                    this, &HandlerWithJson::handleError);
            jsonGetter->start();
        }
        else
        {
            if(musicInfo->status == 1)
            {
                musicInfoGotten();
            }
            else
                musicInfo->status = -1;
        }
    }
}

void HandlerWithJson::onAlbumJsonGotton(QString json)
{
    QString pattern = "(\"dfsId\":)(\\d+)";
    QRegExp rx(pattern);
    json.replace(rx, "\\1\"\\2\"");

    delete jsonParser;
    jsonParser = new AlbumJsonParser(json, musicInfo, this);
    jsonParser->parse();
    if(musicInfo->status == 1)
    {
        musicInfoGotten();
    }
    else
        musicInfo->status = -1;
}

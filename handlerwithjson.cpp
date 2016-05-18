#include "handlerwithjson.h"
#include <QDebug>

HandlerWithJson::HandlerWithJson(QObject *parent) : DataGetter(parent)
{

}

void HandlerWithJson::getRawInfo(QString song_id)
{
    if(jsonGetter)
    {
        delete jsonGetter;
        jsonGetter = nullptr;
    }
    if(jsonHandler)
    {
        delete jsonHandler;
        jsonHandler = nullptr;
    }
    //qDebug()<<song_id<<endl;
    QString music_url = QString("http://music.163.com/api/song/detail/?id=%1&ids=[%1]&csrf_token=Method=GET").arg(song_id);
    //qDebug()<<music_url<<endl;
    jsonGetter = new JsonGetter(QUrl(music_url),this);
    connect(jsonGetter, &JsonGetter::signalsError, this, &HandlerWithJson::signalError);
    connect(jsonGetter, &JsonGetter::signalJsonGotten, this, &HandlerWithJson::onRawInfoGotten);
    jsonGetter->getJson();
}

void HandlerWithJson::parseRawInfo()
{
    if(jsonGetter->getJsonString().isEmpty())
    {
        delete jsonGetter;
        jsonGetter = nullptr;
        return;
    }
    jsonHandler = new JsonHandler(jsonGetter->getJsonString(), music_info, this);
    connect(jsonHandler, &JsonHandler::signalMusicInfoGotten, this, &HandlerWithJson::signalMusicInfoGotten);
    connect(jsonHandler, &JsonHandler::signalJsonhandlerError, this, &HandlerWithJson::signalError);
    jsonHandler->parseJson();
}

void HandlerWithJson::onRawInfoGotten()
{
    parseRawInfo();
}

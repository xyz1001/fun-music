#include "jsonhandler.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QVariant>
#include <QVariantMap>
#include <QDebug>
#include <exception>

JsonHandler::JsonHandler(QString json_content, QMap<QString, QString> *music_info, QObject *parent)
    :QObject(parent)
{
    this->json_content = json_content;
    this->music_info = music_info;
}

void JsonHandler::parseJson()
{
    //Json解析
    QJsonParseError error;
    //qDebug()<<json_content<<endl;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json_content.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError)
    {

//        (*music_info)["name"] = jsonDocument.object()["songs"].toArray()[0].toObject()["name"].toString();
//        (*music_info)["alias"] = jsonDocument.object()["songs"].toArray()[0].toObject()["alias"].toArray()[0].toString();
//        (*music_info)["artists"] = jsonDocument.object()["songs"].toArray()[0].toObject()["artists"].toArray()[0].toObject()["name"].toString();
//        (*music_info)["artistsId"] = jsonDocument.object()["songs"].toArray()[0].toObject()["artists"].toArray()[0].toObject()["id"].toInt();
//        (*music_info)["album"] = jsonDocument.object()["songs"].toArray()[0].toObject()["album"].toObject()["name"].toString();
//        (*music_info)["albumId"] = jsonDocument.object()["songs"].toArray()[0].toObject()["album"].toObject()["id"].toInt();
//        (*music_info)["mp3Url"] = jsonDocument.object()["songs"].toArray()[0].toObject()["mp3Url"].toString();
//        (*music_info)["picUrl"] = jsonDocument.object()["songs"].toArray()[0].toObject()["album"].toObject()["picUrl"].toString();
        if(jsonDocument.isObject())
        {
            QJsonObject obj = jsonDocument.object();
            if(obj.contains("songs"))
            {
                QJsonArray array_songs = obj["songs"].toArray();
                if(!array_songs.isEmpty() && array_songs[0].isObject())
                {
                    QJsonObject obj_songs_0 = array_songs[0].toObject();
                    if(obj_songs_0.contains("name"))
                        (*music_info)["name"] = obj_songs_0["name"].toString();
                    if(obj_songs_0.contains("alias"))
                    {
                        QJsonArray array_alias = obj_songs_0["alias"].toArray();
                        if(!array_alias.isEmpty() && array_alias[0].isString())
                            (*music_info)["alias"] = array_alias[0].toString();
                    }
                    if(obj_songs_0.contains("artists"))
                    {
                        QJsonArray array_artists = obj_songs_0["artists"].toArray();
                        if(!array_artists.empty() && array_artists[0].isObject())
                        {
                            QJsonObject obj_artists_0 = array_artists[0].toObject();
                            {
                                if(obj_artists_0.contains("name"))
                                    (*music_info)["artists"] = obj_artists_0["name"].toString();
                                if(obj_artists_0.contains("id"))
                                    (*music_info)["artistsId"] = QString::number(obj_artists_0["id"].toInt());
                            }
                        }
                    }
                    if(obj_songs_0.contains("album"))
                    {
                        QJsonObject obj_album = obj_songs_0["album"].toObject();
                        {
                            if(obj_album.contains("name"))
                                (*music_info)["album"] = obj_album["name"].toString();
                            if(obj_album.contains("id"))
                                (*music_info)["albumId"] =QString::number(obj_album["id"].toInt());
                            if(obj_album.contains("picUrl"))
                                (*music_info)["picUrl"] = obj_album["picUrl"].toString();
                        }
                    }
                    if(obj_songs_0.contains("mp3Url"))
                        (*music_info)["mp3Url"] = obj_songs_0["mp3Url"].toString();
                    if(obj_songs_0.contains("id"))
                        (*music_info)["id"] =QString(obj_songs_0["id"].toInt());
                }
            }
        }

        mp3UrlReprocessing();

        if(!((*music_info).contains("name")
                &&(*music_info).contains("artists")
                &&(*music_info).contains("mp3Url")
                ))
        {
            //qDebug()<<(*music_info)["name"];
            //qDebug()<<(*music_info)["artists"];
            //qDebug()<<(*music_info)["mp3Url"];
            emit signalJsonhandlerError(tr("Fail to parsing!"));
        }
        else
            emit signalMusicInfoGotten(music_info);
    }
    else
        emit signalJsonhandlerError(error.errorString());
}

void JsonHandler::mp3UrlReprocessing()
{
    //将移动端链接转换为桌面端链接
    (*music_info)["mp3Url"][7]='p';
}

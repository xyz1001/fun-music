#include "albumjsonparser.h"

AlbumJsonParser::AlbumJsonParser(QString json, MusicInfo *musicInfo, QObject *parent)
    :JsonParser(json, musicInfo, parent)
{

}

void AlbumJsonParser::parse()
{
    //Json解析
    QJsonParseError error;
    //qDebug()<<json_content<<endl;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError)
    {
        QJsonObject jsonObject = jsonDocument.object();
        if (jsonObject["code"].toInt() != 200)
        {
            musicInfo->status = -1;
            return;
        }

        QJsonArray albumArray = jsonDocument.object()["album"].toObject()["songs"].toArray();
        foreach (auto i, albumArray)
        {
            if(i.toObject()["id"].toInt() == musicInfo->musicID)
            {
                QJsonObject songObject = i.toObject();
                musicInfo->duration = songObject["duration"].toInt();
                parseDownloadInfo(songObject);
            }
        }
    }
}


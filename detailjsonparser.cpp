#include "detailjsonparser.h"

DetailJsonParser::DetailJsonParser(QString json, MusicInfo *musicInfo, QObject *parent)
    :JsonParser(json, musicInfo, parent)
{

}

void DetailJsonParser::parse()
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

        QJsonObject songObject = jsonDocument.object()["songs"].toArray()[0].toObject();
        musicInfo->name = songObject["name"].toString();
        if(songObject["alias"].toArray().size() != 0
                && !(songObject["alias"].toArray()[0].isNull()))
            musicInfo->alais = QString("(%1)").arg(songObject["alias"].toArray()[0].toString());

        foreach (auto i, songObject["artists"].toArray())
        {
            musicInfo->artists.push_back(i.toObject()["name"].toString());
            musicInfo->artistId.push_back(i.toObject()["id"].toInt());
        }
        if(musicInfo->artists.size() > 2)
        {
            musicInfo->artist = tr("stars");
        }
        else if(musicInfo->artists.size() == 2)
        {
            musicInfo->artist = musicInfo->artists[0] + "," + musicInfo->artists[1];
        }
        else
        {
            musicInfo->artist = musicInfo->artists[0];
        }

        QJsonObject albumObject = songObject["album"].toObject();
        musicInfo->album = albumObject["name"].toString();
        musicInfo->albumId = albumObject["id"].toInt();
        musicInfo->picUrl = albumObject["picUrl"].toString();

        if(songObject["status"] == 1)
        {
            musicInfo->duration = songObject["duration"].toInt();
            parseDownloadInfo(songObject);
        }
    }
}



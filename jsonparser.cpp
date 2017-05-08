#include "jsonparser.h"
#include <QCryptographicHash>
#include <QByteArray>

QString JsonParser::decryption(QString dfsId)
{
    QByteArray byte1 = "3go8&$8*3*3h0k(2)2";
    QByteArray byte2 = dfsId.toLatin1();
    int byte1_len = byte1.size();
    int byte2_len = byte2.size();
    QByteArray byte3(byte2_len, '\0');
    for (int i = 0; i < byte2_len; ++i)
    {
        byte3[i] = byte2[i] ^ byte1[i%byte1_len];
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(byte3);
    QByteArray byte4 = hash.result();
    byte4 = byte4.toBase64();
    byte4.replace('/', '_');
    byte4.replace('+', '-');

    return QString::fromLatin1(byte4);
}

void JsonParser::getDownloadInfo(QJsonObject object, DownloadInfo &downloadInfo)
{
    downloadInfo.bitrate = object["bitrate"].toInt();
    downloadInfo.dfsId = object["dfsId"].toString();
    downloadInfo.extension = object["extension"].toString();
    downloadInfo.size = object["size"].toInt();
    downloadInfo.mp3Url = QString("http://p%1.music.126.net/%2/%3.%4")
            .arg(downloadInfo.size%3 + 1)
            .arg(decryption(downloadInfo.dfsId))
            .arg(downloadInfo.dfsId)
            .arg(downloadInfo.extension);

    musicInfo->downloadInfo.push_back(downloadInfo);
    musicInfo->status = 1;
}

void JsonParser::parseDownloadInfo(QJsonObject &json)
{
    DownloadInfo downloadInfo;

    if(!(json["hMusic"].isNull()))
    {
        downloadInfo.quality = 2;
        getDownloadInfo(json["hMusic"].toObject(), downloadInfo);
    }
    if(!(json["mMusic"].isNull()))
    {
        downloadInfo.quality = 1;
        getDownloadInfo(json["mMusic"].toObject(), downloadInfo);
    }
    if(!(json["lMusic"].isNull()))
    {
        downloadInfo.quality = 0;
        getDownloadInfo(json["lMusic"].toObject(), downloadInfo);
    }
}

JsonParser::JsonParser(QString json, MusicInfo *musicInfo, QObject *parent) : QObject(parent)
{
    this->json = json;
    this->musicInfo = musicInfo;
}


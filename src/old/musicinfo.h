#ifndef MUSICINFO_H
#define MUSICINFO_H

#include <QString>
#include <QVector>
#include <QPixmap>
#include <QProgressBar>

class DataGetter;

struct DownloadInfo
{
    int quality = 1;    //歌曲品质，0为流畅品质，1为普通品质，2为高品质
    int bitrate = 0;
    QString dfsId;  //下载地址码
    QString mp3Url;
    quint64 size = 0;   //文件大小，单位：字节
    QString extension;  //扩展名
};

struct MusicInfo
{
    int musicID;
    QString name;
    QString alais;
    QString artist;
    int duration = 0;   //歌曲时长
    QVector<QString> artists;
    QString album;
    QPixmap *pic = nullptr;
    QVector<int> artistId;  //歌手列表
    int albumId = 0;
    QString picUrl;
    QVector<DownloadInfo> downloadInfo; //下载地址
    int status = 0; //状态，0为未处理，1为解析完成，2为歌曲下载已完成，-1为解析失败，-2为歌曲下载失败

    DataGetter *dataGetter = nullptr;
    QProgressBar *downloadProgressBar = nullptr;    //下载进度条
    int row = 0;
};


#endif // MUSICINFO_H


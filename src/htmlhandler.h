#ifndef HTMLHANDLER_H
#define HTMLHANDLER_H

#include <QObject>
#include <QMap>
#include "musicinfo.h"

class HtmlHandler : public QObject
{
    Q_OBJECT
private:
    QString html;
    MusicInfo *musicInfo;
    //获取歌曲名
    void getMusicName();
    //获取别名
    void getAlais();
    //获取歌手名
    void getArtist();
    //获取专辑名
    void getAlbum();

    //获取歌曲下载地址
    QString getMP3Url();
    //获取歌曲链接
    void getMusicUrl();
    //获取歌手链接
    void getArtistId();
    //获取专辑链接
    void getAlbumId();
    //获取图片链接
    void getPicUrl();
public:
    explicit HtmlHandler(QString html, MusicInfo *musicInfo, QObject *parent = 0);

    QString parse();
signals:
    void signalHtmlHandlerError(QString error);
};

#endif // HTMLHANDLER_H

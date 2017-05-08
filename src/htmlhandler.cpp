#include "htmlhandler.h"
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QVector>

HtmlHandler::HtmlHandler(QString html, MusicInfo *musicInfo, QObject *parent) : QObject(parent)
{
    this->html = html;
    //qDebug()<<htmlString<<endl;
    this->musicInfo = musicInfo;
}

QString HtmlHandler::parse()
{
    QString error;
    getMusicName();
    getAlais();
    getArtist();
    getAlbum();
    error = getMP3Url();
    getArtistId();
    getAlbumId();
    getPicUrl();

    return error;
}

void HtmlHandler::getMusicName()
{
    //匹配歌曲名
    QString pattern("(<h2>)(.*)(</h2>)");
    QRegExp rx(pattern);
    int pos=0;
    pos=rx.indexIn(html, pos);
    if(pos >= 0)
    {
        //捕获组2为歌曲名
        musicInfo->name = rx.cap(2);
        //qDebug()<<"Music: "<<rx.cap(2)<<endl;
    }
}

void HtmlHandler::getAlais()
{

}

void HtmlHandler::getArtist()
{
    //匹配歌手名
    QString pattern("(</h2>)(\\s+)(<p>)([^;=<>]+)(</p>)");
    QRegExp rx(pattern);
    int pos=0;
    pos=rx.indexIn(html, pos);
    if(pos >= 0)
    {
        //捕获组4为歌手名
        musicInfo->artist = rx.cap(4);
        musicInfo->artists.push_back(musicInfo->artist);
        //qDebug()<<"Artist: "<<rx.cap(4)<<endl;
    }
}

void HtmlHandler::getAlbum()
{
    //匹配专辑名 <p class="text-primary">魔鬼的情诗 II </p></a>
    QString pattern("(<p class=\\\"text-primary\\\">)([^;=<>]+)(</p></a>)");
    QRegExp rx(pattern);
    int pos=0;
    pos=rx.indexIn(html, pos);
    if(pos >= 0)
    {
        //捕获组4为专辑名
        musicInfo->album = rx.cap(2);
        //qDebug()<<"Album: "<<rx.cap(2)<<endl;
    }
}

QString HtmlHandler::getMP3Url()
{
    //匹配歌曲下载地址
    QString pattern("(href=\")([^;<>\"]+.mp3)(\")");
    QRegExp rx(pattern);
    int pos=0;
    pos=rx.indexIn(html, pos);
    if(pos >= 0)
    {
        DownloadInfo downloadInfo;
        downloadInfo.mp3Url = rx.cap(2);
        downloadInfo.extension = "mp3";
        //捕获组2为歌曲下载地址
        musicInfo->downloadInfo.push_back(downloadInfo);
        //qDebug()<<"MP3Url: "<<rx.cap(2)<<endl;
    }
    else
        return tr("Fail to get mp3Url");

    return "";
}

void HtmlHandler::getMusicUrl()
{

}

void HtmlHandler::getArtistId()
{
//    //匹配歌手链接
//    QString pattern("(</h2>)(\s+)(<p>)([\u4e00-\u9fa5]+)");
//    QRegExp rx(pattern);
//    int pos=0;
//    pos=rx.indexIn(htmlString, pos);
//    if(pos >= 0)
//    {
//        //捕获组2为歌手链接
//        musicInfo->artist = rx.cap(4);
//        //qDebug()<<rx.cap(2)<<endl;
//    }
}

void HtmlHandler::getAlbumId()
{
    //匹配专辑链接
    QString pattern("(album/)(\\d+)");
    QRegExp rx(pattern);
    int pos=0;
    pos=rx.indexIn(html, pos);
    if(pos >= 0)
    {
        //捕获组2为专辑链接
        musicInfo->albumId = rx.cap(2).toInt();
    }
}

void HtmlHandler::getPicUrl()
{
    //匹配图片链接
    QString pattern("(href=\")([^;<>\"]+.jpg)(\")");
    QRegExp rx(pattern);
    int pos=0;
    pos=rx.indexIn(html, pos);
    if(pos >= 0)
    {
        //捕获组2为图片链接
        musicInfo->picUrl = rx.cap(2);
        //qDebug()<<"PicUrl: "<<rx.cap(2)<<endl;
    }
}

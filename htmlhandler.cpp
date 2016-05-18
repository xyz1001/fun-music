#include "htmlhandler.h"
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QVector>

HtmlHandler::HtmlHandler(QString html_content, QMap<QString, QString> *music_info, QObject *parent) : QObject(parent)
{
    this->html_content = html_content;
    //qDebug()<<html_content<<endl;
    this->music_info = music_info;
}

void HtmlHandler::getMusicInfo()
{
    getInfomation();
    getLinks();
    emit signalMusicInfoGotten(music_info);
}

void HtmlHandler::getInfomation()
{
    QString pattern("(>)([^;=]+)(</a>)");
    QRegExp rx(pattern);
    int pos=0;
    QVector<QString> key={"name", "artists", "album"};
    //qDebug()<<key[1]<<endl;
    int i=0;
    //依次对匹配的内容进行提取
    while(pos>=0 && i<4)
    {
        //丢弃第一次匹配的内容
        pos=rx.indexIn(html_content, pos);
        if(pos >= 0 && i>0)
        {
            //捕获组2为所需要的信息
            (*music_info)[key[i-1]] = rx.cap(2);
            //qDebug()<<rx.cap(2)<<endl;
        }
        //跳过本次匹配到的内容继续匹配
        pos+=rx.matchedLength();
        ++i;
    }
}

void HtmlHandler::getLinks()
{
    QString pattern("(href=\")([^<>\"]+music[^<>\"]+)(\")");
    QRegExp rx(pattern);
    int pos=0;
    QVector<QString> key={"songUrl", "artistsUrl", "albumUrl", "mp3Url", "picUrl"};
    int i=0;

    while(pos>=0 && i<5)
    {
        pos=rx.indexIn(html_content, pos);
        if(pos >= 0)
        {
            (*music_info)[key[i]] = rx.cap(2);
            //qDebug()<<rx.cap(2)<<endl;
        }
        pos+=rx.matchedLength();
        ++i;
    }
}

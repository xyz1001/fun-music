#include "handlerwithhtml.h"
#include "settings.h"

HandlerWithHtml::HandlerWithHtml(MusicInfo *musicInfo, QWidget *widget, QObject *parent)
    :DataGetter(musicInfo, widget, parent)
{

}

HandlerWithHtml::~HandlerWithHtml()
{

}

void HandlerWithHtml::getMusicInfo()
{
    if(htmlGetter)
    {
        delete htmlGetter;
        htmlGetter = nullptr;
    }
    //歌曲查询链接拼接
    QString musicUrl;
    if(Settings::getSettings().configs->quality == 0)
        musicUrl = QString("http://music.mc-x.com/music/%1?h=on").arg(musicInfo->musicID);
    else
        musicUrl = QString("http://music.mc-x.com/music/%1").arg(musicInfo->musicID);
    //qDebug()<<url<<endl;
    htmlGetter = new HtmlGetter(QUrl(musicUrl), musicInfo, this);
    connect(htmlGetter, &HtmlGetter::errorOccurred, this, &HandlerWithHtml::handleError);
    connect(htmlGetter, &HtmlGetter::signalHtmlGotten, this, &HandlerWithHtml::onHtmlGotten);
    htmlGetter->start();
}

void HandlerWithHtml::onHtmlGotten(QString html)
{
    if(htmlHandler != nullptr)
    {
        delete htmlHandler;
        htmlHandler = nullptr;
    }
    htmlHandler = new HtmlHandler(html, musicInfo, this);
    QString error = htmlHandler->parse();
    if(error != "")
    {
        emit signalError(error);
    }
    musicInfoGotten();
}

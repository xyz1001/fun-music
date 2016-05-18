#include "handlerwithhtml.h"

HandlerWithHtml::HandlerWithHtml(QObject *parent):DataGetter(parent)
{

}

void HandlerWithHtml::getRawInfo(QString song_id)
{
    if(!htmlGetter)
    {
        delete htmlGetter;
        htmlGetter = nullptr;
    }
    if(!htmlHandler)
    {
        delete htmlHandler;
        htmlHandler = nullptr;
    }

    //歌曲查询链接拼接
    QString music_url = "http://www.loadfield.com/163/song.php?id=" + song_id;
    //qDebug()<<url<<endl;
    htmlGetter = new HtmlGetter(QUrl(music_url),this);
    connect(htmlGetter, &HtmlGetter::signalsError, this, &HandlerWithHtml::signalError);
    //当原始数据获取完成后进行数据解析
    connect(htmlGetter, &HtmlGetter::signalHtmlGotten, this, &HandlerWithHtml::onRawInfoGotten);
    //开始获取Html
    htmlGetter->getHtml();
}

void HandlerWithHtml::parseRawInfo()
{
    if(htmlGetter->getHtmlString().isEmpty())
    {
        delete htmlGetter;
        htmlGetter = nullptr;
        return;
    }
    htmlHandler = new HtmlHandler(htmlGetter->getHtmlString(), music_info, this);
    connect(htmlHandler, &HtmlHandler::signalMusicInfoGotten, this, &HandlerWithHtml::signalMusicInfoGotten);
    connect(htmlHandler, &HtmlHandler::signalHtmlHandlerError, this, &HandlerWithHtml::signalError);
    //开始解析原始数据，获取歌曲信息
    htmlHandler->getMusicInfo();
}

void HandlerWithHtml::onRawInfoGotten()
{
    parseRawInfo();
}


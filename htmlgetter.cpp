#include "htmlgetter.h"
#include <QDebug>
#include "constant.h"

HtmlGetter::HtmlGetter(QUrl url, MusicInfo *musicInfo, QObject *parent) :
    NetworkAccess(url, parent)
{
    this->musicInfo = musicInfo;
    timer.setInterval(5000);
}

HtmlGetter::~HtmlGetter()
{
    if(reply != nullptr)
    {
        delete reply;
        reply = nullptr;
    }}

void HtmlGetter::sendRequest()
{
    ++retryTime;
    if(reply != nullptr)
    {
        delete reply;
        reply = nullptr;
    }
    //发送请求并获取应答数据
    reply=networkManager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &HtmlGetter::onReplyFinished);
    timer.start();
}

void HtmlGetter::start()
{
    sendRequest();
}

void HtmlGetter::onReplyFinished()
{
    timer.stop();
    //应答完成后将应答数据存入htmlString
    if(reply->error() == QNetworkReply::NoError)
    {
        html=QString::fromUtf8(reply->readAll());
        emit signalHtmlGotten(html);
    }
    else if(retryTime < 3)
    {
        disconnect(reply, &QNetworkReply::finished, this, &HtmlGetter::onReplyFinished);
        start();
    }
    else
    {
        emit errorOccurred(tr("Netword error!"), HTML_GETTER_ERROR);
    }
}

void HtmlGetter::onTimeOut()
{
    if(retryTime >= 3)
    {
        emit errorOccurred(tr("Netword error!"), HTML_GETTER_ERROR);
    }
    else
    {
        disconnect(reply, &QNetworkReply::finished, this, &HtmlGetter::onReplyFinished);
        start();
    }
}

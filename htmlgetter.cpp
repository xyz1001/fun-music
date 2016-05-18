#include "htmlgetter.h"
#include <QDebug>

HtmlGetter::HtmlGetter(QUrl url, QObject *parent) :
    QObject(parent)
{
    this->url=url;
    manager=new QNetworkAccessManager(this);
}

QString HtmlGetter::getHtmlString()
{
    //返回获取到的Html数据
    if(htmlString.isEmpty())
    {
        emit signalsError(tr("Fail to parsing!"));
        return nullptr;
    }
    return htmlString;
}

void HtmlGetter::getHtml()
{
    if(url.isEmpty())
    {
        emit signalsError(tr("Url is empty!"));
        return;
    }
    //发送请求并获取应答数据
    reply=manager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &HtmlGetter::htmlReplyFinished);
}

void HtmlGetter::htmlReplyFinished()
{
    //应答完成后将应答数据存入htmlString
    if(reply->error() == QNetworkReply::NoError)
        htmlString=reply->readAll();
    reply->deleteLater();
    //发送Html原始数据获取信号
    emit signalHtmlGotten();
}


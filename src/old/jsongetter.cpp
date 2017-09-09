#include "jsongetter.h"
#include "constant.h"

JsonGetter::JsonGetter(QUrl url, QObject *parent)
    : NetworkAccess(url, parent)
{
    timer.setInterval(5000);
}

JsonGetter::~JsonGetter()
{
    if(reply != nullptr)
    {
        delete reply;
        reply = nullptr;
    }}

void JsonGetter::sendRequest()
{
    ++retryTime;
    QNetworkRequest request(url);
    request.setRawHeader("Referer", "http://music.163.com/");
    request.setRawHeader("Cookie", "appver=1.5.0.75771");
    if(reply != nullptr)
    {
        delete reply;
        reply = nullptr;
    }
    reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &JsonGetter::onReplyFinished);
    timer.start();
}

void JsonGetter::start()
{
    sendRequest();
}



void JsonGetter::onReplyFinished()
{
    timer.stop();
    if(reply->error() == reply->NoError)
    {
        json = QString::fromUtf8(reply->readAll());
        emit JsonGotton(json);
    }
    else if(retryTime < 3)
    {
        disconnect(reply, &QNetworkReply::finished, this, &JsonGetter::onReplyFinished);
        start();
    }
    else
    {
        emit errorOccurred(tr("Network error!"), JSON_GETTER_ERROR);
    }
}

void JsonGetter::onTimeOut()
{
    if(retryTime >= 3)
    {
        emit errorOccurred(tr("Netword error!"), JSON_GETTER_ERROR);
    }
    else
    {
        disconnect(reply, &QNetworkReply::finished, this, &JsonGetter::onReplyFinished);
        start();
    }
}




#include "jsongetter.h"
#include <QDebug>

JsonGetter::JsonGetter(QUrl url, QObject *parent)
    :QObject(parent)
{
    this->url=url;
    manager=new QNetworkAccessManager(this);
}

QString JsonGetter::getJsonString()
{
    if(jsonString.isEmpty())
    {
        emit signalsError(tr("Fail to parsing!"));
        return nullptr;
    }
    return jsonString;
}

void JsonGetter::getJson()
{
    if(url.isEmpty())
    {
        emit signalsError(tr("Url is empty!"));
        return;
    }
    reply=manager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &JsonGetter::jsonReplyFinished);
}

void JsonGetter::jsonReplyFinished()
{
    if(reply->error() == QNetworkReply::NoError)
        jsonString=reply->readAll();
    reply->deleteLater();
    emit signalJsonGotten();
}

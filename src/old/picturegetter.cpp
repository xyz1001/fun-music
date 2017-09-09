#include "picturegetter.h"
#include <QMessageBox>
#include <QDebug>

PictureGetter::PictureGetter(QUrl url, QObject *parent)
    :NetworkAccess(url, parent)
{
    pixmap = new QPixmap;
    success = false;
    timer.setInterval(20000);
}

PictureGetter::~PictureGetter()
{
    if(reply != nullptr)
    {
        delete reply;
        reply = nullptr;
    }}

QPixmap *PictureGetter::getPixmap()
{
    return pixmap;
}

void PictureGetter::sendRequest()
{
    ++retryTime;
    if(reply != nullptr)
    {
        delete reply;
        reply = nullptr;
    }
    reply=networkManager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &PictureGetter::onReplyFinished);
    timer.start();
}

void PictureGetter::start()
{
    sendRequest();
}

void PictureGetter::onReplyFinished()
{
    timer.stop();
    disconnect(reply, &QNetworkReply::finished, this, &PictureGetter::onReplyFinished);
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray datagram = reply->readAll();
        if(datagram.isEmpty() || !(pixmap->loadFromData(datagram)))
        {
            pixmap->load(":/image/pic/error.png");
        }
        //pixmap->save("pic.jpg");
        emit signalPictureDownloaded(pixmap);
    }
    else if(retryTime < 3)
    {
        start();
    }
    else
    {
        pixmap->load(":/image/pic/error.png");
        emit signalPictureDownloaded(pixmap);
    }
}

void PictureGetter::onTimeOut()
{
    disconnect(reply, &QNetworkReply::finished, this, &PictureGetter::onReplyFinished);
    if(retryTime >= 3)
    {
        pixmap->load(":/image/pic/error.png");
        emit signalPictureDownloaded(pixmap);
    }
    else
    {
        start();
    }
}





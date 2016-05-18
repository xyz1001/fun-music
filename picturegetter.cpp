#include "picturegetter.h"
#include <QMessageBox>
#include <QDebug>

PictureGetter::PictureGetter(QUrl url, QObject *parent)
    :QObject(parent)
{
    pixmap = new QPixmap;
    this->url=url;
    manager=new QNetworkAccessManager(this);
}

QPixmap *PictureGetter::getPixmap()
{
    if(pixmap==nullptr)
    {
        emit signalError(tr("No pixmap is got"));
        return nullptr;
    }
    return pixmap;
}

void PictureGetter::getPicture()
{
    reply=manager->get(QNetworkRequest(url));
    //qDebug()<<url<<endl;
    connect(reply, &QNetworkReply::finished, this, &PictureGetter::PictureReplyFinished);
}

void PictureGetter::PictureReplyFinished()
{
    if(reply->error() == QNetworkReply::NoError)
    {
        pixmap->loadFromData(reply->readAll());
        pixmap->save("pic.jpg");
        emit signalPictureDownloaded(pixmap);
        //qDebug()<<"OK"<<endl;
    }
    else
    {
        emit signalError(tr("Image downloaded failure!"));
    }
    reply->deleteLater();
}





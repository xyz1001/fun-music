#include "mp3getter.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QTime>
#include "mainwindow.h"

MP3Getter::MP3Getter(QUrl url, QString filepath, QObject *parent)
    : NetworkAccess(url, parent)
{
    this->filepath=filepath;
    timer.setInterval(10000);
}

MP3Getter::~MP3Getter()
{
    if(file != nullptr)
    {
        delete file;
        file = 0;
    }
    if(reply != nullptr)
    {
        delete reply;
        reply = nullptr;
    }
}

void MP3Getter::sendRequest()
{
    ++retryTime;
    //qDebug()<<QTime::currentTime().toString()<<": "<<"retryTime: "<<retryTime<<endl;
    if(reply != nullptr)
    {
        delete reply;
        reply = nullptr;
    }
    QNetworkRequest request(url);
    request.setRawHeader("Range", (QString("bytes=%1-").arg(BytesReceived)).toLatin1());
    reply=networkManager->get(request);
    connect(reply, &QNetworkReply::readyRead, this, &MP3Getter::MP3ReadyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &MP3Getter::MP3DownloadProgress);
    connect(reply, &QNetworkReply::finished, this, &MP3Getter::onReplyFinished);

    timer.start();
    //qDebug()<<"start\n";
}

void MP3Getter::start()
{
    file = new QFile(filepath);

    if (!file->open(QIODevice::WriteOnly))
    {
        emit errorOccurred(tr("Unable to save the file %1: %2.").arg(filepath).arg(file->errorString()),
                           MP3_GETTER_ERROR);
        delete file;
        file = nullptr;
    }
    else
    {
        sendRequest();
    }
}

void MP3Getter::MP3ReadyRead()
{
    timer.stop();
    //有数据便进行读取
    if(file)
        file->write(reply->readAll());
    timer.start();
}

void MP3Getter::MP3DownloadProgress(qint64 bytesRead, qint64 totalbytes)
{
    if(totalbytes + BytesReceived > 500)
    {
        //qDebug()<<QString("(%1,%2)").arg(bytesRead).arg(totalbytes)<<endl;
        emit signalDownloadProgress(bytesRead + BytesReceived, totalbytes + BytesReceived);
    }
}

void MP3Getter::onTimeOut()
{
    if(retryTime > 30)
    {
        //qDebug()<<QTime::currentTime().toString()<<": "<<"1\n";
        file->flush();
        file->close();
        file->remove();
        emit errorOccurred(tr("Netword error!"), MP3_GETTER_ERROR);
    }
    else
    {
        disconnect(reply, &QNetworkReply::readyRead, this, &MP3Getter::MP3ReadyRead);
        disconnect(reply, &QNetworkReply::downloadProgress, this, &MP3Getter::MP3DownloadProgress);
        disconnect(reply, &QNetworkReply::finished, this, &MP3Getter::onReplyFinished);
        reply->abort();
        file->flush();
        BytesReceived = file->size();
        sendRequest();
    }
}

void MP3Getter::onReplyFinished()
{
    //qDebug()<<QTime::currentTime().toString()<<": "<<"finished\n";
    timer.stop();
    file->flush();
    file->close();

    if (reply->error() == QNetworkReply::NoError)
    {
        emit signalMP3Downloaded();
    }
    else if(retryTime <= 30)
    {
        disconnect(reply, &QNetworkReply::finished, this, &MP3Getter::onReplyFinished);
        file->remove();
        delete file;
        file = nullptr;
        BytesReceived = 0;
        start();
    }
    else
    {
        file->remove();
        emit errorOccurred(tr("Network error!"), MP3_GETTER_ERROR);
    }
}

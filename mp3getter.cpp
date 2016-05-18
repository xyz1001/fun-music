#include "mp3getter.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include "mainwindow.h"

MP3Getter::MP3Getter(QUrl url, QString filename, QWidget *widget, QObject *parent) : QObject(parent)
{
    this->widget = widget;
    this->url=url;
    this->filename=filename;
    //qDebug()<<url<<endl;
    isAborted = false;
    manager=new QNetworkAccessManager(this);
}

void MP3Getter::getMP3()
{
    if(filenameCheck())
    {
        emit signalInfo(tr("Start downloading ") + QFileInfo(filename).fileName());
        //qDebug()<<"start downloading..."<<endl;
        isAborted=false;
        reply=manager->get(QNetworkRequest(url));
        connect(reply, &QNetworkReply::readyRead, this, &MP3Getter::MP3ReadyRead);
        connect(reply, &QNetworkReply::downloadProgress, this, &MP3Getter::MP3DownloadProgress);
        connect(reply, &QNetworkReply::finished, this, &MP3Getter::MP3replyFinished);
    }
    else
    {
        //qDebug()<<"Unable to save file" + filename<<endl;
        //emit signalError(tr("Unable to save file: ") + filename);
        return;
    }
}

bool MP3Getter::filenameCheck()
{
    //如果歌曲已存在则弹窗询问是否覆盖
    if (QFile::exists(filename)) {
        if (QMessageBox::question(widget, tr("File exists"),
                                  tr("There already exists a file called %1 in "
                                     "the current directory. Overwrite?").arg(filename),
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
            == QMessageBox::No)
            return false;
        QFile::remove(filename);
    }

    file = new QFile(filename);
    if (!file->open(QIODevice::WriteOnly)) {
        emit signalError(tr("Unable to save the file %1: %2.").arg(filename).arg(file->errorString()));
        delete file;
        file = 0;
        return false;
    }

    //qDebug()<<"Check ok!"<<endl;
    return true;
}

void MP3Getter::MP3ReadyRead()
{
    //有数据便进行读取
    if(file)
        file->write(reply->readAll());
}

void MP3Getter::cancelDownload()
{
    reply->abort();
    isAborted=true;
}

void MP3Getter::MP3DownloadProgress(qint64 bytesRead, qint64 totalbytes)
{
    if(isAborted)
        return;
    emit signalDownloadProgress(bytesRead, totalbytes);
}

void MP3Getter::MP3replyFinished()
{
    if (isAborted)
    {
        if (file)
        {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        return;
    }

    file->flush();
    file->close();

    if (reply->error())
    {
        file->remove();
        emit signalError(tr("Download failed: %1.").arg(reply->errorString()));
    }
    else
    {
        emit signalMP3Downloaded();
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
}


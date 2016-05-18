#ifndef MP3GETTER_H
#define MP3GETTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>

class MP3Getter : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QUrl url;
    QFile *file;
    QString filename;
    bool isAborted;

    QWidget *widget;
public:
    explicit MP3Getter(QUrl url, QString filename, QWidget *widget, QObject *parent = 0);

    void getMP3();
signals:
    void signalMP3Downloaded();
    void signalDownloadProgress(qint64, qint64);
    void signalInfo(QString info);
    void signalError(QString error);
public slots:
    bool filenameCheck();
    void MP3ReadyRead();
    void cancelDownload();
    void MP3DownloadProgress(qint64 bytesRead, qint64 totalbytes);
    void MP3replyFinished();
};

#endif // MP3GETTER_H

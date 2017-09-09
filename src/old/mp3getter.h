#ifndef MP3GETTER_H
#define MP3GETTER_H

#include <QFile>
#include "networkaccess.h"
#include "constant.h"

class MP3Getter : public NetworkAccess
{
    Q_OBJECT
private:
    QFile *file = nullptr;
    QString filepath;
    int musicSize = 0;
    quint64 BytesReceived = 0;

    void sendRequest();

public:
    explicit MP3Getter(QUrl url, QString filepath, QObject *parent = 0);
    ~MP3Getter();

    void start();
signals:
    void signalMP3Downloaded();
    void signalDownloadProgress(qint64, qint64);
    void signalInfo(QString info);
public slots:
    void MP3ReadyRead();
    void MP3DownloadProgress(qint64 bytesRead, qint64 totalbytes);
    void onReplyFinished();
    void onTimeOut();
};

#endif // MP3GETTER_H

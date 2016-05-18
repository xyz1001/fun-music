#ifndef PICTUREGETTER_H
#define PICTUREGETTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QPixmap>

class PictureGetter: public QObject
{
    Q_OBJECT
private:
    QPixmap *pixmap;
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

public:
    explicit PictureGetter(QUrl url, QObject *parent = 0);

    QPixmap *getPixmap();
    void getPicture();
signals:
    void signalPictureDownloaded(QPixmap *pixmap);
    void signalError(QString error);
public slots:
    void PictureReplyFinished();
};

#endif // PICTUREGETTER_H

#ifndef PICTUREGETTER_H
#define PICTUREGETTER_H

#include <QPixmap>
#include "networkaccess.h"
#include "constant.h"

class PictureGetter: public NetworkAccess
{
    Q_OBJECT
private:
    QPixmap *pixmap;
    bool success;

    void sendRequest();
public:
    explicit PictureGetter(QUrl url, QObject *parent = 0);
    ~PictureGetter();

    QPixmap *getPixmap();
    void start();
signals:
    void signalPictureDownloaded(QPixmap *pixmap);
public slots:
    void onReplyFinished();
    void onTimeOut();
};

#endif // PICTUREGETTER_H

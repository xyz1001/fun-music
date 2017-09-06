#ifndef JSONGETTER_H
#define JSONGETTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QTimer>
#include "networkaccess.h"

class JsonGetter : public NetworkAccess
{
    Q_OBJECT
private:
    QString json = nullptr;

    void sendRequest();
public:
    explicit JsonGetter(QUrl url, QObject *parent = 0);
    ~JsonGetter();

    void start();
signals:
    void JsonGotton(QString json);
private slots:
    void onReplyFinished();
    void onTimeOut();
};

#endif // JSONGETTER_H

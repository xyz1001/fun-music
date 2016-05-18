#ifndef JSONGETTER_H
#define JSONGETTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

class JsonGetter : public QObject
{
    Q_OBJECT
private:
    QString jsonString;
    QUrl url;

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
public:
    explicit JsonGetter(QUrl url, QObject *parent = 0);

    QString getJsonString();
    void getJson();
signals:
    void signalsError(QString error);
    void signalJsonGotten();
public slots:
    void jsonReplyFinished();
};

#endif // JSONGETTER_H

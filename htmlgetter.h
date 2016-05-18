#ifndef HTMLGETTER_H
#define HTMLGETTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

class HtmlGetter : public QObject
{
    Q_OBJECT
private:
    QString htmlString; //获取到的Html数据
    QUrl url;   //请求链接

    QNetworkAccessManager *manager; //网络管理
    QNetworkReply *reply;   //应答
public:
    explicit HtmlGetter(QUrl url, QObject *parent = 0);

    QString getHtmlString();
    void getHtml();
signals:
    void signalsError(QString error);
    void signalHtmlGotten();
public slots:
    void htmlReplyFinished();
};

#endif // HTMLGETTER_H

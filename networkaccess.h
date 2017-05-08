#ifndef NETWORKACCESS_H
#define NETWORKACCESS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QTimer>

class NetworkAccess : public QObject
{
    Q_OBJECT
protected:
    explicit NetworkAccess(QUrl url, QObject *parent = 0);

    QNetworkAccessManager *networkManager = nullptr;    //网络接入管理
    QNetworkReply *reply = nullptr; //请求响应
    QTimer timer;   //定时器,用于超时重试
    QUrl url;
    int retryTime = 0;  //重试次数


    virtual void sendRequest() = 0; //发送请求

public:
    virtual void start() = 0;   //开始执行

signals:
    void errorOccurred(QString error, int errorType);   //错误信号

protected slots:
    virtual void onReplyFinished() = 0; //请求应答结束
    virtual void onTimeOut() = 0;   //超时
};

#endif // NETWORKACCESS_H

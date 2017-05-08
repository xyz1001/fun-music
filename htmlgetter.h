#ifndef HTMLGETTER_H
#define HTMLGETTER_H

#include <QObject>
#include "networkaccess.h"
#include "constant.h"
#include "musicinfo.h"

class HtmlGetter : public NetworkAccess
{
    Q_OBJECT
private:
    QString html; //获取到的Html数据

    MusicInfo *musicInfo;

    void sendRequest();
public:
    explicit HtmlGetter(QUrl url, MusicInfo *musicInfo, QObject *parent = 0);
    ~HtmlGetter();
    void start();

signals:
    void signalError(QString error);
    void signalHtmlGotten(QString html);
public slots:
    void onReplyFinished();
    void onTimeOut();
};

#endif // HTMLGETTER_H

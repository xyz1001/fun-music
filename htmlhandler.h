#ifndef HTMLHANDLER_H
#define HTMLHANDLER_H

#include <QObject>
#include <QMap>

class HtmlHandler : public QObject
{
    Q_OBJECT
private:
    QString html_content;
    QMap<QString, QString> *music_info;
    //获取歌曲信息
    void getInfomation();
    //获取歌曲链接
    void getLinks();
public:
    explicit HtmlHandler(QString html_content, QMap<QString, QString> *music_info, QObject *parent = 0);

    void getMusicInfo();
signals:
    //信息获取完成
    void signalMusicInfoGotten(QMap<QString, QString> *music_info);
    void signalHtmlHandlerError(QString error);
};

#endif // HTMLHANDLER_H

#ifndef CLIPBOARDMONITOR_H
#define CLIPBOARDMONITOR_H

#include <QObject>
#include <QClipboard>
#include <QRegExp>

class ClipboardMonitor : public QObject
{
    Q_OBJECT
    QString clipboard_content;
    QString pattern;
    QRegExp rx;
    QClipboard *clipboard;
public:
    /*
     * @clipboard: 系统剪贴板指针
     */
    explicit ClipboardMonitor(QClipboard *clipboard, QObject *parent = 0);

signals:
    //当获取到新的歌曲ID发送该信号
    void signalNewSongIDCatched(QString song_id);
public slots:
    //响应剪贴板内容变化事件
    void onClipboardChanged();
};

#endif // CLIPBOARDMONITOR_H

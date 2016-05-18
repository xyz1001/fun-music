#include "clipboardmonitor.h"
#include <QDebug>

ClipboardMonitor::ClipboardMonitor(QClipboard *clipboard, QObject *parent) : QObject(parent)
{
    //提取歌曲ID的正则表达式，可以匹配桌面和网页端歌曲链接
    pattern="music\\.163\\.com/.*song\\?id=(\\d{1,12})";
    rx.setPattern(pattern);
    this->clipboard = clipboard;
}

//获取剪贴板内容且内容成功匹配为歌曲链接则提取歌曲ID并发送
void ClipboardMonitor::onClipboardChanged()
{
    //qDebug()<<"Clipboard changed!"<<endl;
    //获取剪贴板内容
    clipboard_content = clipboard->text();
    //qDebug()<<clipboard_content<<endl;
    //内容不为空且正则匹配成功时
    if(!clipboard_content.isEmpty())
    {
        //qDebug()<<"Not Null"<<endl;
        if(rx.indexIn(clipboard_content) >= 0)
        {
            //QString song_id = rx.cap(0);
            //qDebug()<<song_id;
            emit signalNewSongIDCatched(rx.cap(1));
        }
    }
}

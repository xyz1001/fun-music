#ifndef DATAGETTER_H
#define DATAGETTER_H

#include <QObject>
#include <QMap>
#include <QPixmap>
#include "jsongetter.h"
#include "jsonhandler.h"
#include "htmlgetter.h"
#include "htmlhandler.h"
#include "picturegetter.h"
#include "mp3getter.h"
#include "options.h"

class DataGetter : public QObject
{
    Q_OBJECT
public:
    QMap<QString, QString> *music_info; //歌曲信息
    QString raw_data;   //原始歌曲信息内容
    JsonGetter *jsonGetter; //获取json
    JsonHandler *jsonHandler;   //处理json
    HtmlGetter *htmlGetter; //获取Html
    HtmlHandler *htmlHandler;   //处理Html
    PictureGetter *pictureGetter;   //获取图片
    MP3Getter *mp3Getter;   //获取mp3
public:
    explicit DataGetter(QObject *parent = 0);
    ~DataGetter();
    /*
     * Description: 纯虚函数，根据歌曲ID拼接出原始数据获取链接并获取数据
     */
    virtual void getRawInfo(QString song_id) = 0;
    /*
     * Description: 纯虚函数，解析原始数据
     */
    virtual void parseRawInfo() = 0;
    /*
     * Description: 下载图片
     */
    virtual void getPicture(QUrl picUrl);
    /*
     * Description: 下载歌曲
     * Input: @mp3Url: 歌曲下载地址
     *        @filepath: 歌曲保存目录
     *        @widget: widget对象，用来作为MP3Getter中对话框的父组件
     */
    virtual void downloadMP3(QUrl mp3Url, QString filepath, QWidget *widget);
signals:
    void signalInfo(QString info);
    void signalError(QString error);
    void signalDownloadProgress(qint64 bytesRead, qint64 totalBytes);
    void signalMusicInfoGotten(QMap<QString, QString> *music_info);
    void signalPictureGotten(QPixmap *pixmap);
    void signalMP3downloaded();
public slots:
    virtual void onRawInfoGotten() = 0;
};

#endif // DATAGETTER_H

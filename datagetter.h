#ifndef DATAGETTER_H
#define DATAGETTER_H

#include <QObject>
#include <QMap>
#include <QPixmap>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "picturegetter.h"
#include "mp3getter.h"
#include "musicinfo.h"
#include "constant.h"
#include "settings.h"

class DataGetter : public QObject
{
    Q_OBJECT
public:
    MusicInfo *musicInfo; //歌曲信息
    PictureGetter *pictureGetter;   //获取图片
    MP3Getter *mp3Getter;   //获取mp3

    QWidget *widget = nullptr;
    int row = 0;
    int quality = 0;
    QString downloadPath;

    inline void updateDownloadInfo();
public:
    explicit DataGetter(MusicInfo *musicInfo, QWidget *widget, QObject *parent = 0);
    virtual ~DataGetter();
    /*
     * Description: 纯虚函数，根据歌曲ID拼接出原始数据获取链接并获取数据
     */
    virtual void getMusicInfo() = 0;
    /*
     * Description: 下载图片
     */
    virtual void getPicture();
    /*
     * Description: 下载歌曲
     * Input: @mp3Url: 歌曲下载地址
     *        @filepath: 歌曲保存目录
     *        @widget: widget对象，用来作为MP3Getter中对话框的父组件
     */
    virtual void downloadMP3();
signals:
    void signalInfo(QString info);
    void signalTableWidgetMusicInfoUpdate(int row, int column, QString info);
    void signalError(QString error);
    void signalMusicInfoGotten(MusicInfo *musicInfo);
    void signalPictureGotten(MusicInfo *musicInfo);
    void signalFinished();
public slots:
    void startHandle();
    void musicInfoGotten();
    void onPictureGotten(QPixmap *pixmap);
    void onMp3Downloaded();
    void updateDownloadProgress(qint64 bytesRead, qint64 totalBytes);

    void handleError(QString error, int errorType);
};

#endif // DATAGETTER_H

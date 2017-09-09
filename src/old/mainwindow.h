#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QAction>
#include <QMenu>
#include <QMap>
#include <QClipboard>
#include <QQueue>
#include <QSet>
#include <QTableWidgetItem>
#include "datagetter.h"
#include "clipboardmonitor.h"
#include "settings.h"
#include "options.h"
#include "musicinfo.h"
#include "guidance.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Guidance *guidance = nullptr;

    QSystemTrayIcon *trayicon;  //托盘显示
    QMenu *trayiconMenu;    //托盘菜单

    QAction *quit;  //退出动作
    QAction *options;   //选项动作
    QAction *aboutSoftware; //关于本软件动作
    QAction *aboutMe;   //关于我动作
    QAction *help;  //帮助

    QMenu *contextMenu;
    QAction *deleteItemAction;
    QAction *openFileLocationAction;
    QAction *retryAction;
    QAction *downloadAction;
    QAction *copyToClipboardAction;
    QAction *playAction;

    QMap<QString, MusicInfo*> *musicsInfo = nullptr;  //歌曲信息表，Key：歌曲ID  Value：歌曲信息
    MusicInfo *currentMusicInfo = 0;

    //创建所需对象指针并初始化为空
    DataGetter *dataGetter = nullptr;   //用来获取数据
    QClipboard *clipboard = nullptr;    //剪切板对象
    ClipboardMonitor *clipboardMonitor = nullptr;   //负责监视剪切板
    Options *option = nullptr;    //选项
    QQueue<MusicInfo* > *queue = nullptr;   //下载队列，保存待下载的歌曲ID
    QSet<MusicInfo* > wildPointer;   //已删除的野指针
    int handlingCount = 0;  //正在处理的歌曲数量
    int MAX_HANDLING_COUNT = 5; //同时处理的歌曲数量上限

    void createAction();    //创建动作
    void createTrayIcon();  //创建托盘
    void download();  //下载歌曲
    void showMusicInfo();
    void clearMusicInfo();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);    //重载关闭函数，实现关闭主窗口是最小化到托盘
signals:
    void signalInfo(QString info);  //当有提示信息时发送该信号
    void signalError(QString error);    //当有错误信息是发送该信号

public slots:
    void onInitConfigureError(QString error);
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason); //响应托盘激活事件,@reason为激活类型
    void onOptionsTriggered();  //响应Options动作被触发事件
    void onAboutSoftwareTriggered();    //响应AboutSoftware动作被触发事件
    void onAboutMeTriggered();  //响应AboutMe动作被触发事件
    void onHelpTriggered(); //显示帮助

    void onCopyClicked();   //响应Copy按钮被点击事件
    void onDownloadClicked();   //响应Download按钮被点击事件
    void onPlayClicked();
    void startParse();   //开始解析
    void onFinished();  //处理完毕
    void onMusicInfoGotten(MusicInfo *musicInfo);
    void onPictureGotten(MusicInfo *musicInfo);
    void onTableWidgetRowSelected(int row);    //选择了歌曲列表中的一行
    void updateTableWidgetMusicInfo(int row, int column, QString info);
    void onDeleteItemClicked();
    void onOpenFileLocation();
    void onRetry();
    void onCustomContextMenuRequested(QPoint pos);
    void onDownloadAllClicked();
    void onRetryAllClicked();
    void onClearAllClicked();
    /**
     * Description：响应在剪切板中获取到了新的歌曲ID事件，开始解析歌曲ID
     * input：@song_id：字符串类型的歌曲ID
     */
    void onNewSongIDCatchedFromClipboard(QString songID);

    /**
     * Description：响应显示提示信息事件
     */
    void showInfo(QString info);
    /*
     * Description：处理错误信息事件
     */
    void HandleError(QString error);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QAction>
#include <QMenu>
#include <QMap>
#include <QClipboard>
#include "datagetter.h"
#include "clipboardmonitor.h"
#include "settings.h"
#include "options.h"

namespace Ui {
class AndroidMainWindow;
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QSystemTrayIcon *trayicon;  //托盘显示
    QMenu *trayiconMenu;    //托盘菜单

    QAction *quit;  //退出动作
    QAction *options;   //选项动作
    QAction *aboutSoftware; //关于本软件动作
    QAction *aboutMe;   //关于我动作

    bool isDirectDownload;  //手动输入歌曲ID时是否是解析后立即下载
    bool isAlwaysDirectDownload;    //监视剪切板获取到歌曲ID时，解析后是否自动下载
    bool isAutoHide;    //启动时是否自动隐藏
    QString musicDownloadDir;   //音乐下载目录
    bool isFromClipboard;   //链接是否来自剪切板
    //歌曲信息，包括歌曲名name，歌曲链接songUrl，歌手名artists，歌手链接artistsUrl
    //专辑名album，专辑链接albumUrl，歌曲下载地址mp3Url，图片下载地址picUrl
    QMap<QString, QString> *music_info;

    //创建所需对象指针并初始化为空
    DataGetter *dataGetter = nullptr;   //用来获取数据
    QClipboard *clipboard = nullptr;    //剪切板对象
    ClipboardMonitor *clipboardMonitor = nullptr;   //负责监视剪切板
    Settings *settings = nullptr;   //设置
    Options *option;    //选项

    /*
     * Description：创建动作
     */
    void createAction();
    /*
     * Description：创建托盘
     */
    void createTrayIcon();
    /*
     * Description：检查歌曲ID
     * output：验证是否通过
     */
    bool checkSongID(QString song_id);
    /*
     * Description：检查下载地址
     * output：验证是否通过
     */
    bool checkDownloadAddress();
    /*
     * Description：清空主窗口上的歌曲信息
     */
    void clearInfo();
    /*
     * Description：解析
     */
    void parse();
    /*
     * Description：下载歌曲
     * Input：@musicDownloadDirectory 字符串类型的音乐下载目录
     */
    void download(QString musicDownloadDirectory);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /*
     * Description：重载关闭函数，实现关闭主窗口是最小化到托盘
     */
    void closeEvent(QCloseEvent *event);
    /*
     * description：获取是否启动自动隐藏
     * output：返回是否启动时自动隐藏主窗口
     */
    bool getIsAutoHide() {return isAutoHide;}
signals:
    /*
     * Description：当有提示信息时发送该信号
     * input：字符串类型的提示信息
     */
    void signalInfo(QString info);
    /*
     * Description：当有错误信息是发送该信号
     * input：字符串类型的错误信息
     */
    void signalError(QString error);
public slots:
    /*
     * Description：响应托盘激活事件
     * input：@reason：QSystemTrayIcon中预定义的激活类型
     */
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    /*
     * Description：响应Options动作被触发事件
     */
    void onOptionsTriggered();
    /*
     * Description：响应AboutSoftware动作被触发事件
     */
    void onAboutSoftwareTriggered();
    /*
     * Description：响应AboutMe动作被触发事件
     */
    void onAboutMeTriggered();

    /*
     * Description：响应Parse按钮被点击事件
     */
    void onParseClicked();
    /*
     * Description：响应Parse and Download按钮被点击事件
     */
    void onParseAndDownloadClicked();
    /*
     * Description：响应Clear按钮被点击事件
     */
    void onClearClicked();
    /*
     * Description：响应Copy按钮被点击事件
     */
    void onCopyClicked();
    /*
     * Description：响应Download按钮被点击事件
     */
    void onDownloadClicked();
    /*
     * Description：响应DownloadTo按钮被点击事件
     */
    void onDownloadToClicked();


    /*
     * Description：响应已获取到歌曲信息事件，将歌曲信息展示到主窗口上并开始加载图片和根据设置下载音乐
     * input：@music_info：指向包含歌曲信息的QMap对象的指针，该指针在DataGetter中分配空间
     */
    void onMusicInfoGotten(QMap<QString, QString> *music_info);
    /*
     * Description：响应图片已下载成功事件，将图片显示在主窗口中
     * input：@pixmap：已下载的图片，在PictureGetter中分配空间
     */
    void onPictureGotten(QPixmap *pixmap);
    /*
     * Description：响应音乐下载成功事件，显示相应信息
     */
    void onMP3Downloaded();
    /*
     * Description：响应下载进度事件，在主界面更新下载进度
     */
    void onDownloadProgress();
    /*
     * Description：响应在剪切板中获取到了新的歌曲ID事件，开始解析歌曲ID
     * input：@song_id：字符串类型的歌曲ID
     */
    void onNewSongIDCatchedFromClipboard(QString song_id);
    /*
     * Description：响应设置改变事件，同步修改设置变量并保存至本地配置文件
     * input: @musicSaveDir：QString类型的音乐下载目录
     *        @autoDownload: 是否自动下载
     *        @autoHide: 是否自动隐藏
     */
    void onSettingsChanged(QString musicSaveDir, bool autoDownload, bool autoHide);

    /*
     * Description：响应显示提示信息事件
     * input：@info: 要显示的提示信息
     */
    void showInfo(QString info);
    /*
     * Description：响应显示错误信息事件
     * input：@error: 要显示的错误信息
     */
    void showError(QString error);
private:
    Ui::MainWindow *ui;
    //Ui::AndroidMainWindow *ui;    //Android界面 
};

#endif // MAINWINDOW_H

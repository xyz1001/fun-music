#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_androidmainwindow.h"
#include <QIcon>
#include <QMessageBox>
#include <QDebug>
#include <cctype>
#include <QDir>
#include "handlerwithjson.h"
#include "handlerwithhtml.h"

//TODO 增加下载队列

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  //设置安卓UI
  //ui(new Ui::AndroidMainWindow)
{
    ui->setupUi(this);

    //载入本地设置
    settings = new Settings(this);
    connect(settings, &Settings::signalSettingsError, this, &MainWindow::showError);
    settings->loadSettings(musicDownloadDir, isAlwaysDirectDownload, isAutoHide);
    //设置判断条件初始状态
    //来自剪切板为假
    isFromClipboard = false;
    //手动输入歌曲ID解析后不直接下载
    isDirectDownload = false;

    //创建动作
    createAction();
    //创建托盘
    createTrayIcon();
    //设置大小固定
    setFixedSize(size());
    //设置窗口名称
    setWindowTitle(tr("163 Music Assistant"));

    //为控件添加名称（本来是为了方便翻译，其实UI文件中的字符串也可以被识别为可翻译字符串）
    ui->menu_file->setTitle(tr("&File"));   //文件
    ui->menu_setting->setTitle(tr("&Setting")); //设置
    ui->menu_about->setTitle(tr("&About")); //关于
    ui->labelMusicName->setText(tr("Music: ")); //音乐名：
    ui->labelSingerName->setText(tr("Artist: "));   //歌手名：
    ui->labelAlbumName->setText(tr("Album: ")); //专辑名：
    ui->labelInputSongID->setText(tr("Input the Music ID: "));  //手动输入歌曲ID：
    ui->labelDownloadAddress->setText(tr("Download Address: "));    //下载地址：
    ui->pushButtonParse->setText(tr("Parse"));  //解析
    ui->pushButtonParseAndDownload->setText(tr("Parse and Download"));  //解析并下载
    ui->pushButtonClear->setText(tr("Clear"));  //清空
    ui->pushButtonCopy->setText(tr("Copy"));    //复制
    ui->pushButtonDownload->setText(tr("Download"));    //下载
    ui->pushButtonDownloadTo->setText(tr("Download to..."));    //下载到...

    //为窗口菜单添加动作
    ui->menu_file->addAction(quit);
    ui->menu_setting->addAction(options);
    ui->menu_about->addAction(aboutSoftware);
    ui->menu_about->addAction(aboutMe);

    //关联窗口控件信号及动作
    connect(ui->pushButtonParse, &QPushButton::clicked, this, &MainWindow::onParseClicked); //点击“解析”按钮时
    //点击“解析”并下载按钮时
    connect(ui->pushButtonParseAndDownload, &QPushButton::clicked, this, &MainWindow::onParseAndDownloadClicked);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &MainWindow::onClearClicked); //点击“清空”按钮时
    connect(ui->pushButtonCopy, &QPushButton::clicked, this, &MainWindow::onCopyClicked);   //点击“复制按钮”时
    connect(ui->pushButtonDownload, &QPushButton::clicked, this, &MainWindow::onDownloadClicked);   //点击“下载”按钮时
    //点击“下载到...”按钮时
    connect(ui->pushButtonDownloadTo, &QPushButton::clicked, this, &MainWindow::onDownloadToClicked);

    //设置剪贴板监控
    clipboard = QApplication::clipboard();
    clipboardMonitor = new ClipboardMonitor(clipboard, this);
    //关联剪贴板内容改变事件及对应处理动作
    connect(clipboard, &QClipboard::dataChanged, clipboardMonitor, &ClipboardMonitor::onClipboardChanged);
    //关联剪贴板改变内容为包含网易云音乐歌曲id链接事件和对应处理动作
    connect(clipboardMonitor, &ClipboardMonitor::signalNewSongIDCatched, this,
            &MainWindow::onNewSongIDCatchedFromClipboard);

    //关联程序运行信息和错误事件及气泡显示动作
    connect(this, &MainWindow::signalInfo, this, &MainWindow::showInfo);
    connect(this, &MainWindow::signalError, this, &MainWindow::showError);

    //选项窗口
    option = new Options(this);
    option->hide();
    //qDebug()<<musicDownloadDir<<endl;
    option->loadSettings(musicDownloadDir, isAlwaysDirectDownload, isAutoHide);
    //设置改变时进行相应处理
    connect(option, &Options::signalUpdateSettings, this, &MainWindow::onSettingsChanged);

    //启动自动隐藏时显示气泡
    if(isAutoHide)
        emit signalInfo(tr("163 Music Download Assistant is running background..."));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createAction()
{
    //退出
    quit = new QAction(tr("&Quit"), this);
    connect(quit, &QAction::triggered, qApp, &QApplication::quit);
    //选项
    options = new QAction(tr("&Options"), this);
    connect(options,&QAction::triggered, this, &MainWindow::onOptionsTriggered);
    //关于本软件
    aboutSoftware = new QAction(tr("About &software"), this);
    connect(aboutSoftware, &QAction::triggered, this, &MainWindow::onAboutSoftwareTriggered);
    //关于我
    aboutMe = new QAction(tr("About &me"), this);
    connect(aboutMe, &QAction::triggered, this, &MainWindow::onAboutMeTriggered);
}

void MainWindow::createTrayIcon()
{
    trayicon = new QSystemTrayIcon(this);
    //创建QIcon对象，参数是图标资源，值为项目的资源文件中图标的地址
    QIcon icon(":/image/163MusicAssistant.png");

    trayiconMenu = new QMenu(this);
    //为托盘菜单添加菜单项
    trayiconMenu->addAction(quit);
    //为托盘菜单添加分隔符
    trayiconMenu->addSeparator();

    //将创建的QIcon对象作为系统托盘图标
    trayicon->setIcon(icon);
    //显示托盘图标
    trayicon->show();
    //设置系统托盘提示
    trayicon->setToolTip(tr("163 Music Download Assistant"));
    //将创建菜单作为系统托盘菜单
    trayicon->setContextMenu(trayiconMenu);

    //为系统托盘绑定单击信号的槽 即图标激活时
    connect(trayicon, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayIconClicked);
}

bool MainWindow::checkSongID(QString song_id)
{
    //如果歌曲ID为空返回验证失败
    if(song_id.isEmpty())
    {
        //提示“请先输入歌曲ID或包含歌曲ID的链接”
        emit signalError("Please input song ID or link contained song Id first!");
        return false;
    }

    //如果歌曲ID中包括非数字，返回验证失败
    for(auto i:song_id)
    {
        if(!(i.isDigit()))
        {
            //提示“请输入正确的歌曲ID”
            emit signalError("Please input right song ID!");
            return false;
        }
    }
    return true;
}

bool MainWindow::checkDownloadAddress()
{
    //如果下载地址为空，提示“地址为空”并返回验证失败
    if(ui->lineEditDownloadAddress->text().isEmpty())
    {
        emit signalError(tr("The download link is empty"));
        return false;
    }
    return true;
}

void MainWindow::parse()
{
    //如果dataGetter对象已存在，则将其删除
    if(dataGetter)
    {
        delete dataGetter;
        dataGetter = 0;
    }
    //如果歌曲ID验证通过
    if(checkSongID(ui->lineEditSongID->text()))
    {
        //提示正在解析中
        emit signalInfo(tr("Parsing..."));
        //通过获取www.loadfield.com/163/song.php上Html信息进行解析
        dataGetter = new HandlerWithHtml(this);
        //通过获取Json进行解析
        //dataGetter = new HandlerWithJson(this);

        //关联歌曲信息获取完成信号和槽
        connect(dataGetter, &DataGetter::signalMusicInfoGotten, this, &MainWindow::onMusicInfoGotten);
        //关联歌曲图片获取完成信号和槽
        connect(dataGetter, &DataGetter::signalPictureGotten, this, &MainWindow::onPictureGotten);
        //关联歌曲下载进度信号和槽
        connect(dataGetter, &DataGetter::signalDownloadProgress, this, &MainWindow::onDownloadProgress);
        //关联MP3下载完成信号和槽
        connect(dataGetter, &DataGetter::signalMP3downloaded, this, &MainWindow::onMP3Downloaded);
        //关联提示信息信号和槽
        connect(dataGetter, &DataGetter::signalInfo, this, &MainWindow::showInfo);
        //关联错误信息和槽
        connect(dataGetter, &DataGetter::signalError, this, &MainWindow::showError);
        //qDebug()<<ui->lineEditSongID->text();
        //开始获取原始数据信息
        dataGetter->getRawInfo(ui->lineEditSongID->text());
    }
}

void MainWindow::download(QString musicDownloadDirectory)
{
    //qDebug()<<musicDownloadDirectory<<endl;
    //如果下载地址验证通过，开始下载mp3
    if(checkDownloadAddress())
    {
        dataGetter->downloadMP3(ui->lineEditDownloadAddress->text(),    //下载地址文本框内的下载地址
                                     musicDownloadDirectory + (*(this->music_info))["name"] +".mp3",    //音乐文件保存目录+歌曲名+文件后缀.mp3
                                     this);
    }
   //else
        //qDebug()<<"errer"<<endl;
}

void MainWindow::clearInfo()
{
    //清空歌曲ID输入框由clear动作函数自行执行，避免再次点击parse按钮时也会清空歌曲ID输入框
    //清空音乐名和提示
    ui->labelMusic->clear();
    ui->labelMusic->setStatusTip("");
    //清空歌手名和提示
    ui->labelSinger->clear();
    ui->labelSinger->setStatusTip("");
    //清空专辑名和提示
    ui->labelAlbum->clear();
    ui->labelAlbum->setStatusTip("");
    //清空图片
    ui->labelPicture->clear();
    //清空下载地址和提示
    ui->lineEditDownloadAddress->clear();
    ui->lineEditDownloadAddress->setStatusTip("");
}

void MainWindow::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    //托盘点击方式
    switch(reason)
    {
    //单击
    case QSystemTrayIcon::Trigger:
        //双击
    case QSystemTrayIcon::DoubleClick:
        //恢复窗口显示
        this->setWindowState(Qt::WindowActive);
        this->show();
        break;
    default:
        break;
    }
}

void MainWindow::onOptionsTriggered()
{
    //显示选项窗口
    option->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //如果窗口可见
    if(trayicon->isVisible())
    {
        //隐藏窗口
        hide();
        //提示后台运行信息
        emit signalInfo(tr("163 Music Download Assistant is running background..."));
        //忽略关闭事件
        event->ignore();
    }
}

void MainWindow::onAboutSoftwareTriggered()
{
    //关于本软件中介绍信息
    QString introduce = tr("Parse the download addresss of musics on 163 Music, even removed musics!\n"
                           "only for study, prohibit commercial use!\n"
                           "otherwise it has nothing to do with me."
                           "Copyright reserved\n");
    //弹出“关于本软件”关于窗口
    QMessageBox::about(this, tr("AboutSoftware"),
                       introduce);
}

void MainWindow::onAboutMeTriggered()
{
    //“关于我”介绍信息
    QString introduce = tr("Zix\n"
                           "My Blog: http://www.xyz520.cn\n"
                           "Welcome to visit");
    //弹出“关于我”关于窗口
    QMessageBox::about(this, tr("AboutMe"), introduce);
}


void MainWindow::onParseClicked()
{
    //清空先前信息
    clearInfo();
    //解析
    parse();
}

void MainWindow::onParseAndDownloadClicked()
{
    //开启解析后直接下载
    isDirectDownload = true;
    //清空先前信息
    clearInfo();
    //解析
    parse();
}

void MainWindow::onClearClicked()
{
    //清空歌曲ID输入框
    ui->lineEditSongID->clear();
    //清除歌曲信息
    clearInfo();
}

void MainWindow::onCopyClicked()
{
    //下载地址验证通过时
    if(checkDownloadAddress())
    {
        //复制下载地址框中下载地址至系统剪贴板
        clipboard->setText(ui->lineEditDownloadAddress->text());
        //显示提示信息
        emit signalInfo(tr("Copyed to the system clipboard"));
    }
}

void MainWindow::onDownloadClicked()
{
    //下载
   download(musicDownloadDir);
}

void MainWindow::onDownloadToClicked()
{
    //弹出文件保存位置对话框并下载至选定的位置
    download(option->chooseMusicDownloadDir());
}

void MainWindow::onNewSongIDCatchedFromClipboard(QString song_id)
{
    if(checkSongID(song_id) && song_id != ui->lineEditSongID->text())
    {
        clearInfo();
        ui->lineEditSongID->setText(song_id);
        parse();
        isFromClipboard = true;
    }
}

void MainWindow::onMusicInfoGotten(QMap<QString, QString> *music_info)
{
    //保存获取的歌曲信息表
    this->music_info = music_info;

    //依次将获取的歌曲信息在主窗口展示出来
    //qDebug()<<(*music_info)["songUrl"];
    //显示音乐名并设置超链接
    ui->labelMusic->setText((*music_info)["name"]);
    //开启链接可用
    ui->labelMusic->setOpenExternalLinks(true);
    //如果获取到了别名则在音乐名后添加音乐别名
    if((*music_info).contains("alias") && !(*music_info)["alias"].isEmpty())
        ui->labelMusic->setText(ui->labelMusic->text() + "("
                                + (*music_info)["alias"] + ")");
    ui->labelMusic->setStatusTip(ui->labelMusic->text());
    ui->labelMusic->setText("<a href=\"" + (*music_info)["songUrl"] + "\">"
                            + ui->labelMusic->text()
                            + "</a>");
    //qDebug()<<ui->labelMusic->text();
    //显示歌手名并设置超链接
    ui->labelSinger->setOpenExternalLinks(true);
    ui->labelSinger->setText("<a href=\"" + (*music_info)["artistsUrl"] + "\">"
                            + (*music_info)["artists"]
                            + "</a>");
    ui->labelSinger->setStatusTip((*music_info)["artists"]);
    //显示专辑名并设置超链接
    ui->labelAlbum->setOpenExternalLinks(true);
    ui->labelAlbum->setText("<a href=\"" + (*music_info)["albumUrl"] + "\">"
            + (*music_info)["album"]
            + "</a>");
    ui->labelAlbum->setStatusTip((*music_info)["album"]);
    //显示下载地址
    ui->lineEditDownloadAddress->setText((*music_info)["mp3Url"]);
    ui->lineEditDownloadAddress->setStatusTip(ui->lineEditDownloadAddress->text());

    //开始下载歌曲图片
    dataGetter->getPicture((*music_info)["picUrl"]);

    //如果歌曲ID来自剪切板且开启了自动下载，开始下载音乐
    if(isFromClipboard)
    {
        //重置“来自剪切板”
        isFromClipboard = false;
        if(isAlwaysDirectDownload)
        {
            download(musicDownloadDir);
        }
    }
    //如果当前为解析并直接下载，开始下载音乐
    else if(isDirectDownload)
    {
        //重置“立即下载”
        isDirectDownload = false;
        download(musicDownloadDir);
    }
}

void MainWindow::onPictureGotten(QPixmap *pixmap)
{
    //当图片下载完成后将图片显示在主窗口，并调整其大小为图片显示空间大小
    ui->labelPicture->setPixmap((*pixmap).scaled(ui->labelPicture->size()));
}

void MainWindow::onDownloadProgress()
{
    //TODO 显示下载进度
}

void MainWindow::onMP3Downloaded()
{
    //当音乐下载完成时提示“成功下载信息”
    emit signalInfo((*(this->music_info))["name"] +".mp3" + tr(" download successfully!"));
}

void MainWindow::onSettingsChanged(QString musicSaveDir, bool autoDownload, bool autoHide)
{
    //当修改设置时修改设置变量
    //TODO 将设置变量整合至Options类中，避免出现多处设置变量
    this->musicDownloadDir = musicSaveDir;
    this->isAlwaysDirectDownload = autoDownload;
    this->isAutoHide = autoHide;
    //将设置同步保存至本地配置文件
    settings->saveSettings(musicSaveDir, autoDownload, autoHide);
}

void MainWindow::showInfo(QString info)
{
    //托盘显示提示信息，显示时间为1s（显示时间不对？）
    trayicon->showMessage(tr("163 Music Assistant"), info,
                          QSystemTrayIcon::Information, 100);
}

void MainWindow::showError(QString error)
{
    //托盘显示错误信息，显示时间为3s
    trayicon->showMessage(tr("163 Music Assistant"), error,
                          QSystemTrayIcon::Warning, 3000);
}

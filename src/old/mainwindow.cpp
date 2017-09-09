#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QMessageBox>
#include <QDebug>
#include <cctype>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QTimer>
#include <QDesktopServices>
#include "constant.h"
#include "handlerwithjson.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidgetMusicsInfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableWidgetMusicsInfo->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableWidgetMusicsInfo->setContextMenuPolicy(Qt::CustomContextMenu);

    contextMenu = new QMenu(this);
    deleteItemAction = new QAction(QIcon(":/image/icon/delete.png"), tr("Delete"), this);
    openFileLocationAction = new QAction(QIcon(":/image/icon/openfile.png"), tr("Open file location"), this);
    retryAction = new QAction(QIcon(":/image/icon/retry.png"), tr("Retry"), this);
    downloadAction = new QAction(QIcon(":/image/icon/download.png"), tr("Download"), this);
    copyToClipboardAction = new QAction(QIcon(":/image/icon/copylink.png"), tr("Copy to clipboard"), this);
    playAction = new QAction(QIcon(":/image/icon/play.png"), tr("Play"), this);

    contextMenu->addAction(playAction);
    contextMenu->addAction(retryAction);
    contextMenu->addAction(downloadAction);
    contextMenu->addAction(openFileLocationAction);
    contextMenu->addAction(copyToClipboardAction);
    contextMenu->addAction(deleteItemAction);
    connect(deleteItemAction, &QAction::triggered, this, &MainWindow::onDeleteItemClicked);
    connect(openFileLocationAction, &QAction::triggered, this, &MainWindow::onOpenFileLocation);
    connect(retryAction, &QAction::triggered, this, &MainWindow::onRetry);
    connect(downloadAction, &QAction::triggered, this, &MainWindow::download);
    connect(copyToClipboardAction, &QAction::triggered, this, &MainWindow::onCopyClicked);
    connect(playAction, &QAction::triggered, this, &MainWindow::onPlayClicked);

    connect(ui->tableWidgetMusicsInfo, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::onCustomContextMenuRequested);

    musicsInfo = new QMap<QString, MusicInfo*>;
    queue = new QQueue<MusicInfo *>;

    //显示帮助并同步“启动时显示帮助”设置
    if(!(Settings::getSettings().configs->autoHide)
            && Settings::getSettings().configs->showGuidanceOnStart)
    {
        guidance = new Guidance(true, 0);
        guidance->exec();
        delete guidance;
        guidance = nullptr;
    }

    //创建动作
    createAction();
    //创建托盘
    createTrayIcon();
    //设置大小固定
    setFixedSize(size());

    //为窗口菜单添加动作
    ui->menu_file->addAction(quit);
    ui->menu_setting->addAction(options);
    ui->menu_about->addAction(help);
    ui->menu_about->addAction(aboutSoftware);
    ui->menu_about->addAction(aboutMe);

    //关联窗口控件信号及动作
    connect(ui->pushButtonCopy, &QPushButton::clicked, this, &MainWindow::onCopyClicked);   //点击“复制按钮”时
    connect(ui->pushButtonDownload, &QPushButton::clicked, this, &MainWindow::onDownloadClicked);   //点击“下载”按钮时
    connect(ui->pushButtonPlay, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    connect(ui->pushButtonDeleteAll, &QPushButton::clicked, this, &MainWindow::onClearAllClicked);
    connect(ui->pushButtonDownloadAll, &QPushButton::clicked, this, &MainWindow::onDownloadAllClicked);
    connect(ui->pushButtonRetryAll, &QPushButton::clicked, this, &MainWindow::onRetryAllClicked);
    //点击音乐信息列表某一行时
    connect(ui->tableWidgetMusicsInfo, &QTableWidget::currentCellChanged,
            this, &MainWindow::onTableWidgetRowSelected);


    //设置剪贴板监控
    clipboard = QApplication::clipboard();
    clipboardMonitor = new ClipboardMonitor(clipboard, this);
    //关联剪贴板内容改变事件及对应处理动作
    connect(clipboard, &QClipboard::dataChanged, clipboardMonitor,
            &ClipboardMonitor::onClipboardChanged);
    //关联剪贴板改变内容为包含网易云音乐歌曲id链接事件和对应处理动作
    connect(clipboardMonitor, &ClipboardMonitor::signalNewSongIDCatched, this,
            &MainWindow::onNewSongIDCatchedFromClipboard);

    //关联程序运行信息和错误事件及气泡显示动作
    connect(this, &MainWindow::signalInfo, this, &MainWindow::showInfo);
    connect(this, &MainWindow::signalError, this, &MainWindow::HandleError);

    //选项窗口
    option = new Options(this);
    option->hide();
    option->loadConfigs();

    //启动自动隐藏时显示气泡
    if(Settings::getSettings().configs->autoHide)
    {
        //qDebug()<<"running...\n";
        emit signalInfo(tr("fun music is running background..."));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if(guidance)
        delete guidance;
}

void MainWindow::createAction()
{
    //退出
    quit = new QAction(QIcon(":/image/icon/quit.png"), tr("&Quit"), this);
    connect(quit, &QAction::triggered, qApp, &QApplication::quit);
    //选项
    options = new QAction(QIcon(":/image/icon/options.png"), tr("&Options"), this);
    connect(options,&QAction::triggered, this, &MainWindow::onOptionsTriggered);
    //关于本软件
    aboutSoftware = new QAction(QIcon(":/image/icon/about.png"), tr("About &software"), this);
    connect(aboutSoftware, &QAction::triggered, this, &MainWindow::onAboutSoftwareTriggered);
    //关于我
    aboutMe = new QAction(QIcon(":/image/icon/aboutme.png"), tr("About &me"), this);
    connect(aboutMe, &QAction::triggered, this, &MainWindow::onAboutMeTriggered);
    //帮助
    help = new QAction(QIcon(":/image/icon/help.png"), tr("&Help"), this);
    connect(help, &QAction::triggered, this, &MainWindow::onHelpTriggered);
}

void MainWindow::createTrayIcon()
{
    trayicon = new QSystemTrayIcon(this);
    //创建QIcon对象，参数是图标资源，值为项目的资源文件中图标的地址
    QIcon icon(":/image/icon/fun-music.png");

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
    trayicon->setToolTip(tr("fun music"));
    //将创建菜单作为系统托盘菜单
    trayicon->setContextMenu(trayiconMenu);

    //为系统托盘绑定单击信号的槽 即图标激活时
    connect(trayicon, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayIconClicked);
}

void MainWindow::download()
{
    QTimer::singleShot(0, currentMusicInfo->dataGetter, &DataGetter::downloadMP3);
}

void MainWindow::startParse()
{
    for(int i=0; i<MAX_HANDLING_COUNT-handlingCount && !(queue->isEmpty()); ++i)
    {
        //qDebug()<<MAX_HANDLING_COUNT<<'/'<<handlingCount<<endl;
        ++handlingCount;
        MusicInfo *musicInfo = queue->takeFirst();
        if(wildPointer.contains(musicInfo))
        {
            --handlingCount;
            QTimer::singleShot(0, this, &MainWindow::startParse);
            return;
        }
        //提示正在解析中
        //emit signalInfo(tr("Parsing..."));
        musicInfo->dataGetter = new HandlerWithJson(musicInfo, this, this);

        //处理完成
        connect(musicInfo->dataGetter, &DataGetter::signalFinished, this, &MainWindow::onFinished);
        connect(musicInfo->dataGetter, &DataGetter::signalMusicInfoGotten, this, &MainWindow::onMusicInfoGotten);
        connect(musicInfo->dataGetter, &DataGetter::signalPictureGotten, this, &MainWindow::onPictureGotten);
        //关联提示信息信号和槽
        connect(musicInfo->dataGetter, &DataGetter::signalInfo, this, &MainWindow::showInfo);
        //关联错误信息和槽
        connect(musicInfo->dataGetter, &DataGetter::signalError, this, &MainWindow::HandleError);
        //更新歌曲信息列表
        connect(musicInfo->dataGetter, &DataGetter::signalTableWidgetMusicInfoUpdate,
                this, &MainWindow::updateTableWidgetMusicInfo);
        //qDebug()<<ui->lineEditSongID->text();
        //开始获取原始数据信息
        QTimer::singleShot(0, musicInfo->dataGetter, &DataGetter::startHandle);
    }
}

void MainWindow::onFinished()
{
    --handlingCount;
    showMusicInfo();
    if(Settings::getSettings().getStatus() >= 0)
    {
        QTimer::singleShot(0, this, &MainWindow::startParse);
    }
}

void MainWindow::onMusicInfoGotten(MusicInfo *musicInfo)
{
    if(currentMusicInfo == musicInfo)
        showMusicInfo();
}

void MainWindow::onPictureGotten(MusicInfo *musicInfo)
{
    //显示图片
    if(currentMusicInfo == musicInfo)
    {
        ui->labelPicture->clear();
        ui->labelPicture->setPixmap(currentMusicInfo->pic->scaled(ui->labelPicture->size()));
    }
}

void MainWindow::onTableWidgetRowSelected(int row)
{
    clearMusicInfo();
    if(row >= 0 && row < musicsInfo->size())
    {
        currentMusicInfo = (*musicsInfo)[ui->tableWidgetMusicsInfo->item(row, 0)->text()];
        if(currentMusicInfo->status >= 1 || currentMusicInfo->status == -2)
        {
            showMusicInfo();
        }
        else
        {
            clearMusicInfo();
        }
    }
}

void MainWindow::updateTableWidgetMusicInfo(int row, int column, QString info)
{
    //qDebug()<<row<<','<<column<<':'<<info<<endl;
    ui->tableWidgetMusicsInfo->item(row, column)->setText(info);
    MusicInfo *musicInfo = (*musicsInfo)[ui->tableWidgetMusicsInfo->item(row, 0)->text()];
    if(musicInfo->status < 0)
    {
        for(int i=0; i<COLUMN_COUNT; ++i)
        {
            ui->tableWidgetMusicsInfo->item(row, i)->setTextColor(QColor(Qt::red));
        }
        musicInfo->downloadProgressBar->setTextVisible(false);
        musicInfo->downloadProgressBar->setStyleSheet("border:1px solid #FFFFFF;"
                                                      "background: red;"
                                                      "color:rgb(255,255,0);");
    }
}

void MainWindow::onDeleteItemClicked()
{
    if(currentMusicInfo->status == 2)
    {
        int quality = Settings::getSettings().configs->quality <= currentMusicInfo->downloadInfo.size()-1?
                    Settings::getSettings().configs->quality : currentMusicInfo->downloadInfo.size()-1;
        QString filepath = QString("%1/%2 - %3.%4")
                .arg(Settings::getSettings().configs->downloadDirectory)
                .arg(currentMusicInfo->artist)
                .arg(currentMusicInfo->name)
                .arg(currentMusicInfo->downloadInfo.at(quality).extension);
        QFile file(filepath);
        if(file.exists())
        {
            switch(QMessageBox::question(this, tr("Prompt"),
                                         tr("Delete the song file that is downloaded along with?"),
                                         tr("Yes"), tr("No")))
            {
            case 0:
                file.remove();
                break;
            case 1:
            default:
                break;
            }
        }
    }
    delete currentMusicInfo->dataGetter;
    delete currentMusicInfo->downloadProgressBar;
    delete currentMusicInfo->pic;
    wildPointer.insert(currentMusicInfo);
    musicsInfo->remove(QString::number(currentMusicInfo->musicID));

    int row = currentMusicInfo->row;
    delete currentMusicInfo;
    currentMusicInfo = nullptr;
    foreach (auto i, (*musicsInfo))
    {
        if(i->row > row)
        {
            --(i->row);
        }
    }
    for(int i=0; i<COLUMN_COUNT; ++i)
    {
        delete ui->tableWidgetMusicsInfo->item(row, i);
    }
    ui->tableWidgetMusicsInfo->removeRow(row);

    if(row >= ui->tableWidgetMusicsInfo->rowCount())
    {
        --row;
    }
    if(row != -1)
    {
        ui->tableWidgetMusicsInfo->selectRow(row);
        onTableWidgetRowSelected(row);
    }
}

void MainWindow::onOpenFileLocation()
{
    QDesktopServices::openUrl(QUrl(QString("file:///%1")
                                   .arg(Settings::getSettings().configs->downloadDirectory)));
}

void MainWindow::onRetry()
{
    QString musicId = QString::number(currentMusicInfo->musicID);
    onDeleteItemClicked();
    onNewSongIDCatchedFromClipboard(musicId);
}

void MainWindow::onCustomContextMenuRequested(QPoint pos)
{
    QTableWidgetItem *i = ui->tableWidgetMusicsInfo->itemAt(pos);
    if(i != 0)
    {
        contextMenu->exec(QCursor::pos());
    }
}

void MainWindow::onDownloadAllClicked()
{
    foreach(auto i , (*musicsInfo))
    {
        if(i->status == 1)
        {
            i->dataGetter->downloadMP3();
        }
    }
}

void MainWindow::onRetryAllClicked()
{
    QList<QString> musicID((*musicsInfo).keys());

    onClearAllClicked();

    foreach (auto i, musicID)
    {
        onNewSongIDCatchedFromClipboard(i);
    }
}

void MainWindow::onClearAllClicked()
{
    while(ui->tableWidgetMusicsInfo->rowCount() > 0)
    {
        for(int i=0; i<COLUMN_COUNT; ++i)
        {
            delete ui->tableWidgetMusicsInfo->item(0, i);
        }
        ui->tableWidgetMusicsInfo->removeRow(0);

    }
    while(musicsInfo->size() > 0)
    {
        if(currentMusicInfo->dataGetter != nullptr)
        {
            delete currentMusicInfo->dataGetter;
            currentMusicInfo->dataGetter = nullptr;
        }
        if(currentMusicInfo->downloadProgressBar != nullptr)
        {
            delete currentMusicInfo->downloadProgressBar;
            currentMusicInfo->downloadProgressBar = nullptr;
        }
        if(currentMusicInfo->pic != nullptr)
        {
            delete currentMusicInfo->pic;
            currentMusicInfo->pic = nullptr;
        }
        wildPointer.insert(currentMusicInfo);
        musicsInfo->remove(QString::number(currentMusicInfo->musicID));

        delete currentMusicInfo;
        currentMusicInfo = nullptr;

        currentMusicInfo = *(*musicsInfo).begin();
    }
    currentMusicInfo = nullptr;
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
        emit signalInfo(tr("fun music is running background..."));
        //忽略关闭事件
        event->ignore();
    }
}

void MainWindow::onInitConfigureError(QString error)
{
    QMessageBox::critical(this, tr("Error"), error, QMessageBox::Ok);
    exit(-1);
}

void MainWindow::onAboutSoftwareTriggered()
{
    //关于本软件中介绍信息
    QString introduce = tr("Parse the download addresss of musics , even removed musics and charge musics!\n"
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
                           "My Blog: http://www.xyz1001.xyz\n"
                           "Welcome to visit");
    //弹出“关于我”关于窗口
    QMessageBox::about(this, tr("AboutMe"), introduce);
}

void MainWindow::onHelpTriggered()
{
    if(!guidance)
        guidance = new Guidance(0);
    guidance->show();
}

void MainWindow::onCopyClicked()
{

    //复制下载地址框中下载地址至系统剪贴板
    clipboard->setText(ui->lineEditDownloadAddress->text());
    //显示提示信息
    emit signalInfo(tr("Copyed to the system clipboard"));
}

void MainWindow::onDownloadClicked()
{
    //下载
    download();
}

void MainWindow::onPlayClicked()
{
    int quality = Settings::getSettings().configs->quality <= currentMusicInfo->downloadInfo.size()-1?
                Settings::getSettings().configs->quality : currentMusicInfo->downloadInfo.size()-1;
    QString downloadPath = QString("%1%2 - %3.%4")
            .arg(Settings::getSettings().configs->downloadDirectory)
            .arg(currentMusicInfo->artist)
            .arg(currentMusicInfo->name)
            .arg(currentMusicInfo->downloadInfo.at(quality).extension);
    if(QFile::exists(downloadPath))
    {
        QDesktopServices::openUrl(QUrl(QString("file:///") + downloadPath));
    }
    else
    {
        int button = QMessageBox::warning(this, tr("File does not exist"),
                                          tr("File is missing, maybe caused by file is not downloaded,\n"
                                             " file is deleted or download path is changed.\n"
                                             " download it now?"), tr("yes"),tr("no"), 0);
        if(button == 0)
        {
            download();
        }
    }
}

void MainWindow::onNewSongIDCatchedFromClipboard(QString songID)
{
    int musicID = songID.toInt();
    foreach (auto i, (*musicsInfo))
    {
        if(i->musicID == musicID)
        {
            ui->tableWidgetMusicsInfo->selectRow(i->row);
            onTableWidgetRowSelected(i->row);
            return;
        }
    }

    int order = musicsInfo->size();
    ui->tableWidgetMusicsInfo->setRowCount(order + 1);

    (*musicsInfo)[songID] = new MusicInfo;
    (*musicsInfo)[songID]->musicID = musicID;
    (*musicsInfo)[songID]->status = 0;
    (*musicsInfo)[songID]->downloadProgressBar = new QProgressBar(this);
    (*musicsInfo)[songID]->downloadProgressBar->setAlignment(Qt::AlignCenter);
    (*musicsInfo)[songID]->row = order;
    queue->push_back((*musicsInfo)[songID]);


    ui->tableWidgetMusicsInfo->setItem(order, MUSIC_ID, new QTableWidgetItem(songID));
    ui->tableWidgetMusicsInfo->setItem(order, MUSIC_NAME, new QTableWidgetItem("Unknown"));
    ui->tableWidgetMusicsInfo->setItem(order, MUSIC_DURATION, new QTableWidgetItem("Unknown"));
    ui->tableWidgetMusicsInfo->setItem(order, MUSIC_SIZE, new QTableWidgetItem("Unknown"));
    ui->tableWidgetMusicsInfo->setItem(order, DOWNLOAD_PROGRESS, new QTableWidgetItem(""));
    ui->tableWidgetMusicsInfo->setCellWidget(order, DOWNLOAD_PROGRESS, (*musicsInfo)[songID]->downloadProgressBar);
    ui->tableWidgetMusicsInfo->setItem(order, STATUS, new QTableWidgetItem(tr("Waiting")));
    for(int i=0; i<COLUMN_COUNT; ++i)
    {
        QTableWidgetItem *item = ui->tableWidgetMusicsInfo->item(order, i);
        if(item != nullptr)
            item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

    currentMusicInfo = (*musicsInfo)[songID];
    ui->tableWidgetMusicsInfo->selectRow(order);

    if(Settings::getSettings().getStatus() >= 0)
    {
        QTimer::singleShot(0, this, &MainWindow::startParse);
    }
}

void MainWindow::showMusicInfo()
{
    //依次将获取的歌曲信息在主窗口展示出来
    //qDebug()<<currentMusicInfo->["songUrl"];
    //显示音乐名并设置超链接
    ui->labelMusic->setText(currentMusicInfo->name);
    //开启链接可用
    ui->labelMusic->setOpenExternalLinks(true);
    //显示音乐名和音乐别名
    ui->labelMusic->setText(QString("<a href=\"http://music.163.com/#/song?id=%1\">%2</a>%3")
                            .arg(currentMusicInfo->musicID)
                            .arg(currentMusicInfo->name)
                            .arg(currentMusicInfo->alais));
    ui->labelMusic->setStatusTip(currentMusicInfo->name + currentMusicInfo->alais);

    //显示歌手名
    ui->labelSinger->setOpenExternalLinks(true);
    switch (currentMusicInfo->artistId.size())
    {
    case 1:
        ui->labelSinger->setText(QString("<a href=\"http://music.163.com/#/artist?id=%1\">%2</a>")
                                 .arg(currentMusicInfo->artistId.at(0))
                                 .arg(currentMusicInfo->artist));
        break;
    case 2:
        ui->labelSinger->setText(QString("<a href=\"http://music.163.com/#/artist?id=%1\">%2</a>"
                                         ",<a href=\"http://music.163.com/#/artist?id=%3\">%4</a>")
                                 .arg(currentMusicInfo->artistId.at(0))
                                 .arg(currentMusicInfo->artists.at(0))
                                 .arg(currentMusicInfo->artistId.at(1))
                                 .arg(currentMusicInfo->artists.at(1)));
        break;
    default:
        ui->labelSinger->setText(currentMusicInfo->artist);
        break;
    }
    QString artists;
    foreach (auto i, currentMusicInfo->artists)
    {
        artists += (i + ",");
    }
    artists.resize(artists.size()-1);
    ui->labelSinger->setStatusTip(artists);

    //显示专辑名
    if(currentMusicInfo->albumId != 0)
    {
        ui->labelAlbum->setOpenExternalLinks(true);
        ui->labelAlbum->setText(QString("<a href=\"http://music.163.com/#/album?id=%1\">%2</a>")
                                .arg(currentMusicInfo->albumId)
                                .arg(currentMusicInfo->album));
    }
    else
    {
        ui->labelAlbum->setText(currentMusicInfo->album);
    }
    ui->labelAlbum->setStatusTip(currentMusicInfo->album);

    //显示下载地址
    if(currentMusicInfo->downloadInfo.size() != 0)
    {
        ui->lineEditDownloadAddress->setText(currentMusicInfo->downloadInfo[0].mp3Url);
        ui->lineEditDownloadAddress->setStatusTip(currentMusicInfo->downloadInfo[0].mp3Url);
    }

    //显示图片
    if(currentMusicInfo->pic != nullptr)
    {
        ui->labelPicture->clear();
        ui->labelPicture->setPixmap(currentMusicInfo->pic->scaled(ui->labelPicture->size()));
    }
    else
        ui->labelPicture->setText(tr("Picture is downloading..."));

    ui->pushButtonCopy->setEnabled(true);
    ui->pushButtonDownload->setEnabled(true);
    ui->pushButtonPlay->setEnabled(true);
}

void MainWindow::clearMusicInfo()
{
    ui->labelMusic->clear();
    ui->labelMusic->setStatusTip("");
    ui->labelSinger->clear();
    ui->labelSinger->setStatusTip("");
    ui->labelAlbum->clear();
    ui->labelAlbum->setStatusTip("");
    ui->labelPicture->clear();
    ui->lineEditDownloadAddress->clear();
    ui->lineEditDownloadAddress->setStatusTip("");
    ui->pushButtonCopy->setEnabled(false);
    ui->pushButtonDownload->setEnabled(false);
    ui->pushButtonPlay->setEnabled(false);
}

void MainWindow::showInfo(QString info)
{
    //托盘显示提示信息，显示时间为1s（显示时间不对？）
    trayicon->showMessage(tr("fun music"), info,
                          QSystemTrayIcon::Information, 1000);
}

void MainWindow::HandleError(QString error )
{
    //托盘显示错误信息，显示时间为3s
    trayicon->showMessage(tr("fun music"), error,
                          QSystemTrayIcon::Warning, 3000);

    --handlingCount;
    if(!(Settings::getSettings().getStatus() >= 0))
    {
        startParse();
    }
}

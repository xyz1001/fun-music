#include "options.h"
#include "ui_options.h"
#include <QFileDialog>
#include <QPushButton>
#include <QDir>
#include <QDebug>
#include <QDesktopServices>

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    //设置控件名
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());

    connect(ui->pushButtonOkSelect, &QPushButton::clicked, this, &Options::onBtnSelectClicked);
    connect(ui->pushButtonOkOpen, &QPushButton::clicked, this, &Options::onBtnOpenClicked);
    connect(ui->pushButtonOk, &QPushButton::clicked, this, &Options::onBtnOKClicked);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &Options::onBtnApplyClicked);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &Options::onBtnCancelClicked);
}

Options::~Options()
{
    delete ui;
}

QString Options::chooseMusicDownloadDir()
{
    //获取目录，默认在程序当前目录下music目录下
    QString path=QFileDialog::getExistingDirectory(this, tr("Select directory"),
                                      QDir::currentPath()+"/music/",
                                             QFileDialog::ShowDirsOnly);    //仅显示文件夹
    //如果路径不为空且不以"/"结尾，即不再根目录则在路径后添加"/"
    if(!path.isEmpty() && !path.endsWith('/'))
        path+="/";
    //qDebug()<<path;

    //如果选择文件目录时点击了取消
    if(path == "")
        path = ui->lineEditMusicSaveDir->text();
    return path;
}

void Options::loadConfigs()
{
    Configs *configs = Settings::getSettings().configs;
    ui->lineEditMusicSaveDir->setText(configs->downloadDirectory);
    ui->checkBoxAutoDownload->setChecked(configs->autoDownload);
    ui->checkBoxAutoHide->setChecked(configs->autoHide);
    ui->checkBoxShowGuidaceOnStart->setChecked(configs->showGuidanceOnStart);
    ui->comboBoxQuality->setCurrentIndex(configs->quality);
    ui->radioButtonJsonParser->setChecked(configs->useJsonParser);
    ui->radioButtonHtmlParser->setChecked(!(configs->useJsonParser));
}

void Options::writeConfigs()
{
    Configs *configs = Settings::getSettings().configs;
    configs->downloadDirectory = ui->lineEditMusicSaveDir->text();
    configs->autoDownload = ui->checkBoxAutoDownload->isChecked();
    configs->autoHide = ui->checkBoxAutoHide->isChecked();
    configs->showGuidanceOnStart = ui->checkBoxShowGuidaceOnStart->isChecked();
    configs->quality = ui->comboBoxQuality->currentIndex();
    configs->useJsonParser = ui->radioButtonJsonParser->isChecked();
}

void Options::apply()
{   
    writeConfigs();
    Settings::getSettings().save();
}

void Options::onBtnSelectClicked()
{
    ui->lineEditMusicSaveDir->setText(chooseMusicDownloadDir());
}

void Options::onBtnOpenClicked()
{
    QDesktopServices::openUrl(QUrl(QString("file:///%1")
                                   .arg(ui->lineEditMusicSaveDir->text())));
}

void Options::onBtnOKClicked()
{
    apply();
    loadConfigs();
    hide();
}

void Options::onBtnApplyClicked()
{
    apply();
}

void Options::onBtnCancelClicked()
{
    //恢复当前设置，丢弃未保存的设置修改
    loadConfigs();
    hide();
}

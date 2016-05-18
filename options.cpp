#include "options.h"
#include "ui_options.h"
#include "ui_androidoptions.h"
#include <QFileDialog>
#include <QPushButton>
#include <QDir>
#include <QDebug>

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
  //ui(new Ui::AndroidOptions)
{
    ui->setupUi(this);
    setWindowTitle(tr("Options"));
    ui->labelMusicSaveDir->setText(tr("Music Download Dir:"));
    ui->pushButtonOkSelect->setText(tr("Select"));
    ui->checkBoxAutoDownload->setText(tr("Auto download"));
    ui->checkBoxAutoHide->setText(tr("Auto hide"));
    ui->pushButtonOk->setText(tr("OK"));
    ui->pushButtonApply->setText(tr("Apply"));
    ui->pushButtonCancel->setText(tr("Cancel"));

    connect(ui->pushButtonOkSelect, &QPushButton::clicked, this, &Options::onBtnSelectClicked);
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
    QString path=QFileDialog::getExistingDirectory(this, tr("Select directory"),
                                      QDir::currentPath()+"/music/",
                                             QFileDialog::ShowDirsOnly);
    if(!path.isEmpty() && !path.endsWith('/'))
        path+="/";
    //qDebug()<<path;
    return path;
}

void Options::loadSettings(QString musicSaveDir, bool autoDownload, bool autoHide)
{
    this->musicSaveDir=musicSaveDir;
    this->autoDownload=autoDownload;
    this->autoHide=autoHide;
    ui->lineEditMusicSaveDir->setText(musicSaveDir);
    ui->checkBoxAutoDownload->setChecked(autoDownload);
    ui->checkBoxAutoHide->setChecked(autoHide);
}

void Options::apply()
{
    if(musicSaveDir.isEmpty())
        musicSaveDir = QDir::currentPath()+"/music/";
    else
        musicSaveDir = ui->lineEditMusicSaveDir->text();
    autoDownload = ui->checkBoxAutoDownload->isChecked();
    autoHide = ui->checkBoxAutoHide->isChecked();
    emit signalUpdateSettings(musicSaveDir, autoDownload, autoHide);
}

void Options::onBtnSelectClicked()
{
    ui->lineEditMusicSaveDir->setText(chooseMusicDownloadDir());
}

void Options::onBtnOKClicked()
{
    apply();
    ui->lineEditMusicSaveDir->setText(this->musicSaveDir);
    ui->checkBoxAutoDownload->setChecked(this->autoDownload);
    ui->checkBoxAutoHide->setChecked(this->autoHide);
    hide();
}

void Options::onBtnApplyClicked()
{
    apply();
}

void Options::onBtnCancelClicked()
{
    ui->lineEditMusicSaveDir->setText(this->musicSaveDir);
    ui->checkBoxAutoDownload->setChecked(this->autoDownload);
    ui->checkBoxAutoHide->setChecked(this->autoHide);
    hide();
}

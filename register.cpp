#include "register.h"
#include "ui_register.h"
#include "settings.h"
#include <QMessageBox>
#include <QApplication>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    ui->lineEditDeviceCode->setText(deviceCheck.getCpuId());
    this->setWindowFlags(Qt::CustomizeWindowHint);
    setFixedSize(this->size());
    connect(ui->lineEditRegisterCode, &QLineEdit::textChanged, this, &Register::onInputRegisterCode);
    connect(ui->pushButtonOK, &QPushButton::clicked, this, &Register::onBtnOKClicked);
    connect(ui->pushButtonExit, &QPushButton::clicked, this, &Register::onBtnExitClicked);
}

bool Register::check(QString registerCode)
{
    return deviceCheck.checkRegisterCode(registerCode);
}

bool Register::isRegistered() const
{
    return registered;
}

Register::~Register()
{

    delete ui;
}

QString Register::getRegisterCode() const
{
    return registerCode;
}

void Register::onInputRegisterCode(QString registerCode)
{
    if(registerCode.isEmpty())
    {
        ui->pushButtonOK->setEnabled(false);
    }
    else
    {
        ui->pushButtonOK->setEnabled(true);
    }
}

void Register::onBtnOKClicked()
{
    if(deviceCheck.checkRegisterCode(ui->lineEditRegisterCode->text()))
    {
        registered = true;
        this->registerCode = ui->lineEditRegisterCode->text();
        this->close();
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Wrong Register Code"), tr("OK"));
    }
}

void Register::onBtnExitClicked()
{
    close();
}

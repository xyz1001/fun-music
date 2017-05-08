#include "guidance.h"
#include "ui_guidance.h"
#include <QPushButton>

Guidance::Guidance(bool openOnStart, QWidget *parent):
    QDialog(parent),
    ui(new Ui::Guidance)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    ui->checkBoxShowOnStart->setVisible(openOnStart);
    ui->checkBoxShowOnStart->setChecked(Settings::getSettings().configs->showGuidanceOnStart);
    connect(ui->pushButtonOK, &QPushButton::clicked, this, &Guidance::onBtnOKClicked);
}

Guidance::~Guidance()
{
    delete ui;
}

void Guidance::onBtnOKClicked()
{
    if(ui->checkBoxShowOnStart->isVisible())
    {
        Settings::getSettings().configs->showGuidanceOnStart = ui->checkBoxShowOnStart->isChecked();
        Settings::getSettings().save();
    }
    this->hide();
}


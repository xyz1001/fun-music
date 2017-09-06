#ifndef GUIDANCE_H
#define GUIDANCE_H

#include <QDialog>
#include "settings.h"

namespace Ui {
    class Guidance;
}


class Guidance : public QDialog
{
    Q_OBJECT

public:
    Guidance(bool openOnStart = false, QWidget *parent = 0);
    ~Guidance();
private:
    Ui::Guidance *ui;

public slots:
    void onBtnOKClicked();
};

#endif // GUIDANCE_H

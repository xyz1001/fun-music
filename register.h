#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include "devicecheck.h"

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT
private:
    DeviceCheck deviceCheck;    //校验
    QString registerCode;   //注册码

    bool registered = false;    //是否注册成功
public:
    explicit Register(QWidget *parent = 0);
    ~Register();

    bool check(QString registerCode);   //检查注册码是否正确
    bool isRegistered() const;
    QString getRegisterCode() const;    //获取注册码
public slots:

    void onInputRegisterCode(QString registerCode); //当输入注册码时
    void onBtnOKClicked();
    void onBtnExitClicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H

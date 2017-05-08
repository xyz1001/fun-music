/*获取硬件信息，并对license文件内容进行AES解密处理，再将二者进一步处理得到注册信息*/
#ifndef DEVICECHECK_H
#define DEVICECHECK_H
#include <QStringList>
#include <QString>
class DeviceCheck
{
    QString deviceCode;

public:
    DeviceCheck();
    bool checkRegisterCode(QString registerCode);   //检查注册码是否正确
    QString getCpuId(); //获取CPU序列号
};

#endif // DEVICECHECK_H

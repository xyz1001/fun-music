#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QWidget>
#include "constant.h"
#include "settings.h"

namespace Ui {
    class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

    /*
     * Description: 选择音乐下载目录
     * Output: 返回音乐保存目录
     */
    QString chooseMusicDownloadDir();
    /*
     * Description: 导入当前配置
     */
    void loadConfigs();
private:
    Ui::Options *ui;

    //应用选项
    void apply();
    void writeConfigs();

public slots:
    /*
     * Description: 响应选择目录按钮点击事件，弹出音乐下载目录选择框
     */
    void onBtnSelectClicked();
    void onBtnOpenClicked();
    /*
     * Description: 响应确认按钮点击事件，应用设置并关闭选项窗口
     */
    void onBtnOKClicked();
    /*
     * Description: 响应应用按钮点击事件，应用设置
     */
    void onBtnApplyClicked();
    /*
     * Description: 响应取消按钮点击事件，隐藏选项窗口
     */
    void onBtnCancelClicked();
};

#endif // OPTIONS_H

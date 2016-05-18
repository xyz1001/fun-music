/* *******************************************************************************************************************
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/14
 * Description：网易云音乐下载助手（163MusicDownloadAssistant）是一款用Qt5.5编写的软件，通过获取网易云音乐的歌曲ID获取该歌曲的下载地址，
 * 包括付费和已下架的歌曲。
 *
 * Version：0.1
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/14
 * Description: 1.提出需求，初步完成了UI设计
 *              2.通过抓取"http://music.163.com/api/song/detail/?id="的Json初步获取了部分音乐的信息，为
 *                获取下载地址提供了API接口
 *              3.完成JsonGetter，PictureGetter和MP3Getter三个网络下载类的编写
 *              4.实现了托盘程序
 *
 * Version：0.2
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/15
 * Description: 1.发现已下架歌曲及付费音乐通过抓取Json无法获取下载地址，转向通过抓取"http://www.loadfield.com/163/song.php?id="
 *  网页内容并提取其中的歌曲信息
 *
 * Version：0.3
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/16
 * Description: 1.对整体进行代码重构，将Json和Html两种方式提取出一个虚基类并分别实现。
 *              2.添加设置功能
 *              3.添加存储本地配置保存功能
 *              4.添加剪贴板监视
 *
 * Version：1.0
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/17
 * Description: 1.完善了基本功能，对部分小Bug进行的修改。
 *              2.完成了翻译
 *              3.打包成独立版本
 * Summary: 实现了基本功能，包括包括托盘功能，获取音乐信息功能，信息展示功能，下载功能，剪贴板监视功能和设置功能，可以实现对下架和付费歌曲的下载
 *          下一阶段计划有: 1.实现队列下载，可同时进行多个任务
 *                       2.实现高品质音乐的下载
 *                       3.实现搜索功能
 *
 * Version：1.0
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/18
 * Description: 1.添加注释
 * *******************************************************************************************************************/
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //添加中文翻译
    QTranslator translator;
    translator.load(":/lang/myI18N_zh_CN.qm");
    a.installTranslator(&translator);

    MainWindow w;
    //如果未设置启动时自动隐藏，显示主窗口
    if(!w.getIsAutoHide())
        w.show();
    return a.exec();
}

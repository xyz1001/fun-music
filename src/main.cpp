/* *******************************************************************************************************************
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/14
 * Description：网易云音乐助手（netease music assistant）是一款用Qt5编写的软件，通过获取网易云音乐的歌曲ID获取该歌曲的下载地址，
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
 *          下一阶段计划有: 1.实现队列下载，可同时进行多个任务 √
 *                       2.实现高品质音乐的下载 √
 *                       3.实现搜索功能
 *
 * Version：1.0
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/18
 * Description: 1.添加注释
 *
 * Version：1.1
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/20
 * Description: 1.新增队列下载，可连续复制歌曲链接
 *
 * Version：1.2
 * Copyright：Zix
 * Author：Zix
 * Date：2016/05/30
 * Description: 1.紧急修复歌曲解析失效的问题
 *
 * Version：2.0
 * Copyright：Zix
 * Author：Zix
 * Date：2016/06/06
 * Description: 1.界面大幅度修改，优化体验
 *              2.去除不必要的手动输入ID功能及相关功能
 *              3.新增歌曲信息列表，方便查看历史记录
 *              4.新增同时多首歌曲解析下载功能
 *              5.新增高品质音乐下载功能
 *              6.优化信息提示，避免频繁弹出信息提示
 *              7.其他BUG修复，增强稳定性
 *
 * Version：2.1
 * Copyright：Zix
 * Author：Zix
 * Date：2016/06/07
 * Description: 1.关机蓝屏导致MainWindow.cpp文件代码全部丢失，写完代码后一定要及时保存！！！！
 *
 * Version：2.2
 * Copyright：Zix
 * Author：Zix
 * Date：2016/06/07
 * Description: 1.新增帮助界面，方便使用
 *              2.新增“音质选择”和“启动时是否显示帮助”设置
 * TODO: 1.更新除下载地址之外的歌曲信息来源为官方查询API： √
 *         http://music.163.com/api/song/detail/?id=$id&ids=[$id]&csrf_token=Method=GET;
 *       2.修复多首歌曲下载时崩溃的问题
 *       3.在线软件更新
 *       4.自动在线更新歌曲下载链接获取源
 *       5.取消下载功能
 *       6.界面的进一步优化，包括但不限于查看大图，别名显示，跳转链接等
 *       7.支持自定义歌曲命名
 *       8.同时进行任务数设置
 * Summary: 2.0版相对于1.0版用户体验改善了很多，去除了没有必要的功能，新增了帮助引导和改善了界面，但存在一定的崩溃问题。
 *
 * Version：2.3
 * Copyright：Zix
 * Author：Zix
 * Date：2016/09/17
 * Description: 1.修复由于网址及HTML页面修改导致无法解析的问题
 *
 * Version：2.5
 * Copyright：Zix
 * Author：Zix
 * Date：2016/09/18
 * Description: 1.将Settings类重构,直接使用QSetting类,并使其为单例类,大大降低了代码复杂度
 *              2.重构options类,将载入配置提取为单独的函数,方便使用
 *
 * Version：3.0
 * Copyright：Zix
 * Author：Zix
 * Date：2016/09/19
 * Description: 1.新增通过网易云音乐API接口获取歌曲信息的解析方式
 *              2.移除旧的Json获取和解析类,重写HandlerWithJson类
 *              3.MusicInfo结构体重新设计,减少冗余变量,提高灵活性
 *
 * Version：3.1
 * Copyright：Zix
 * Author：Zix
 * Date：2016/09/20
 * Description: 1.新增设置中可选择高中低三种音质
 *              2.新增设置中可选择HTML解析方式或JSON解析方式
 *              3.大量BUG修复
 *
 * Version：3.2
 * Copyright：Zix
 * Author：Zix
 * Date：2016/09/21
 * Description: 1.将所有的网络数据获取类重新设计,均继承于NetworkAccess类,大大提高代码可读性
 *              2.新增设置中可打开歌曲下载目录
 *              3.移除歌曲信息显示表垂直表头,美化水平表头
 *              4.为歌曲条目添加右键菜单,包括播放,下载,重试,删除,复制下载链接到剪贴板和打开文件所在位置
 *
 * Version：3.3
 * Copyright：Zix
 * Author：Zix
 * Date：2016/09/22
 * Description: 1.为所有的网络数据获取类添加超时重试机制
 *              2.歌曲信息显示表添加歌曲时长大小信息栏
 *              3.添加批量处理按钮:全部下载,全部重试及清空列表
 *
 * Version：3.4
 * Copyright：Zix
 * Author：Zix
 * Date：2016/09/23
 * Description: 1.为MP3下载添加断点续传功能
 *              2.图片下载失败时提示更清晰
 *              3.解析失败,下载失败时歌曲条目标红,查看更方便
 *              4.移除无用的"下载到"按钮,替换为"播放"按钮
 *              5.添加程序背景图,美化程序
 *
 * Version：3.5
 * Copyright：Zix
 * Author：Zix
 * Date：2016/09/24
 * Description: 1.添加注册界面,避免大量传播
 *              2.添加中文翻译
 *              3.优化调整
 *
 * Version：3.6
 * Copyright：Zix
 * Author：Zix
 * Date：2016/09/25
 * Description: 1.打包发布
 *              2.优化提示
 *              3.界面微调
 * TODO: 1.搜索歌曲功能
 *       2.稳定性增强
 *       3.在线软件更新
 *       4.自动在线更新歌曲下载链接获取源
 *       5.历史记录功能
 *       6.可查看大图
 *       7.支持自定义歌曲命名
 *       8.同时进行任务数设置
 *       9.歌曲ID默认使用整型存储
 *       10.UI优化
 * Summary: 3.X版大幅度优化了程序的代码,使得整个程序的结构及冗余度得到很大改善。同时，3.X将解析方式切换为官方API直接解析，
 *          不仅提高了解析的稳定性，加快了解析速度，也提供了更多有用的信息用来展示。但由于之前版本遗留的问题，导致程序的稳定
 *          性还需要进一步提高，另外，UI界面还需要进一步修改
 *
 * Version：3.7
 * Copyright：Zix
 * Author：Zix
 * Date: 2017/05/08
 * Description: 1.统一程序名称为网易云音乐助手(netease music assistant)
 *
 * *******************************************************************************************************************/
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include "settings.h"
#include "register.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //添加中文翻译
    QTranslator translator;
    translator.load(":/language/qm/zh_CN.qm");
    a.installTranslator(&translator);

    //载入本地设置
    Settings::getSettings().load();
    Register reg;
    //如果之前未注册
    if(!(reg.check(Settings::getSettings().configs->registerCode)))
    {
        reg.exec();
        if(reg.isRegistered())  //如果注册码输入正确
        {
            MainWindow w;
            //保存注册码，下次打开时无需再次注册
            Settings::getSettings().configs->registerCode = reg.getRegisterCode();
            Settings::getSettings().save();
            //如果未设置启动时自动隐藏，显示主窗口
            if(!(Settings::getSettings().configs->autoHide))
                w.show();
            return a.exec();
        }
        else
            return 0;
    }
    else
    {
        MainWindow w;
        if(!(Settings::getSettings().configs->autoHide))
            w.show();
        return a.exec();
    }
}

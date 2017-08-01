# 网易云音乐助手
网易云音乐助手是一款辅助网易云音乐下载歌曲的软件

# 安装
- 二进制包安装
本程序提供deb安装包，前往[下载页面](https://github.com/xyz1001/neteaseMusicAssistant/releases)

- 编译安装
 1. 安装编译依赖
 ```
 sudo apt install qt5-qmake qt5-default
 ```
 2. 安装运行依赖
 ```
 sudo apt install libqt5widgets5 libqt5gui5 libqt5network5 libqt5xml5 libqt5core5a
 ```
 3. 下载本程序源代码并解压进入
 4. 编译
 ```
 mkdir build && cd build && qmake ../src && make
 ```
 5. 运行
 ```
 ./netease-music-assistant
 ```
![运行效果](http://otz8yoa7u.bkt.clouddn.com/neteaseMusicAssistant/%E7%BD%91%E6%98%93%E4%BA%91%E9%9F%B3%E4%B9%90%E5%8A%A9%E6%89%8B%E6%BC%94%E7%A4%BA.png)
![运行效果](http://otz8yoa7u.bkt.clouddn.com/neteaseMusicAssistant/%E7%BD%91%E6%98%93%E4%BA%91%E9%9F%B3%E4%B9%90%E5%8A%A9%E6%89%8B%E6%BC%94%E7%A4%BA2.png)
![运行效果](http://otz8yoa7u.bkt.clouddn.com/neteaseMusicAssistant/%E7%BD%91%E6%98%93%E4%BA%91%E9%9F%B3%E4%B9%90%E5%8A%A9%E6%89%8B%E6%BC%94%E7%A4%BA3.png)

# 使用 
![使用方法](http://otz8yoa7u.bkt.clouddn.com/neteaseMusicAssistant/%E7%BD%91%E6%98%93%E4%BA%91%E9%9F%B3%E4%B9%90%E5%8A%A9%E6%89%8B%E8%8E%B7%E5%8F%96%E9%93%BE%E6%8E%A5.png)

# 注意事项
1. 为了避免软件过于大范围传播，软件加入了注册验证，该措施仅仅用于限制传播，不为个人盈利目的。若有需要使用的，可自行研究源码或私信我
2. 存在少量付费或下架歌曲下载失败的问题，暂时没有找到原因
3. 下载线路二已基本废弃，请勿使用

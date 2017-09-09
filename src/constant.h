#ifndef CONSTANT
#define CONSTANT
#include <QString>
#include <QMap>

//歌曲信息表列字段常量
const int MUSIC_ID = 0;
const int MUSIC_NAME = 1;
const int MUSIC_DURATION = 2;
const int MUSIC_SIZE = 3;
const int DOWNLOAD_PROGRESS = 4;
const int STATUS = 5;
const int COLUMN_COUNT = 6;

//错误信息
enum {
    DATA_GETTER_ERROR, //数据获取错误
    JSON_GETTER_ERROR,  //JSON获取错误
    MP3_GETTER_ERROR,  //MP3下载错误
    PICTURE_GETTER_ERROR,  //图片下载错误
    OPTIONS_ERROR, //选项设置错误
    SETTINGS_ERROR,   //配置错误
    OTHER_ERROR //其他错误
};

#endif // CONSTANT


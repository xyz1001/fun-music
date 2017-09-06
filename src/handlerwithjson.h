#ifndef HANDLERWITHJSON_H
#define HANDLERWITHJSON_H

#include <QObject>
#include "datagetter.h"
#include "jsongetter.h"
#include "jsonparser.h"
#include "detailjsonparser.h"
#include "albumjsonparser.h"

//实现DataGetter，通过Json获取歌曲信息，需要实现getRawInfo，parseRawInfo和onRawInfoGotten函数
class HandlerWithJson : public DataGetter
{
    Q_OBJECT
private:
    JsonGetter *jsonGetter;
    JsonParser *jsonParser;

public:
    explicit HandlerWithJson(MusicInfo *musicInfo, QWidget *widget, QObject *parent = 0);
    void getMusicInfo();

signals:

public slots:
    void onDetailJsonGotton(QString json);
    void onAlbumJsonGotton(QString json);
};

#endif // HANDLERWITHJSON_H

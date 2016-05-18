#ifndef HANDLERWITHJSON_H
#define HANDLERWITHJSON_H

#include "datagetter.h"

//实现DataGetter，通过Json获取歌曲信息，需要实现getRawInfo，parseRawInfo和onRawInfoGotten函数
class HandlerWithJson : public DataGetter
{
    Q_OBJECT
public:
    explicit HandlerWithJson(QObject *parent = 0);

    void getRawInfo(QString song_id);
    void parseRawInfo();
signals:

public slots:
    void onRawInfoGotten();
};

#endif // HANDLERWITHJSON_H

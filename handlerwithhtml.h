#ifndef HANDLERWITHHTML_H
#define HANDLERWITHHTML_H

#include "datagetter.h"

//实现DataGetter，通过Html获取歌曲信息，需要实现getRawInfo，parseRawInfo和onRawInfoGotten函数
class HandlerWithHtml : public DataGetter
{
    Q_OBJECT
public:
    explicit HandlerWithHtml(QObject *parent = 0);

    void getRawInfo(QString song_id);
    void parseRawInfo();
signals:

public slots:
    void onRawInfoGotten();
};

#endif // HANDLERWITHHTML_H

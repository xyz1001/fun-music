#ifndef HANDLERWITHHTML_H
#define HANDLERWITHHTML_H

#include "datagetter.h"
#include "musicinfo.h"
#include "htmlgetter.h"
#include "htmlhandler.h"

//实现DataGetter，通过Html获取歌曲信息，需要实现getRawInfo，parseRawInfo和onRawInfoGotten函数
class HandlerWithHtml : public DataGetter
{
    Q_OBJECT
private:
    HtmlGetter *htmlGetter = nullptr; //获取Html
    HtmlHandler *htmlHandler = nullptr;

public:
    explicit HandlerWithHtml(MusicInfo *musicInfo, QWidget *widget, QObject *parent = 0);
    ~HandlerWithHtml();

    void getMusicInfo();

private slots:
    void onHtmlGotten(QString html);
};

#endif // HANDLERWITHHTML_H

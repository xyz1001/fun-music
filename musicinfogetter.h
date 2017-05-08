#ifndef MUSICINFOGETTER_H
#define MUSICINFOGETTER_H

#include <QObject>
#include "jsongetter.h"
#include "jsonparser.h"
#include "detailjsonparser.h"
#include "albumjsonparser.h"

class MusicInfoGetter : public QObject
{
    Q_OBJECT
private:
    JsonGetter *jsonGetter;
    JsonParser *jsonParser;
    MusicInfo *musicInfo;

public:
    explicit MusicInfoGetter(MusicInfo *musicInfo, QObject *parent = 0);
    void getMusicInfo();

signals:

public slots:
    void onDetailJsonGotton(QString json);
    void onAlbumJsonGotton(QString json);
    void onGetJsonFailed();
};

#endif // MUSICINFOGETTER_H

#ifndef ALBUMJSONPARSER_H
#define ALBUMJSONPARSER_H

#include "jsonparser.h"

class AlbumJsonParser : public JsonParser
{
    Q_OBJECT
public:
    AlbumJsonParser(QString json, MusicInfo *musicInfo, QObject *parent = 0);
    void parse();

};

#endif // ALBUMJSONPARSER_H

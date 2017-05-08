#ifndef DETAILJSONPARSER_H
#define DETAILJSONPARSER_H

#include "jsonparser.h"

class DetailJsonParser : public JsonParser
{
    Q_OBJECT

public:
    DetailJsonParser(QString json, MusicInfo *musicInfo, QObject *parent = 0);
    void parse();
};

#endif // DETAILJSONPARSER_H

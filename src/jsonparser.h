#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include "musicinfo.h"

class JsonParser : public QObject
{
    Q_OBJECT
private:
    QString decryption(QString dfsId);
    void getDownloadInfo(QJsonObject object, DownloadInfo &downloadInfo);

protected:
    QString json;
    MusicInfo *musicInfo;

    void parseDownloadInfo(QJsonObject &json);
public:
    explicit JsonParser(QString json, MusicInfo *musicInfo, QObject *parent = 0);
    virtual void parse() = 0;

signals:
    void ParseError();

public slots:

};

#endif // JSONPARSER_H

#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QObject>
#include <QMap>

class JsonHandler : public QObject
{
    Q_OBJECT
private:
    QString json_content;
    QMap<QString, QString> *music_info;

    void mp3UrlReprocessing();
signals:
    void signalMusicInfoGotten(QMap<QString, QString> *music_info);
    void signalJsonhandlerError(QString error);
public:
    explicit JsonHandler(QString json_content, QMap<QString, QString> *music_info, QObject *parent = 0);

    void parseJson();
};

#endif // JSONHANDLER_H

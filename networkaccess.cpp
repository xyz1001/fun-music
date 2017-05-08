#include "networkaccess.h"
#include "constant.h"

NetworkAccess::NetworkAccess(QUrl url, QObject *parent) : QObject(parent)
{
    if(url.isEmpty())
    {
        emit errorOccurred(tr("Empty Url"), OTHER_ERROR);
        return;
    }
    this->url = url;
    networkManager = new QNetworkAccessManager(this);

    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, this, &NetworkAccess::onTimeOut);
}


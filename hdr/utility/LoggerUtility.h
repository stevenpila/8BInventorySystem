#ifndef LOGGERUTILITY_H
#define LOGGERUTILITY_H

#include <QDebug>

#define LOG_INFO(msg) \
    qInfo() << msg;
#define LOG_DEBUG(msg) \
    qDebug() << msg;
#define LOG_WARNING(msg) \
    qWarning() << msg;
#define LOG_ERROR(msg) \
    qCritical() << msg;

#endif // LOGGERUTILITY_H

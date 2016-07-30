#ifndef ISQMLCONTROLLER_H
#define ISQMLCONTROLLER_H

#include <QObject>

class ISQmlController : public QObject
{
    Q_OBJECT
public:
    explicit ISQmlController(QObject *parent = 0);

signals:
    void sample();

public slots:
    Q_INVOKABLE void sampleSlot();
};

#endif // ISQMLCONTROLLER_H

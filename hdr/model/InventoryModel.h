#ifndef INVENTORYMODEL_H
#define INVENTORYMODEL_H

#include <QObject>

#include "../utility/LoggerUtility.h"

class InventoryModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint64 id READ getId)
    Q_PROPERTY(QString item READ getItem WRITE setItem NOTIFY sigItemChanged)
    Q_PROPERTY(quint16 quantity READ getQuantity WRITE setQuantity NOTIFY sigQuantityChanged)
    Q_PROPERTY(float_t price READ getPrice WRITE setPrice NOTIFY sigPriceChanged)

    quint64 m_id;
    QString m_item;
    quint16 m_quantity;
    float_t m_price;
public:
    explicit InventoryModel(QObject *parent = 0);
    ~InventoryModel();

    void setId(quint64);
    quint64 getId() const;

    void setItem(const QString&);
    const QString& getItem() const;

    void setQuantity(quint16);
    quint16 getQuantity() const;

    void setPrice(float_t);
    float_t getPrice() const;

signals:
    void sigItemChanged(const QString&);
    void sigQuantityChanged(quint16);
    void sigPriceChanged(float_t);

public slots:
    void slotItemChanged(const QString&);
};

#endif // INVENTORYMODEL_H

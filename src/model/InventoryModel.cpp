#include "InventoryModel.h"

InventoryModel::InventoryModel(QObject *parent) : QObject(parent),
      m_id(0),
      m_item(),
      m_quantity(0),
      m_price(0.0)
{
    if(!connect(this, SIGNAL(sigItemChanged(QString)), SLOT(slotItemChanged(QString))))
    {
        LOG_ERROR("there is an error connecting signal sigItemChanged to slot slotItemChanged!")
    }
}

InventoryModel::~InventoryModel()
{
    if(!disconnect(this, 0, 0, 0))
    {
        LOG_ERROR("there is an error disconnecting all signals!")
    }
}

void InventoryModel::setId(quint64 id)
{
    this->m_id = id;
}

quint64 InventoryModel::getId() const
{
    return this->m_id;
}

void InventoryModel::setItem(const QString& item)
{
    this->m_item = item;
}

const QString& InventoryModel::getItem() const
{
    return this->m_item;
}

void InventoryModel::setQuantity(quint16 quantity)
{
    this->m_quantity = quantity;
}

quint16 InventoryModel::getQuantity() const
{
    return this->m_quantity;
}

void InventoryModel::setPrice(float_t price)
{
    this->m_price = price;
}

float_t InventoryModel::getPrice() const
{
    return this->m_price;
}

void InventoryModel::slotItemChanged(const QString& item)
{
    Q_UNUSED(item)
}

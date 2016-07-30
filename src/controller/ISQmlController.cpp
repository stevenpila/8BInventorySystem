#include <ISQmlController.h>

#include <DatabaseManager.h>

ISQmlController::ISQmlController(QObject *parent) : QObject(parent)
{

}

void ISQmlController::sampleSlot()
{
    DatabaseManager dbManager;

    InventoryModel inventoryItem;
    inventoryItem.setItem("Charger");
    inventoryItem.setQuantity(5);
    inventoryItem.setPrice(150.50);

    DB::STATUS_CODE retCode = dbManager.addInventoryItem(inventoryItem);
    if(DB::STATUS_CODE::SUCCESS != retCode)
    {
        LOG_ERROR(dbManager.getStatusCodeMessage(retCode) << dbManager.getErrorMessage())
    }

    InventoryItemList_t inventoryItemList;
    if(DB::STATUS_CODE::SUCCESS != (retCode = dbManager.getAllInventoryItems(inventoryItemList)))
    {
        LOG_ERROR(dbManager.getStatusCodeMessage(retCode) << dbManager.getErrorMessage())
    }

    qDebug() << "inventoryItemList size: " << inventoryItemList.size();

    emit sample();
}

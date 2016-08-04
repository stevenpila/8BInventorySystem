#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <LoggerUtility.h>
#include <InventoryModel.h>
#include <Commons.h>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    DB::STATUS_CODE getAllInventoryItems(InventoryItemList_t&) const;
    DB::STATUS_CODE getInventoryItem(InventoryModel&) const;
    DB::STATUS_CODE addInventoryItem(InventoryModel&);
    DB::STATUS_CODE updateInventoryItem(const InventoryModel&);

    const char* getErrorMessage(const DB::STATUS_CODE&) const;
    const char* getErrorMessage() const;

private:
    DB::STATUS_CODE createConnection();
    DB::STATUS_CODE closeConnection();
    DB::STATUS_CODE initializeDatabaseSchema();


    QSqlDatabase m_db;
    mutable QString m_errorMessage;
};

#endif // DATABASEMANAGER_H

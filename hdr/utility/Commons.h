#ifndef COMMONS_H
#define COMMONS_H

#ifndef EXCEPTION
#define EXCEPTION ...
#endif

#include <QString>
#include <QDir>
#include <QVector>
#include <QSharedPointer>
#include <QMap>

typedef QVector<QSharedPointer<QObject>> InventoryItemList_t;

const quint8 SUCCESS = 0;

namespace DB
{
    const QString SQL_DRIVER = "QSQLITE";
    const QString PATH = QDir::currentPath() + "/db/";
    const QString FILENAME = "inventory_system.db";
    const QString CONNECTION_NAME = "inventory_system_connection";
    const QString NAME = "inventory_system_db";

    const QString TABLE_INVENTORY = "inventory";
    const QString CREATE_TABLE_INVENTORY_QUERY = "CREATE TABLE IF NOT EXISTS " + TABLE_INVENTORY +
            " ('id'         INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, \
               'item'       NVARCHAR(50) NOT NULL UNIQUE, \
               'quantity'   INTEGER NOT NULL, \
               'price'      DOUBLE NOT NULL, \
               'is_deleted' INTEGER DEFAULT 0)";
    const QString TABLE_INVENTORY_HISTORY = "inventory_history";
    const QString CREATE_TABLE_INVENTORY_HISTORY_QUERY = "CREATE TABLE IF NOT EXISTS " + TABLE_INVENTORY_HISTORY +
            " ('item_id'            INTEGER NOT NULL, \
               'description'        NVHARCHAR(500) NOT NULL, \
               'transaction_date'   DATETIME NOT NULL, \
                                    PRIMARY KEY(item_id, transaction_date))";

    const QString GET_ALL_INVENTORY_ITEMS = "SELECT * FROM " + TABLE_INVENTORY;
    const QString GET_INVENTORY_ITEM = "SELECT * FROM " + TABLE_INVENTORY + " WHERE id = :id LIMIT 1";
    const QString ADD_INVENTORY_ITEM = "INSERT INTO " + TABLE_INVENTORY + " (item, quantity, price) VALUES (:item, :quantity, :price)";
    const QString UPDATE_INVENTORY_ITEM = "UPDATE " + TABLE_INVENTORY + " SET item = :item, quantity = :quantity, price = :price WHERE id = :id";

    const quint8 TABLE_INVENTORY_GET_BIND_VALUES_SIZE = 1;
    const quint8 TABLE_INVENTORY_ADD_BIND_VALUES_SIZE = 3;
    const quint8 TABLE_INVENTORY_UPDATE_BIND_VALUE_SIZE = 4;

    enum struct STATUS_CODE
    {
        SQLITE_DRIVER_NOT_AVAILABLE = -15,
        CONNECTION_INVALID_DRIVER,

        CREATE_SQLITE_FILE_FAILED,

        OPEN_CONNECTION_FAILED,
        CONNECTION_NOT_OPENED,
        CLOSE_CONNECTION_FAILED,

        REMOVE_CONNECTION_NAME_FAILED,

        RECORD_NOT_FOUND,
        START_TRANSACTION_FAILED,
        CREATE_SQL_QUERY_FAILED,
        EXECUTE_SQL_QUERY_FAILED,
        ROLLBACK_TRANSACTION_FAILED,
        COMMIT_TRANSACTION_FAILED,

        PREPARE_SQL_QUERY_FAILED,
        INCORRECT_BOUND_VALUES_SIZE,

        SUCCESS,

        CREATE_SQL_QUERY_AND_ROLLBAK_TRANSACTION_FAILED = DB::STATUS_CODE::CREATE_SQL_QUERY_FAILED + DB::STATUS_CODE::ROLLBACK_TRANSACTION_FAILED,
        EXECUTE_SQL_QUERY_AND_ROLLBAK_TRANSACTION_FAILED = DB::STATUS_CODE::EXECUTE_SQL_QUERY_FAILED + DB::STATUS_CODE::ROLLBACK_TRANSACTION_FAILED
    };

    typedef QMap<STATUS_CODE, QString> StatusCodeMessageList_t;
    const StatusCodeMessageList_t STATUS_MESSAGE = {
        { STATUS_CODE::SQLITE_DRIVER_NOT_AVAILABLE, "Driver " + SQL_DRIVER + " is not available! " },
        { STATUS_CODE::CONNECTION_INVALID_DRIVER, "Invalid driver! " },

        { STATUS_CODE::CREATE_SQLITE_FILE_FAILED, "Failed to create " + PATH + " directory! " },

        { STATUS_CODE::OPEN_CONNECTION_FAILED, "Failed to connect to database! " },
        { STATUS_CODE::CONNECTION_NOT_OPENED, "Database connection is not opened! " },
        { STATUS_CODE::CLOSE_CONNECTION_FAILED, "Failed to close database connection! " },

        { STATUS_CODE::REMOVE_CONNECTION_NAME_FAILED, "Failed to remove database connection name! " },

        { STATUS_CODE::RECORD_NOT_FOUND, "Record not found!" },
        { STATUS_CODE::START_TRANSACTION_FAILED, "Failed to begin transaction! " },
        { STATUS_CODE::CREATE_SQL_QUERY_FAILED, "Failed to create sql query! " },
        { STATUS_CODE::EXECUTE_SQL_QUERY_FAILED, "Failed to execute sql query! " },
        { STATUS_CODE::ROLLBACK_TRANSACTION_FAILED, "Failed to rollback transaction! " },
        { STATUS_CODE::COMMIT_TRANSACTION_FAILED, "Failed to commit transaction! " },

        { STATUS_CODE::PREPARE_SQL_QUERY_FAILED, "Failed to prepare sql query! " },
        { STATUS_CODE::INCORRECT_BOUND_VALUES_SIZE, "Incorrect number of bound values! " },

        { STATUS_CODE::SUCCESS, "Database operation completed successfully! " },

        { STATUS_CODE::CREATE_SQL_QUERY_AND_ROLLBAK_TRANSACTION_FAILED, "Failed to create sql query and rollback transaction! " },
        { STATUS_CODE::EXECUTE_SQL_QUERY_AND_ROLLBAK_TRANSACTION_FAILED, "Failed to execute sql query and rollback transaction! " }
    };
}

inline DB::STATUS_CODE& operator+= (DB::STATUS_CODE& left, const DB::STATUS_CODE& right)
{
    left = static_cast<DB::STATUS_CODE>(static_cast<int>(left) + static_cast<int>(right));
    return left;
}

#endif // COMMONS_H

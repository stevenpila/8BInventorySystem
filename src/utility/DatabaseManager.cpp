#include "DatabaseManager.h"

DatabaseManager::DatabaseManager()
{
    try
    {
        DB::STATUS_CODE retCode = createConnection();

        if(DB::STATUS_CODE::SUCCESS != retCode)
        {
            LOG_ERROR(getStatusCodeMessage(retCode) << "! " << m_db.lastError().text())
        }
        else
        {
            LOG_INFO("Successfully created connection to database!")
        }
    }
    catch(EXCEPTION)
    {
        LOG_ERROR("Exception occured while connecting to database! " << m_db.lastError().text())
    }
}

DatabaseManager::~DatabaseManager()
{
    try
    {
        DB::STATUS_CODE retCode = closeConnection();

        if(DB::STATUS_CODE::SUCCESS != retCode)
        {
            LOG_ERROR(getStatusCodeMessage(retCode) << "! " << m_db.lastError().text())
        }
        else
        {
            LOG_INFO("Successfully closed connection to database!")
        }
    }
    catch(EXCEPTION)
    {
        LOG_ERROR("Exception occured while closing connection to database! " << m_db.lastError().text())
    }
}

DB::STATUS_CODE DatabaseManager::createConnection()
{
    m_errorMessage = "";

    if(!QSqlDatabase::isDriverAvailable(DB::SQL_DRIVER))
    {
        return DB::STATUS_CODE::SQLITE_DRIVER_NOT_AVAILABLE;
    }

    if(QSqlDatabase::contains(DB::CONNECTION_NAME))
    {
        LOG_DEBUG("Connection name " << DB::CONNECTION_NAME << " already exists!")
        m_db = QSqlDatabase::database(DB::CONNECTION_NAME);
    }
    else
    {
        LOG_DEBUG("Connection name " << DB::CONNECTION_NAME << " does not exist!")
        m_db = QSqlDatabase::addDatabase(DB::SQL_DRIVER, DB::CONNECTION_NAME);
    }

    if(!m_db.isValid())
    {
        m_errorMessage = m_db.lastError().text();
        return DB::STATUS_CODE::CONNECTION_INVALID_DRIVER;
    }

    QDir dbPath(DB::PATH);
    bool isDbPathExists = true;
    if(!dbPath.exists())
    {
        LOG_DEBUG(DB::PATH << " does not exist!")
        if(!dbPath.mkdir(DB::PATH))
        {
            return DB::STATUS_CODE::CREATE_SQLITE_FILE_FAILED;
        }

        isDbPathExists = false;
    }

    m_db.setDatabaseName(DB::PATH + DB::FILENAME);

    if(!m_db.open())
    {
        m_errorMessage = m_db.lastError().text();
        return DB::STATUS_CODE::OPEN_CONNECTION_FAILED;
    }

    if(!isDbPathExists)
    {
        DB::STATUS_CODE retCode = initializeDatabaseSchema();

        if(DB::STATUS_CODE::SUCCESS != retCode)
        {
            return retCode;
        }

        LOG_INFO("Successfully initialized database!")
    }

    return DB::STATUS_CODE::SUCCESS;
}

DB::STATUS_CODE DatabaseManager::closeConnection()
{
    m_errorMessage = "";

    if(m_db.isOpen())
    {
        m_db.close();

        if(m_db.isOpen())
        {
            m_errorMessage = m_db.lastError().text();
            return DB::STATUS_CODE::CLOSE_CONNECTION_FAILED;
        }
    }

    m_db = QSqlDatabase();  // REFERENCE - http://stackoverflow.com/questions/9519736/warning-remove-database
    QSqlDatabase::removeDatabase(DB::CONNECTION_NAME);

    if(QSqlDatabase::contains(DB::CONNECTION_NAME) && m_db.isValid())
    {
        m_errorMessage = m_db.lastError().text();
        return DB::STATUS_CODE::REMOVE_CONNECTION_NAME_FAILED;
    }

    return DB::STATUS_CODE::SUCCESS;
}

DB::STATUS_CODE DatabaseManager::initializeDatabaseSchema()
{
    m_errorMessage = "";

    if(!m_db.isOpen())
    {
        return DB::STATUS_CODE::CONNECTION_NOT_OPENED;
    }

    if(!m_db.transaction())
    {
        m_errorMessage = m_db.lastError().text();
        return DB::STATUS_CODE::START_TRANSACTION_FAILED;
    }

    DB::STATUS_CODE retCode = [&]() -> DB::STATUS_CODE
    {
        QSqlQuery sqlQuery(DB::CREATE_TABLE_INVENTORY_QUERY, m_db);

        if(sqlQuery.lastError().isValid())
        {
            m_errorMessage = sqlQuery.lastError().text();
            return DB::STATUS_CODE::CREATE_SQL_QUERY_FAILED;
        }

        if(!sqlQuery.exec())
        {
            m_errorMessage = sqlQuery.lastError().text() + sqlQuery.executedQuery();
            return DB::STATUS_CODE::EXECUTE_SQL_QUERY_FAILED;
        }

        sqlQuery = QSqlQuery(DB::CREATE_TABLE_INVENTORY_HISTORY_QUERY, m_db);

        if(sqlQuery.lastError().isValid())
        {
            m_errorMessage = sqlQuery.lastError().text();
            return DB::STATUS_CODE::CREATE_SQL_QUERY_FAILED;
        }

        if(!sqlQuery.exec())
        {
            m_errorMessage = sqlQuery.lastError().text() + sqlQuery.executedQuery();
            return DB::STATUS_CODE::EXECUTE_SQL_QUERY_FAILED;
        }

        return DB::STATUS_CODE::SUCCESS;
    }();

    if(DB::STATUS_CODE::SUCCESS != retCode)
    {
        if(!m_db.rollback())
        {
            m_errorMessage += m_db.lastError().text();
            retCode += DB::STATUS_CODE::ROLLBACK_TRANSACTION_FAILED;
        }
    }
    else
    {
        if(!m_db.commit())
        {
            m_errorMessage = m_db.lastError().text();
            retCode += DB::STATUS_CODE::COMMIT_TRANSACTION_FAILED;
        }
    }

    return retCode;
}

DB::STATUS_CODE DatabaseManager::getAllInventoryItems(InventoryItemList_t& inventoryItemList) const
{
    m_errorMessage = "";

    if(!m_db.isOpen())
    {
        return DB::STATUS_CODE::CONNECTION_NOT_OPENED;
    }

    QSqlQuery sqlQuery(DB::GET_ALL_INVENTORY_ITEMS, m_db);

    if(sqlQuery.lastError().isValid())
    {
        m_errorMessage = sqlQuery.lastError().text();
        return DB::STATUS_CODE::CREATE_SQL_QUERY_FAILED;
    }

    if(!sqlQuery.exec())
    {
        m_errorMessage = sqlQuery.lastError().text() + sqlQuery.executedQuery();
        return DB::STATUS_CODE::EXECUTE_SQL_QUERY_FAILED;
    }

    sqlQuery.setForwardOnly(true);

    while(sqlQuery.next())
    {
        QSharedPointer<QObject> item(new InventoryModel());

        if(!item.isNull())
        {
            QSharedPointer<InventoryModel> itemTemp(item.dynamicCast<InventoryModel>());
            itemTemp->setId(sqlQuery.value("id").toULongLong());
            itemTemp->setItem(sqlQuery.value("item").toString());
            itemTemp->setQuantity(sqlQuery.value("quantity").toUInt());
            itemTemp->setPrice(sqlQuery.value("price").toFloat());

            inventoryItemList.append(item);
        }
    }

    return DB::STATUS_CODE::SUCCESS;
}

DB::STATUS_CODE DatabaseManager::addInventoryItem(const InventoryModel& inventoryItem)
{
    m_errorMessage = "";

    if(!m_db.isOpen())
    {
        return DB::STATUS_CODE::CONNECTION_NOT_OPENED;
    }

    if(!m_db.transaction())
    {
        m_errorMessage =  m_db.lastError().text();
        return DB::STATUS_CODE::START_TRANSACTION_FAILED;
    }

    DB::STATUS_CODE retCode = [&](const InventoryModel& inventoryItem) -> DB::STATUS_CODE
    {
        QSqlQuery sqlQuery(m_db);

        if(!sqlQuery.prepare(DB::ADD_INVENTORY_ITEM))
        {
            m_errorMessage = sqlQuery.lastError().text();
            return DB::STATUS_CODE::PREPARE_SQL_QUERY_FAILED;
        }

        sqlQuery.bindValue(":item", inventoryItem.getItem());
        sqlQuery.bindValue(":quantity", inventoryItem.getQuantity());
        sqlQuery.bindValue(":price", inventoryItem.getPrice());

        if(DB::TABLE_INVENTORY_ADD_BIND_VALUES_SIZE != sqlQuery.boundValues().size())
        {
            m_errorMessage = sqlQuery.lastError().text() + sqlQuery.boundValues().size();
            return DB::STATUS_CODE::INCORRECT_BOUND_VALUES_SIZE;
        }

        if(sqlQuery.lastError().isValid())
        {
            m_errorMessage = sqlQuery.lastError().text();
            return DB::STATUS_CODE::CREATE_SQL_QUERY_FAILED;
        }

        if(!sqlQuery.exec())
        {
            m_errorMessage = sqlQuery.lastError().text() + sqlQuery.executedQuery(); // TODO - print also bound values
            return DB::STATUS_CODE::EXECUTE_SQL_QUERY_FAILED;
        }

        return DB::STATUS_CODE::SUCCESS;
    }(inventoryItem);

    if(DB::STATUS_CODE::SUCCESS != retCode)
    {
        if(!m_db.rollback())
        {
            m_errorMessage += m_db.lastError().text();
            retCode += DB::STATUS_CODE::ROLLBACK_TRANSACTION_FAILED;
        }
    }
    else
    {
        if(!m_db.commit())
        {
            m_errorMessage = m_db.lastError().text();
            retCode += DB::STATUS_CODE::COMMIT_TRANSACTION_FAILED;
        }
    }

    return retCode;
}

const QString& DatabaseManager::getStatusCodeMessage(const DB::STATUS_CODE& statusCode) const
{
    DB::StatusCodeMessageList_t::ConstIterator iterStatusCodeMessage = DB::STATUS_MESSAGE.constFind(statusCode);

    if(iterStatusCodeMessage == DB::STATUS_MESSAGE.constEnd())
    {
        return EMPTY_STRING;
    }

    return iterStatusCodeMessage.value();
}

const QString& DatabaseManager::getErrorMessage() const
{
    return m_errorMessage;
}

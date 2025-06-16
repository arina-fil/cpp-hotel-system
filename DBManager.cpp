/**
 * @file DBManager.cpp
 * @brief Этот файл содержит реализацию класса DBManager для управления базой данных PostgreSQL.
 */

#include "DBManager.h"
#include <iostream>
#include <sstream>

/**
 * @brief Конструктор класса DBManager.
 * Инициализирует параметры подключения к базе данных.
 * @param host Имя хоста базы данных.
 * @param user Имя пользователя базы данных.
 * @param password Пароль пользователя базы данных.
 * @param database Имя базы данных.
 * @param port Порт базы данных (по умолчанию 5432).
 */
DBManager::DBManager(const std::string& host, const std::string& user,
                     const std::string& password, const std::string& database,
                     int port)
    : host(host), user(user), password(password), database(database), port(port),
      connection(nullptr) {
}

/**
 * @brief Деструктор класса DBManager.
 * Обеспечивает отключение от базы данных при уничтожении объекта.
 */
DBManager::~DBManager() {
    disconnect();
}

/**
 * @brief Устанавливает соединение с базой данных PostgreSQL.
 * @return True, если соединение успешно установлено, иначе false.
 */
bool DBManager::connect() {
    try {
        std::stringstream conninfo;
        conninfo << "host=" << host
                 << " port=" << port
                 << " dbname=" << database
                 << " user=" << user
                 << " password=" << password;

        connection = PQconnectdb(conninfo.str().c_str());

        if (PQstatus(connection) != CONNECTION_OK) {
            std::cerr << "Connection to database failed: " << PQerrorMessage(connection) << std::endl;
            PQfinish(connection);
            connection = nullptr;
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Отключается от базы данных PostgreSQL.
 */
void DBManager::disconnect() {
    if (connection) {
        PQfinish(connection);
        connection = nullptr;
    }
}

/**
 * @brief Проверяет, активно ли соединение с базой данных.
 * @return True, если соединение активно, иначе false.
 */
bool DBManager::isConnected() const {
    return connection != nullptr && PQstatus(connection) == CONNECTION_OK;
}

/**
 * @brief Выполняет SQL-запрос, который возвращает результат (например, SELECT).
 * @param query Строка SQL-запроса.
 * @return Объект PGResultWrapper, содержащий результат запроса.
 * @throw std::runtime_error Если база данных не подключена или выполнение запроса завершилось с ошибкой.
 */
PGResultWrapper DBManager::executeQuery(const std::string& query) {
    if (!isConnected()) {
        throw std::runtime_error("Database not connected");
    }
    
    PGresult* result = PQexec(connection, query.c_str());
    
    if (PQresultStatus(result) != PGRES_TUPLES_OK && 
        PQresultStatus(result) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(connection);
        PQclear(result); 
        throw std::runtime_error("Query execution failed: " + error);
    }
    
    return PGResultWrapper(result); 
}

/**
 * @brief Выполняет SQL-запрос на обновление данных (например, INSERT, UPDATE, DELETE).
 * @param query Строка SQL-запроса.
 * @return Количество затронутых строк.
 * @throw std::runtime_error Если база данных не подключена или выполнение запроса завершилось с ошибкой.
 */
int DBManager::executeUpdate(const std::string& query) {
    if (!isConnected()) {
        throw std::runtime_error("Database not connected");
    }
    
    PGResultWrapper result(PQexec(connection, query.c_str()));
    
    if (PQresultStatus(result.get()) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(connection);
        throw std::runtime_error("Update execution failed: " + error);
    }
    
    int affected = atoi(PQcmdTuples(result.get()));
    return affected; 
}

/**
 * @brief Начинает новую транзакцию базы данных.
 * @throw std::runtime_error Если база данных не подключена или начало транзакции завершилось с ошибкой.
 */
void DBManager::beginTransaction() {
    if (!isConnected()) {
        throw std::runtime_error("Database not connected");
    }
    
    PGResultWrapper result(PQexec(connection, "BEGIN"));
    if (PQresultStatus(result.get()) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(connection);
        throw std::runtime_error("Failed to begin transaction: " + error);
    }
}

/**
 * @brief Подтверждает (коммитит) текущую транзакцию базы данных.
 * @throw std::runtime_error Если база данных не подключена или коммит транзакции завершился с ошибкой.
 */
void DBManager::commit() {
    if (!isConnected()) {
        throw std::runtime_error("Database not connected");
    }
    
    PGResultWrapper result(PQexec(connection, "COMMIT"));
    if (PQresultStatus(result.get()) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(connection);
        throw std::runtime_error("Failed to commit transaction: " + error);
    }
}

/**
 * @brief Откатывает (отменяет) текущую транзакцию базы данных.
 * @throw std::runtime_error Если база данных не подключена или откат транзакции завершился с ошибкой.
 */
void DBManager::rollback() {
    if (!isConnected()) {
        throw std::runtime_error("Database not connected");
    }
    
    PGResultWrapper result(PQexec(connection, "ROLLBACK"));
    if (PQresultStatus(result.get()) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(connection);
        throw std::runtime_error("Failed to rollback transaction: " + error);
    }
}
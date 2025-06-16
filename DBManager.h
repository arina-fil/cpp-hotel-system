/**
 * @file DBManager.h
 * @brief Этот файл содержит объявление класса DBManager и класса PGResultWrapper.
 *        DBManager управляет подключениями к базе данных и выполнением запросов.
 */
#pragma once

#include <libpq-fe.h>
#include <string>
#include <memory>
#include <stdexcept>

/**
 * @brief RAII-обертка для PGresult* для устранения ручного управления памятью.
 * Этот класс гарантирует правильное освобождение ресурсов PGresult* при выходе из области видимости.
 */
class PGResultWrapper {
private:
    PGresult* result;

public:
    /**
     * @brief Конструирует новый объект PGResultWrapper.
     * @param res Указатель на PGresult для оборачивания.
     */
    explicit PGResultWrapper(PGresult* res) : result(res) {}

    /**
     * @brief Уничтожает объект PGResultWrapper и освобождает связанный PGresult.
     */
    ~PGResultWrapper() {
        if (result) {
            PQclear(result);
        }
    }
    

    PGResultWrapper(const PGResultWrapper&) = delete;
    PGResultWrapper& operator=(const PGResultWrapper&) = delete;
    
    /**
     * @brief Конструктор перемещения.
     * @param other Другой объект PGResultWrapper для перемещения.
     */
    PGResultWrapper(PGResultWrapper&& other) noexcept : result(other.result) {
        other.result = nullptr;
    }
    
    /**
     * @brief Оператор присваивания перемещением.
     * @param other Другой объект PGResultWrapper для перемещения.
     * @return Ссылка на перемещенный объект.
     */
    PGResultWrapper& operator=(PGResultWrapper&& other) noexcept {
        if (this != &other) {
            if (result) {
                PQclear(result);
            }
            result = other.result;
            other.result = nullptr;
        }
        return *this;
    }
    
    /**
     * @brief Получает обернутый указатель PGresult*.
     * @return Указатель на структуру PGresult.
     */
    PGresult* get() const { return result; }
    
    /**
     * @brief Проверяет, является ли результат PGresult действительным (не нулевым).
     * @return True, если результат действителен, иначе false.
     */
    bool isValid() const { return result != nullptr; }
    
    /**
     * @brief Освобождает владение указателем PGresult*.
     * Вызывающая сторона становится ответственной за освобождение возвращаемого PGresult*.
     * @return Освобожденный указатель PGresult*.
     */
    PGresult* release() {
        PGresult* temp = result;
        result = nullptr;
        return temp;
    }
};

/**
 * @brief Управляет подключениями и операциями с базой данных PostgreSQL.
 * Этот класс предоставляет методы для подключения, отключения, выполнения запросов
 * и управления транзакциями.
 */
class DBManager {
private:
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    int port;

    PGconn* connection; ///< Указатель на объект соединения PostgreSQL.
    
public:
    /**
     * @brief Конструирует новый объект DBManager.
     * @param host Хост базы данных.
     * @param user Пользователь базы данных.
     * @param password Пароль базы данных.
     * @param database Имя базы данных.
     * @param port Порт базы данных (по умолчанию 5432).
     */
    DBManager(const std::string& host, const std::string& user, 
              const std::string& password, const std::string& database, 
              int port = 5432);
    /**
     * @brief Уничтожает объект DBManager и отключается от базы данных, если подключено.
     */
    ~DBManager();

    /**
     * @brief Устанавливает соединение с базой данных PostgreSQL.
     * @return True, если соединение успешно, иначе false.
     */
    bool connect();
        
    /**
     * @brief Отключается от базы данных PostgreSQL.
     */
    void disconnect();

    /**
     * @brief Проверяет, активно ли соединение с базой данных.
     * @return True, если подключено, иначе false.
     */
    bool isConnected() const;
    
    /**
     * @brief Выполняет запрос к базе данных, который возвращает результаты (например, SELECT).
     * @param query Строка SQL-запроса для выполнения.
     * @return PGResultWrapper, содержащая результаты запроса.
     */
    PGResultWrapper executeQuery(const std::string& query);

    /**
     * @brief Выполняет запрос на обновление базы данных (например, INSERT, UPDATE, DELETE).
     * @param query Строка SQL-запроса для выполнения.
     * @return Количество затронутых строк.
     */
    int executeUpdate(const std::string& query);
    
    /**
     * @brief Начинает новую транзакцию базы данных.
     */
    void beginTransaction();

    /**
     * @brief Подтверждает текущую транзакцию базы данных.
     */
    void commit();
        
    /**
     * @brief Откатывает текущую транзакцию базы данных.
     */
    void rollback();
}; 
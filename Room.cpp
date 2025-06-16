/**
 * @file Room.cpp
 * @brief Этот файл содержит реализацию класса Room.
 */

#include "Room.h"
#include "DBManager.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Конструктор класса Room.
 * @param id Уникальный идентификатор номера.
 * @param number Номер комнаты.
 * @param type Тип комнаты.
 * @param pricePerDay Цена за номер в день.
 * @param description Описание номера.
 */
Room::Room(int id, const std::string& number, const std::string& type,
           double pricePerDay, const std::string& description)
    : id(id), number(number), type(type), pricePerDay(pricePerDay), description(description) {}

/**
 * @brief Возвращает идентификатор номера.
 * @return Идентификатор номера.
 */
int Room::getId() const { return id; }

/**
 * @brief Возвращает номер комнаты.
 * @return Номер комнаты.
 */
std::string Room::getNumber() const { return number; }

/**
 * @brief Возвращает тип комнаты.
 * @return Тип комнаты.
 */
std::string Room::getType() const { return type; }

/**
 * @brief Возвращает цену за номер в день.
 * @return Цена за номер в день.
 */
double Room::getPricePerDay() const { return pricePerDay; }

/**
 * @brief Возвращает описание номера.
 * @return Описание номера.
 */
std::string Room::getDescription() const { return description; }

/**
 * @brief Получает список всех номеров из базы данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @return Вектор объектов Room, представляющих все номера.
 */
std::vector<Room> Room::getAllRooms(DBManager& dbManager) {
    std::vector<Room> rooms;
    try {
        std::string query = "SELECT id, number, type, price_per_day, description FROM rooms;";
        PGResultWrapper result = dbManager.executeQuery(query);

        for (int i = 0; i < PQntuples(result.get()); i++) {
            int id = std::stoi(PQgetvalue(result.get(), i, 0));
            std::string number = PQgetvalue(result.get(), i, 1);
            std::string type = PQgetvalue(result.get(), i, 2);
            double pricePerDay = std::stod(PQgetvalue(result.get(), i, 3));
            std::string description = PQgetvalue(result.get(), i, 4);
            rooms.emplace_back(id, number, type, pricePerDay, description);
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to get all rooms: " << e.what() << std::endl;
    }
    return rooms;
}

/**
 * @brief Добавляет новый номер в базу данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param number Номер комнаты.
 * @param type Тип комнаты.
 * @param pricePerDay Цена за номер в день.
 * @param description Описание номера.
 * @return True, если номер успешно добавлен, иначе false.
 */
bool Room::addRoom(DBManager& dbManager, const std::string& number, const std::string& type,
                   double pricePerDay, const std::string& description) {
    try {
        std::string query = "INSERT INTO rooms (number, type, price_per_day, description) VALUES ('" +
                            number + "', '" + type + "', " + std::to_string(pricePerDay) + ", '" + description + "');";
        dbManager.executeUpdate(query);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to add room: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Находит номер по его идентификатору в базе данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param id Идентификатор номера для поиска.
 * @return Уникальный указатель на объект Room, если номер найден, иначе nullptr.
 */
std::unique_ptr<Room> Room::findRoomById(DBManager& dbManager, int id) {
    try {
        std::string query = "SELECT number, type, price_per_day, description FROM rooms WHERE id = " + std::to_string(id) + ";";
        PGResultWrapper result = dbManager.executeQuery(query);

        if (PQntuples(result.get()) == 1) {
            std::string number = PQgetvalue(result.get(), 0, 0);
            std::string type = PQgetvalue(result.get(), 0, 1);
            double pricePerDay = std::stod(PQgetvalue(result.get(), 0, 2));
            std::string description = PQgetvalue(result.get(), 0, 3);
            
            auto room = std::make_unique<Room>(id, number, type, pricePerDay, description);
            return room; 
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to find room by ID: " << e.what() << std::endl;
    }
    return nullptr;
}

/**
 * @brief Находит номер по его номеру комнаты в базе данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param number Номер комнаты для поиска.
 * @return Уникальный указатель на объект Room, если номер найден, иначе nullptr.
 */
std::unique_ptr<Room> Room::findRoomByNumber(DBManager& dbManager, const std::string& number) {
     try {
        std::string query = "SELECT id, type, price_per_day, description FROM rooms WHERE number = '" + number + "';";
        PGResultWrapper result = dbManager.executeQuery(query);

        if (PQntuples(result.get()) == 1) {
            int id = std::stoi(PQgetvalue(result.get(), 0, 0));
            std::string type = PQgetvalue(result.get(), 0, 1);
            double pricePerDay = std::stod(PQgetvalue(result.get(), 0, 2));
            std::string description = PQgetvalue(result.get(), 0, 3);
            
            auto room = std::make_unique<Room>(id, number, type, pricePerDay, description);
            return room; 
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to find room by number: " << e.what() << std::endl;
    }
    return nullptr;
} 
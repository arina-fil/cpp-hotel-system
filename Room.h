/**
 * @file Room.h
 * @brief Этот файл содержит объявление класса Room, представляющего собой номер в отеле.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "DBManager.h"

/**
 * @brief Класс Room представляет собой номер в отеле.
 * Он содержит информацию об идентификаторе, номере, типе, цене за день и описании номера.
 */
class Room {
private:
    int id;
    std::string number;
    std::string type;
    double pricePerDay;
    std::string description;

public:
    /**
     * @brief Конструктор для создания нового объекта Room.
     * @param id Идентификатор номера.
     * @param number Номер комнаты.
     * @param type Тип комнаты.
     * @param pricePerDay Цена за номер в день.
     * @param description Описание номера.
     */
    Room(int id, const std::string& number, const std::string& type, 
         double pricePerDay, const std::string& description);
    
    /**
     * @brief Возвращает идентификатор номера.
     * @return Идентификатор номера.
     */
    int getId() const;
        
    /**
     * @brief Возвращает номер комнаты.
     * @return Номер комнаты.
     */
    std::string getNumber() const;
        
    /**
     * @brief Возвращает тип комнаты.
     * @return Тип комнаты.
     */
    std::string getType() const;
        
    /**
     * @brief Возвращает цену за номер в день.
     * @return Цена за номер в день.
     */
    double getPricePerDay() const;

    /**
     * @brief Возвращает описание номера.
     * @return Описание номера.
     */
    std::string getDescription() const;
    
    /**
     * @brief Получает список всех номеров из базы данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @return Вектор объектов Room, представляющих все номера.
     */
    static std::vector<Room> getAllRooms(DBManager& dbManager);

    /**
     * @brief Добавляет новый номер в базу данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param number Номер комнаты.
     * @param type Тип комнаты.
     * @param pricePerDay Цена за номер в день.
     * @param description Описание номера.
     * @return True, если номер успешно добавлен, иначе false.
     */
    static bool addRoom(DBManager& dbManager, const std::string& number, const std::string& type, 
                        double pricePerDay, const std::string& description);
    /**
     * @brief Находит номер по его идентификатору в базе данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param id Идентификатор номера для поиска.
     * @return Уникальный указатель на объект Room, если номер найден, иначе nullptr.
     */
    static std::unique_ptr<Room> findRoomById(DBManager& dbManager, int id);
        
    /**
     * @brief Находит номер по его номеру комнаты в базе данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param number Номер комнаты для поиска.
     * @return Уникальный указатель на объект Room, если номер найден, иначе nullptr.
     */
    static std::unique_ptr<Room> findRoomByNumber(DBManager& dbManager, const std::string& number);
}; 
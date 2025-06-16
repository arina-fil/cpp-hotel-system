/**
 * @file Service.h
 * @brief Этот файл содержит объявление класса Service, представляющего собой услугу в отеле.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "DBManager.h"

/**
 * @brief Класс Service представляет собой услугу, предоставляемую отелем.
 * Он содержит информацию об идентификаторе, названии и цене услуги.
 */
class Service {
private:
    int id;
    std::string name;
    double price;

public:
    /**
     * @brief Конструктор для создания нового объекта Service.
     * @param id Идентификатор услуги.
     * @param name Название услуги.
     * @param price Цена услуги.
     */
    Service(int id, const std::string& name, double price);

    /**
     * @brief Возвращает идентификатор услуги.
     * @return Идентификатор услуги.
     */
    int getId() const;

        /**
     * @brief Возвращает название услуги.
     * @return Название услуги.
     */
    std::string getName() const;

    /**
     * @brief Возвращает цену услуги.
     * @return Цена услуги.
     */
    double getPrice() const;

    /**
     * @brief Получает список всех услуг из базы данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @return Вектор объектов Service, представляющих все услуги.
     */
    static std::vector<Service> getAllServices(DBManager& dbManager);

    /**
     * @brief Добавляет новую услугу в базу данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param name Название новой услуги.
     * @param price Цена новой услуги.
     * @return True, если услуга успешно добавлена, иначе false.
     */
    static bool addService(DBManager& dbManager, const std::string& name, double price);

    /**
     * @brief Находит услугу по ее идентификатору в базе данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param id Идентификатор услуги для поиска.
     * @return Уникальный указатель на объект Service, если услуга найдена, иначе nullptr.
     */
    static std::unique_ptr<Service> findServiceById(DBManager& dbManager, int id);
}; 
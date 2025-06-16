/**
 * @file Service.cpp
 * @brief Этот файл содержит реализацию класса Service.
 */
#include "Service.h"
#include "DBManager.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Конструктор класса Service.
 * @param id Уникальный идентификатор услуги.
 * @param name Название услуги.
 * @param price Цена услуги.
 */
Service::Service(int id, const std::string& name, double price)
    : id(id), name(name), price(price) {}

/**
 * @brief Возвращает идентификатор услуги.
 * @return Идентификатор услуги.
 */
int Service::getId() const { return id; }

/**
 * @brief Возвращает название услуги.
 * @return Название услуги.
 */
std::string Service::getName() const { return name; }

/**
 * @brief Возвращает цену услуги.
 * @return Цена услуги.
 */
double Service::getPrice() const { return price; }

/**
 * @brief Получает список всех услуг из базы данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @return Вектор объектов Service, представляющих все услуги.
 */
std::vector<Service> Service::getAllServices(DBManager& dbManager) {
    std::vector<Service> services;
    try {
        std::string query = "SELECT id, name, price FROM services;";
        PGResultWrapper result = dbManager.executeQuery(query);

        for (int i = 0; i < PQntuples(result.get()); i++) {
            int id = std::stoi(PQgetvalue(result.get(), i, 0));
            std::string name = PQgetvalue(result.get(), i, 1);
            double price = std::stod(PQgetvalue(result.get(), i, 2));
            services.emplace_back(id, name, price);
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to get all services: " << e.what() << std::endl;
    }
    return services;
}

/**
 * @brief Добавляет новую услугу в базу данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param name Название новой услуги.
 * @param price Цена новой услуги.
 * @return True, если услуга успешно добавлена, иначе false.
 */
bool Service::addService(DBManager& dbManager, const std::string& name, double price) {
    try {
        std::string query = "INSERT INTO services (name, price) VALUES ('" +
                            name + "', " + std::to_string(price) + ");";
        dbManager.executeUpdate(query);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to add service: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Находит услугу по ее идентификатору в базе данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param id Идентификатор услуги для поиска.
 * @return Уникальный указатель на объект Service, если услуга найдена, иначе nullptr.
 */
std::unique_ptr<Service> Service::findServiceById(DBManager& dbManager, int id) {
    try {
        std::string query = "SELECT name, price FROM services WHERE id = " + std::to_string(id) + ";";
        PGResultWrapper result = dbManager.executeQuery(query);

        if (PQntuples(result.get()) == 1) {
            std::string name = PQgetvalue(result.get(), 0, 0);
            double price = std::stod(PQgetvalue(result.get(), 0, 1));
            
            auto service = std::make_unique<Service>(id, name, price);
            return service;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to find service by ID: " << e.what() << std::endl;
    }
    return nullptr;
} 
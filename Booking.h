/**
 * @file Booking.h
 * @brief Этот файл содержит объявление класса Booking, представляющего собой бронирование номера в отеле.
 *        Также содержит перечисление BookingStatus для статусов бронирования. ДАННЫЙ КОД БЫЛ ВЗЯТ ИЗ СТОРОННИХ ИСТОЧНИКОВ
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "DBManager.h"
#include "User.h"
#include "Room.h"
#include "Service.h"

/**
 * @brief Перечисление, определяющее возможные статусы бронирования.
 */
enum class BookingStatus {
    PENDING,
    CONFIRMED,
    CANCELLED,
    COMPLETED
};

/**
 * @brief Класс Booking представляет собой запись о бронировании номера в отеле.
 * Он содержит информацию о бронировании, такую как пользователь, номер, даты,
 * статус и связанные услуги.
 */
class Booking {
private:
    int id;
    int userId;
    int roomId;
    std::string dateFrom;
    std::string dateTo;
    BookingStatus status;

public:
    /**
     * @brief Конструктор для создания нового объекта Booking.
     * @param id Идентификатор бронирования.
     * @param userId Идентификатор пользователя.
     * @param roomId Идентификатор номера.
     * @param dateFrom Дата начала бронирования.
     * @param dateTo Дата окончания бронирования.
     * @param status Статус бронирования.
     */
    Booking(int id, int userId, int roomId, const std::string& dateFrom, 
            const std::string& dateTo, BookingStatus status);
    
    /**
     * @brief Возвращает идентификатор бронирования.
     * @return Идентификатор бронирования.
     */
    int getId() const;

    /**
     * @brief Возвращает идентификатор пользователя, связанного с бронированием.
     * @return Идентификатор пользователя.
     */
    int getUserId() const;

    /**
     * @brief Возвращает идентификатор номера, связанного с бронированием.
     * @return Идентификатор номера.
     */
    int getRoomId() const;

    /**
     * @brief Возвращает дату начала бронирования.
     * @return Строка с датой начала бронирования.
     */
    std::string getDateFrom() const;

    /**
     * @brief Возвращает дату окончания бронирования.
     * @return Строка с датой окончания бронирования.
     */
    std::string getDateTo() const;

    /**
     * @brief Возвращает текущий статус бронирования.
     * @return Статус бронирования (BookingStatus).
     */
    BookingStatus getStatus() const;

    /**
     * @brief Возвращает строковое представление текущего статуса бронирования.
     * @return Строка, представляющая статус бронирования.
     */
    std::string getStatusString() const;
    
    /**
     * @brief Получает все услуги, связанные с данным бронированием, из базы данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @return Карта, где ключ - ID услуги, значение - количество.
     */
    std::map<int, int> getServices(DBManager& dbManager);

    /**
     * @brief Добавляет услугу к данному бронированию в базе данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param serviceId Идентификатор услуги для добавления.
     * @param quantity Количество добавляемой услуги.
     */
    void addService(DBManager& dbManager, int serviceId, int quantity);

    /**
     * @brief Удаляет услугу из данного бронирования в базе данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param serviceId Идентификатор услуги для удаления.
     */
    void removeService(DBManager& dbManager, int serviceId);
    
    /**
     * @brief Обновляет статус данного бронирования в базе данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param newStatus Новый статус для установки.
     */
    void updateStatus(DBManager& dbManager, BookingStatus newStatus);
    
    /**
     * @brief Находит бронирование по его идентификатору в базе данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param id Идентификатор бронирования для поиска.
     * @return Уникальный указатель на объект Booking, если бронирование найдено, иначе nullptr.
     */
    static std::unique_ptr<Booking> findBookingById(DBManager& dbManager, int id);

    /**
     * @brief Получает список всех бронирований из базы данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @return Вектор объектов Booking, представляющих все бронирования.
     */
    static std::vector<Booking> getAllBookings(DBManager& dbManager);

    /**
     * @brief Находит бронирования по идентификатору пользователя в базе данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param userId Идентификатор пользователя для поиска бронирований.
     * @return Вектор объектов Booking, связанных с указанным пользователем.
     */
    static std::vector<Booking> findBookingsByUserId(DBManager& dbManager, int userId);

    /**
     * @brief Проверяет доступность номера на указанные даты.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param roomId Идентификатор номера для проверки.
     * @param dateFrom Дата начала проверки доступности.
     * @param dateTo Дата окончания проверки доступности.
     * @return True, если номер доступен, иначе false.
     */
    static bool isRoomAvailable(DBManager& dbManager, int roomId, const std::string& dateFrom, const std::string& dateTo);

    /**
     * @brief Создает новое бронирование в базе данных.
     * @param dbManager Менеджер базы данных для взаимодействия с БД.
     * @param userId Идентификатор пользователя.
     * @param roomId Идентификатор номера.
     * @param dateFrom Дата начала бронирования.
     * @param dateTo Дата окончания бронирования.
     * @return Уникальный указатель на созданный объект Booking, если бронирование успешно создано, иначе nullptr.
     */
    static std::unique_ptr<Booking> createBooking(DBManager& dbManager, int userId, int roomId, const std::string& dateFrom, const std::string& dateTo);
}; 
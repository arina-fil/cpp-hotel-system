/**
 * @file Booking.cpp
 * @brief Этот файл содержит реализацию класса Booking. ДАННЫЙ КОД БЫЛ ВЗЯТЬ ИЗ СТОРОННИХ ИСТОЧНИКОВ
 */
#include "Booking.h"
#include "DBManager.h"
#include <iostream>
#include <memory>

/**
 * @brief Вспомогательная функция для преобразования строкового представления статуса бронирования в перечисление BookingStatus.
 * @param statusStr Строковое представление статуса (например, "pending", "confirmed").
 * @return Соответствующее значение перечисления BookingStatus. Если строка не распознана, возвращает BookingStatus::PENDING.
 */
BookingStatus toBookingStatus(const std::string& statusStr) {
    if (statusStr == "pending") return BookingStatus::PENDING;
    if (statusStr == "confirmed") return BookingStatus::CONFIRMED;
    if (statusStr == "cancelled") return BookingStatus::CANCELLED;
    if (statusStr == "completed") return BookingStatus::COMPLETED;
    return BookingStatus::PENDING; // Default
}

/**
 * @brief Конструктор класса Booking.
 * @param id Уникальный идентификатор бронирования.
 * @param userId Идентификатор пользователя, создавшего бронирование.
 * @param roomId Идентификатор забронированного номера.
 * @param dateFrom Дата начала бронирования в формате YYYY-MM-DD.
 * @param dateTo Дата окончания бронирования в формате YYYY-MM-DD.
 * @param status Текущий статус бронирования.
 */
Booking::Booking(int id, int userId, int roomId, const std::string& dateFrom, 
                 const std::string& dateTo, BookingStatus status)
    : id(id), userId(userId), roomId(roomId), dateFrom(dateFrom), dateTo(dateTo), status(status) {}

/**
 * @brief Возвращает идентификатор бронирования.
 * @return Идентификатор бронирования.
 */
int Booking::getId() const { return id; }

/**
 * @brief Возвращает идентификатор пользователя, связанного с бронированием.
 * @return Идентификатор пользователя.
 */
int Booking::getUserId() const { return userId; }

/**
 * @brief Возвращает идентификатор номера, связанного с бронированием.
 * @return Идентификатор номера.
 */
int Booking::getRoomId() const { return roomId; }

/**
 * @brief Возвращает дату начала бронирования.
 * @return Строка с датой начала бронирования.
 */
std::string Booking::getDateFrom() const { return dateFrom; }

/**
 * @brief Возвращает дату окончания бронирования.
 * @return Строка с датой окончания бронирования.
 */
std::string Booking::getDateTo() const { return dateTo; }

/**
 * @brief Возвращает текущий статус бронирования.
 * @return Статус бронирования (BookingStatus).
 */
BookingStatus Booking::getStatus() const { return status; }

/**
 * @brief Возвращает строковое представление текущего статуса бронирования.
 * @return Строка, представляющая статус бронирования.
 */
std::string Booking::getStatusString() const {
    switch (status) {
        case BookingStatus::PENDING: return "pending";
        case BookingStatus::CONFIRMED: return "confirmed";
        case BookingStatus::CANCELLED: return "cancelled";
        case BookingStatus::COMPLETED: return "completed";
        default: return "unknown";
    }
}

/**
 * @brief Получает все услуги, связанные с данным бронированием, из базы данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @return Карта, где ключ - ID услуги, значение - количество.
 */
std::map<int, int> Booking::getServices(DBManager& dbManager) {
    std::map<int, int> servicesMap;
    std::string query = "SELECT service_id, quantity FROM booking_services WHERE booking_id = " + std::to_string(id) + ";";
    PGResultWrapper result = dbManager.executeQuery(query);
    for (int i = 0; i < PQntuples(result.get()); ++i) {
        servicesMap[std::stoi(PQgetvalue(result.get(), i, 0))] = std::stoi(PQgetvalue(result.get(), i, 1));
    }
    return servicesMap; // PGResultWrapper automatically cleans up
}

/**
 * @brief Добавляет услугу к данному бронированию в базе данных.
 * Если услуга уже существует, обновляет количество.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param serviceId Идентификатор услуги для добавления.
 * @param quantity Количество добавляемой услуги.
 */
void Booking::addService(DBManager& dbManager, int serviceId, int quantity) {
    std::string query = "INSERT INTO booking_services (booking_id, service_id, quantity) VALUES (" +
                        std::to_string(id) + ", " + std::to_string(serviceId) + ", " + std::to_string(quantity) +
                        ") ON CONFLICT (booking_id, service_id) DO UPDATE SET quantity = " + std::to_string(quantity) + ";";
    dbManager.executeUpdate(query);
}

/**
 * @brief Удаляет услугу из данного бронирования в базе данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param serviceId Идентификатор услуги для удаления.
 */
void Booking::removeService(DBManager& dbManager, int serviceId) {
    std::string query = "DELETE FROM booking_services WHERE booking_id = " + std::to_string(id) + 
                        " AND service_id = " + std::to_string(serviceId) + ";";
    dbManager.executeUpdate(query);
}

/**
 * @brief Обновляет статус данного бронирования в базе данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param newStatus Новый статус для установки.
 */
void Booking::updateStatus(DBManager& dbManager, BookingStatus newStatus) {
    this->status = newStatus;
    std::string query = "UPDATE bookings SET status = '" + getStatusString() + "' WHERE id = " + std::to_string(id) + ";";
    dbManager.executeUpdate(query);
}

/**
 * @brief Находит бронирование по его идентификатору в базе данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param id Идентификатор бронирования для поиска.
 * @return Уникальный указатель на объект Booking, если бронирование найдено, иначе nullptr.
 */
std::unique_ptr<Booking> Booking::findBookingById(DBManager& dbManager, int id) {
    std::string query = "SELECT user_id, room_id, date_from, date_to, status FROM bookings WHERE id = " + std::to_string(id) + ";";
    PGResultWrapper result = dbManager.executeQuery(query);
    if (PQntuples(result.get()) == 1) {
        auto booking = std::make_unique<Booking>(
            id,
            std::stoi(PQgetvalue(result.get(), 0, 0)),
            std::stoi(PQgetvalue(result.get(), 0, 1)),
            PQgetvalue(result.get(), 0, 2),
            PQgetvalue(result.get(), 0, 3),
            toBookingStatus(PQgetvalue(result.get(), 0, 4))
        );
        return booking;
    }
    return nullptr; 
}

/**
 * @brief Получает список всех бронирований из базы данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @return Вектор объектов Booking, представляющих все бронирования.
 */
std::vector<Booking> Booking::getAllBookings(DBManager& dbManager) {
    std::vector<Booking> bookings;
    std::string query = "SELECT id, user_id, room_id, date_from, date_to, status FROM bookings;";
    PGResultWrapper result = dbManager.executeQuery(query);
    for (int i = 0; i < PQntuples(result.get()); i++) {
        bookings.emplace_back(
            std::stoi(PQgetvalue(result.get(), i, 0)),
            std::stoi(PQgetvalue(result.get(), i, 1)),
            std::stoi(PQgetvalue(result.get(), i, 2)),
            PQgetvalue(result.get(), i, 3),
            PQgetvalue(result.get(), i, 4),
            toBookingStatus(PQgetvalue(result.get(), i, 5))
        );
    }
    return bookings;
}

/**
 * @brief Находит бронирования по идентификатору пользователя в базе данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param userId Идентификатор пользователя для поиска бронирований.
 * @return Вектор объектов Booking, связанных с указанным пользователем.
 */
std::vector<Booking> Booking::findBookingsByUserId(DBManager& dbManager, int userId) {
    std::vector<Booking> bookings;
    std::string query = "SELECT id, room_id, date_from, date_to, status FROM bookings WHERE user_id = " + std::to_string(userId) + ";";
    PGResultWrapper result = dbManager.executeQuery(query);
    for (int i = 0; i < PQntuples(result.get()); i++) {
        bookings.emplace_back(
            std::stoi(PQgetvalue(result.get(), i, 0)),
            userId,
            std::stoi(PQgetvalue(result.get(), i, 1)),
            PQgetvalue(result.get(), i, 2),
            PQgetvalue(result.get(), i, 3),
            toBookingStatus(PQgetvalue(result.get(), i, 4))
        );
    }
    return bookings;
}

/**
 * @brief Проверяет доступность номера на указанные даты.
 * Номер считается недоступным, если существует бронирование (не отмененное), которое пересекается с желаемым диапазоном дат.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param roomId Идентификатор номера для проверки.
 * @param dateFrom Дата начала проверки доступности.
 * @param dateTo Дата окончания проверки доступности.
 * @return True, если номер доступен, иначе false.
 */
bool Booking::isRoomAvailable(DBManager& dbManager, int roomId, const std::string& dateFrom, const std::string& dateTo) {
    std::string query = "SELECT COUNT(*) FROM bookings WHERE room_id = " + std::to_string(roomId) +
                        " AND status <> 'cancelled' AND (date_from, date_to) OVERLAPS ('" + dateFrom + "', '" + dateTo + "');";
    PGResultWrapper result = dbManager.executeQuery(query);
    bool isAvailable = (std::stoi(PQgetvalue(result.get(), 0, 0)) == 0);
    return isAvailable; 
}

/**
 * @brief Создает новое бронирование в базе данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param userId Идентификатор пользователя.
 * @param roomId Идентификатор номера.
 * @param dateFrom Дата начала бронирования.
 * @param dateTo Дата окончания бронирования.
 * @return Уникальный указатель на созданный объект Booking, если бронирование успешно создано, иначе nullptr.
 */
std::unique_ptr<Booking> Booking::createBooking(DBManager& dbManager, int userId, int roomId, const std::string& dateFrom, const std::string& dateTo) {
    if (!isRoomAvailable(dbManager, roomId, dateFrom, dateTo)) {
        return nullptr; 
    }
    std::string query = "INSERT INTO bookings (user_id, room_id, date_from, date_to, status) VALUES (" +
                        std::to_string(userId) + ", " + std::to_string(roomId) + ", '" + dateFrom + "', '" + dateTo + "', 'pending') RETURNING id;";
    PGResultWrapper result = dbManager.executeQuery(query);
    if (PQntuples(result.get()) == 1) {
        int newId = std::stoi(PQgetvalue(result.get(), 0, 0));
        return findBookingById(dbManager, newId); 
    }
    return nullptr; 
} 
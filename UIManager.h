/**
 * @file UIManager.h
 * @brief Этот файл содержит объявления функций для управления пользовательским интерфейсом.
 *        Включает функции для отображения меню, управления пользователями, бронированиями, номерами и услугами.
 */

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <string>

class DBManager;

/**
 * @brief Отображает главное меню приложения.
 */
void showMainMenu();

/**
 * @brief Отображает меню администратора.
 */
void showAdminMenu();

/**
 * @brief Отображает меню обычного пользователя.
 */
void showManagerMenu();

/**
 * @brief Отображает меню обычного пользователя.
 */
void showUserMenu();

/**
 * @brief Выполняет вход пользователя в систему.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 * @return True, если вход выполнен успешно, иначе false.
 */
bool login(DBManager& db);

/**
 * @brief Регистрирует нового пользователя в системе.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 * @return True, если регистрация успешна, иначе false.
 */
bool registerUser(DBManager& db);

/**
 * @brief Просматривает все бронирования в системе.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewAllBookings(DBManager& db);

/**
 * @brief Управляет конкретным бронированием (например, изменением статуса).
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void manageBooking(DBManager& db);

/**
 * @brief Добавляет услугу к существующему бронированию.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void addServiceToBooking(DBManager& db);

/**
 * @brief Рассчитывает счет для бронирования.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void calculateBill(DBManager& db);

/**
 * @brief Просматривает все номера в отеле.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewAllRooms(DBManager& db);

/**
 * @brief Добавляет новый номер в отель.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void addRoom(DBManager& db);

/**
 * @brief Просматривает все доступные услуги.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewAllServices(DBManager& db);

/**
 * @brief Добавляет новую услугу.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void addService(DBManager& db);

/**
 * @brief Просматривает доступные номера для бронирования.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewAvailableRooms(DBManager& db);

/**
 * @brief Создает новое бронирование.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void makeBooking(DBManager& db);

/**
 * @brief Просматривает бронирования текущего пользователя.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewMyBookings(DBManager& db);

/**
 * @brief Получает от пользователя валидную дату.
 * @return Строка, содержащая валидную дату в формате YYYY-MM-DD.
 */
std::string getValidDate();

/**
 * @brief Управляет ролями пользователей (доступно только администраторам).
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void manageUserRoles(DBManager& db);

#endif // UIMANAGER_H 
/**
 * @file User.h
 * @brief Этот файл содержит объявление класса User, представляющего пользователя системы.
 *        Также содержит перечисление UserRole для ролей пользователей.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "DBManager.h"

/**
 * @brief Перечисление, определяющее возможные роли пользователя в системе.
 */
enum class UserRole {
    ADMIN,
    MANAGER,
    USER
};

/**
 * @brief Класс User представляет пользователя системы.
 * Он содержит информацию об идентификаторе, логине, пароле, роли пользователя.
 * Также предоставляет статические методы для управления пользователями и текущей сессией.
 */
class User {
private:
    int id;
    std::string login;
    std::string password;
    UserRole role;
    static std::unique_ptr<User> currentUser;

public:
    /**
     * @brief Конструктор для создания нового объекта User.
     * @param id Идентификатор пользователя.
     * @param login Логин пользователя.
     * @param password Пароль пользователя (предполагается, что он уже хэширован).
     * @param role Роль пользователя.
     */
    User(int id, const std::string& login, const std::string& password, UserRole role);
    
    /**
     * @brief Возвращает идентификатор пользователя.
     * @return Идентификатор пользователя.
     */
    int getId() const;

    /**
     * @brief Возвращает логин пользователя.
     * @return Логин пользователя.
     */
    const std::string& getLogin() const;

    /**
     * @brief Возвращает роль пользователя.
     * @return Роль пользователя (UserRole).
     */
    UserRole getRole() const;
    
    /**
     * @brief Возвращает строковое представление роли пользователя.
     * @return Строка, представляющая роль пользователя.
     */
    std::string getRoleString() const;
    
    /**
     * @brief Аутентифицирует пользователя по логину и паролю.
     * @param db Менеджер базы данных для взаимодействия с БД.
     * @param login Логин пользователя.
     * @param password Пароль пользователя.
     * @return True, если аутентификация успешна, иначе false.
     */
    static bool authenticate(DBManager& db, const std::string& login, const std::string& password);

    /**
     * @brief Добавляет нового пользователя в базу данных.
     * @param db Менеджер базы данных для взаимодействия с БД.
     * @param login Логин нового пользователя.
     * @param password Пароль нового пользователя.
     * @param role Роль нового пользователя.
     * @return True, если пользователь успешно добавлен, иначе false.
     */
    static bool addUser(DBManager& db, const std::string& login, const std::string& password, UserRole role);
        
    /**
     * @brief Находит пользователя по его идентификатору в базе данных.
     * @param db Менеджер базы данных для взаимодействия с БД.
     * @param id Идентификатор пользователя для поиска.
     * @return Уникальный указатель на объект User, если пользователь найден, иначе nullptr.
     */
    static std::unique_ptr<User> findUserById(DBManager& db, int id);
    
    /**
     * @brief Получает список всех пользователей из базы данных.
     * @param db Менеджер базы данных для взаимодействия с БД.
     * @return Вектор объектов User, представляющих всех пользователей.
     */
    static std::vector<User> getAllUsers(DBManager& db);
    
    /**
     * @brief Обновляет роль текущего пользователя в базе данных.
     * @param db Менеджер базы данных для взаимодействия с БД.
     * @param newRole Новая роль для установки.
     * @return True, если роль успешно обновлена, иначе false.
     */
    bool updateRole(DBManager& db, UserRole newRole);
    
    /**
     * @brief Возвращает указатель на текущего вошедшего пользователя.
     * @return Указатель на объект User текущего пользователя, или nullptr, если никто не вошел.
     */
    static User* getCurrentUser();

    /**
     * @brief Устанавливает текущего вошедшего пользователя.
     * @param user Уникальный указатель на объект User, который будет установлен как текущий пользователь.
     */
    static void setCurrentUser(std::unique_ptr<User> user);

    /**
     * @brief Выполняет выход текущего пользователя из системы.
     */
    static void logout();
}; 
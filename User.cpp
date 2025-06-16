/**
 * @file User.cpp
 * @brief Этот файл содержит реализацию класса User.
 */

#include "User.h"
#include "DBManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

/**
 * @brief Статическая переменная, представляющая текущего вошедшего в систему пользователя.
 * Инициализируется как nullptr, когда никто не вошел в систему.
 */
std::unique_ptr<User> User::currentUser = nullptr;

/**
 * @brief Конструктор класса User.
 * @param id Уникальный идентификатор пользователя.
 * @param login Логин пользователя.
 * @param password Хэшированный пароль пользователя.
 * @param role Роль пользователя.
 */
User::User(int id, const std::string& login, const std::string& password, UserRole role)
    : id(id), login(login), password(password), role(role) {
}

/**
 * @brief Возвращает идентификатор пользователя.
 * @return Идентификатор пользователя.
 */
int User::getId() const { return id; }

/**
 * @brief Возвращает логин пользователя.
 * @return Логин пользователя.
 */
const std::string& User::getLogin() const { return login; }

/**
 * @brief Возвращает роль пользователя.
 * @return Роль пользователя (UserRole).
 */
UserRole User::getRole() const { return role; }

/**
 * @brief Возвращает строковое представление роли пользователя.
 * @return Строка, представляющая роль пользователя.
 */
std::string User::getRoleString() const {
    switch (role) {
        case UserRole::ADMIN: return "admin";
        case UserRole::MANAGER: return "manager";
        case UserRole::USER: return "user";
        default: return "unknown";
    }
}

/**
 * @brief Аутентифицирует пользователя по логину и паролю, взаимодействуя с базой данных.
 * Если аутентификация успешна, устанавливает текущего пользователя.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param login Логин пользователя.
 * @param password Пароль пользователя (предполагается, что он уже хэширован).
 * @return True, если аутентификация успешна, иначе false.
 */
bool User::authenticate(DBManager& dbManager, const std::string& login, const std::string& password) {
    try {
        std::string query = "SELECT id, login, password_hash, role FROM users WHERE login = '" + login + "' AND password_hash = '" + password + "';";
        PGResultWrapper result = dbManager.executeQuery(query);

        if (PQntuples(result.get()) == 1) {
            int id = std::stoi(PQgetvalue(result.get(), 0, 0));
            std::string dbLogin = PQgetvalue(result.get(), 0, 1);
            std::string dbPassword = PQgetvalue(result.get(), 0, 2);
            std::string roleStr = PQgetvalue(result.get(), 0, 3);

            UserRole role;
            if (roleStr == "admin") role = UserRole::ADMIN;
            else if (roleStr == "manager") role = UserRole::MANAGER;
            else role = UserRole::USER;

            setCurrentUser(std::make_unique<User>(id, dbLogin, dbPassword, role));
            return true; // PGResultWrapper automatically cleans up
        }

        return false; // PGResultWrapper automatically cleans up
    } catch (const std::exception& e) {
        std::cerr << "Authentication failed: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Возвращает указатель на текущего вошедшего пользователя.
 * @return Указатель на объект User текущего пользователя, или nullptr, если никто не вошел.
 */
User* User::getCurrentUser() {
    return currentUser.get();
}

/**
 * @brief Устанавливает текущего вошедшего пользователя.
 * Предыдущий текущий пользователь будет удален.
 * @param user Уникальный указатель на объект User, который будет установлен как текущий пользователь.
 */
void User::setCurrentUser(std::unique_ptr<User> user) {
    currentUser = std::move(user);
}

/**
 * @brief Выполняет выход текущего пользователя из системы.
 * Удаляет объект текущего пользователя.
 */
void User::logout() {
    currentUser.reset();
}

/**
 * @brief Добавляет нового пользователя в базу данных.
 * Перед добавлением проверяет, существует ли пользователь с таким логином.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param login Логин нового пользователя.
 * @param password Пароль нового пользователя.
 * @param role Роль нового пользователя.
 * @return True, если пользователь успешно добавлен, иначе false (например, если пользователь с таким логином уже существует).
 */
bool User::addUser(DBManager& dbManager, const std::string& login, const std::string& password, UserRole role) {
    try {
        std::string checkQuery = "SELECT id FROM users WHERE login = '" + login + "';";
        PGResultWrapper result = dbManager.executeQuery(checkQuery);
        bool userExists = (PQntuples(result.get()) > 0);

        if (userExists) {
            std::cout << "User with login '" << login << "' already exists." << std::endl;
            return false;
        }
        
        std::string roleStr;
        switch (role) {
            case UserRole::ADMIN: roleStr = "admin"; break;
            case UserRole::MANAGER: roleStr = "manager"; break;
            case UserRole::USER: roleStr = "user"; break;
        }

        std::string insertQuery = "INSERT INTO users (login, password_hash, role) VALUES ('" + login + "', '" + password + "', '" + roleStr + "');";
        dbManager.executeUpdate(insertQuery);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to add user: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Находит пользователя по его идентификатору в базе данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param id Идентификатор пользователя для поиска.
 * @return Уникальный указатель на объект User, если пользователь найден, иначе nullptr.
 */
std::unique_ptr<User> User::findUserById(DBManager& dbManager, int id) {
    try {
        std::string query = "SELECT id, login, password_hash, role FROM users WHERE id = " + std::to_string(id) + ";";
        PGResultWrapper result = dbManager.executeQuery(query);

        if (PQntuples(result.get()) == 1) {
            int userId = std::stoi(PQgetvalue(result.get(), 0, 0));
            std::string dbLogin = PQgetvalue(result.get(), 0, 1);
            std::string dbPassword = PQgetvalue(result.get(), 0, 2);
            std::string roleStr = PQgetvalue(result.get(), 0, 3);

            UserRole role;
            if (roleStr == "admin") role = UserRole::ADMIN;
            else if (roleStr == "manager") role = UserRole::MANAGER;
            else role = UserRole::USER;

            auto user = std::make_unique<User>(userId, dbLogin, dbPassword, role);
            return user; 
        }

        return nullptr; 
    } catch (const std::exception& e) {
        std::cerr << "User lookup failed: " << e.what() << std::endl;
        return nullptr;
    }
}

/**
 * @brief Получает список всех пользователей из базы данных.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @return Вектор объектов User, представляющих всех пользователей.
 */
std::vector<User> User::getAllUsers(DBManager& dbManager) {
    std::vector<User> users;
    try {
        std::string query = "SELECT id, login, password_hash, role FROM users;";
        PGResultWrapper result = dbManager.executeQuery(query);

        int numRows = PQntuples(result.get());
        for (int i = 0; i < numRows; ++i) {
            int id = std::stoi(PQgetvalue(result.get(), i, 0));
            std::string login = PQgetvalue(result.get(), i, 1);
            std::string password = PQgetvalue(result.get(), i, 2);
            std::string roleStr = PQgetvalue(result.get(), i, 3);

            UserRole role;
            if (roleStr == "admin") role = UserRole::ADMIN;
            else if (roleStr == "manager") role = UserRole::MANAGER;
            else role = UserRole::USER;
            
            users.emplace_back(id, login, password, role);
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to get all users: " << e.what() << std::endl;
    }
    return users;
}

/**
 * @brief Обновляет роль текущего пользователя в базе данных и в текущем объекте.
 * @param dbManager Менеджер базы данных для взаимодействия с БД.
 * @param newRole Новая роль для установки.
 * @return True, если роль успешно обновлена, иначе false.
 */
bool User::updateRole(DBManager& dbManager, UserRole newRole) {
    try {
        std::string roleStr;
        switch (newRole) {
            case UserRole::ADMIN: roleStr = "admin"; break;
            case UserRole::MANAGER: roleStr = "manager"; break;
            case UserRole::USER: roleStr = "user"; break;
        }

        std::string query = "UPDATE users SET role = '" + roleStr + "' WHERE id = " + std::to_string(this->id) + ";";
        dbManager.executeUpdate(query);
        this->role = newRole; // Update role in the current object as well
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to update user role: " << e.what() << std::endl;
        return false;
    }
} 
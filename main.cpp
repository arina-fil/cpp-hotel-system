#include "DBManager.h"
#include "User.h"
#include "UIManager.h"
#include <iostream>
#include <exception>
#include <limits>
#include <memory>

/** @brief Точка входа. */
int main() {
    std::unique_ptr<DBManager> db;
    /**
     * @brief Установка соединения с базой данных PostgreSQL.
     * Оборачивается в try-catch для безопасной обработки ошибок подключения.
     */
    try {
        db = std::make_unique<DBManager>("127.0.0.1", "postgres", "dfvgbh04", "hotel_management", 5432);
        if (!db->connect()) {
            std::cerr << "FATAL: Failed to connect to database!" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "FATAL: DB connection error: " << e.what() << std::endl;
        return 1;
    }
    
    /**
     * @brief Основной цикл приложения. Показывает меню в зависимости от роли.
     */
    bool running = true;
    while (running) {
        User* currentUser = User::getCurrentUser();
        int choice = -1;

        if (currentUser == nullptr) {
            showMainMenu();
            std::cout << "Select action: ";
            std::cin >> choice;
            if(std::cin.fail()) {
                 std::cout << "Invalid input. Please enter a number." << std::endl;
                 std::cin.clear();
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 continue;
            }
            switch (choice) {
                case 1: login(*db); break;
                case 2: registerUser(*db); break;
                case 0: running = false; break;
                default: std::cout << "Invalid choice.\n"; break;
            }
        } else {
            switch (currentUser->getRole()) {
                case UserRole::ADMIN:   showAdminMenu();   break;
                case UserRole::MANAGER: showManagerMenu(); break;
                case UserRole::USER:    showUserMenu();    break;
            }
            std::cout << "Select action: ";
            std::cin >> choice;
            if(std::cin.fail()) {
                std::cout << "Invalid input. Please enter a number." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            switch (currentUser->getRole()) {
                case UserRole::ADMIN:
                    switch (choice) {
                        case 1: viewAllBookings(*db); break;
                        case 2: manageBooking(*db); break;
                        case 3: addServiceToBooking(*db); break;
                        case 4: calculateBill(*db); break;
                        case 5: manageUserRoles(*db); break;
                        case 6: registerUser(*db); break;
                        case 7: viewAllRooms(*db); break;
                        case 8: addRoom(*db); break;
                        case 9: viewAllServices(*db); break;
                        case 10: addService(*db); break;
                        case 0: User::logout(); break;
                        default: std::cout << "Invalid choice.\n"; break;
                    }
                    break;
                case UserRole::MANAGER:
                     switch (choice) {
                        case 1: viewAllBookings(*db); break;
                        case 2: manageBooking(*db); break;
                        case 3: calculateBill(*db); break;
                        case 4: viewAllRooms(*db); break;
                        case 5: addRoom(*db); break;
                        case 6: viewAllServices(*db); break;
                        case 7: addService(*db); break;
                        case 0: User::logout(); break;
                        default: std::cout << "Invalid choice.\n"; break;
                    }
                    break;
                case UserRole::USER:
                     switch (choice) {
                        case 1: viewAvailableRooms(*db); break;
                        case 2: makeBooking(*db); break;
                        case 3: viewMyBookings(*db); break;
                        case 0: User::logout(); break;
                        default: std::cout << "Invalid choice.\n"; break;
                    }
                    break;
            }
        }
        std::cout << std::endl;
    }

    if (db) {
        db->disconnect();
    }
    
    User::logout();
    
    std::cout << "Thank you for using the Hotel Management System!" << std::endl;
    return 0;
}
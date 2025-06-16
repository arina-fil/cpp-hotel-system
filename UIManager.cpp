/**
 * @file UIManager.cpp
 * @brief Этот файл содержит реализацию функций для управления пользовательским интерфейсом отельной системы.
 */

#include "UIManager.h"
#include "DBManager.h"
#include "User.h"
#include "Room.h"
#include "Booking.h"
#include "Service.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <regex>

/**
 * @brief Отображает детали конкретного бронирования.
 * @param db Ссылка на объект DBManager для получения связанных данных (пользователь, комната).
 * @param booking Объект Booking для отображения.
 */
void displayBooking(DBManager& db, Booking& booking);

/**
 * @brief Отображает детали конкретного номера.
 * @param room Объект Room для отображения.
 */
void displayRoom(const Room& room);

/**
 * @brief Отображает детали конкретной услуги.
 * @param service Объект Service для отображения.
 */
void displayService(const Service& service);

/**
 * @brief Проверяет, является ли строка допустимой датой в формате YYYY-MM-DD.
 * @param date Строка для проверки.
 * @return True, если строка является действительной датой, иначе false.
 */
bool isValidDate(const std::string& date);


/**
 * @brief Отображает главное меню приложения, предоставляя опции входа или регистрации.
 */
void showMainMenu() {
    std::cout << "\n===== Hotel Management System =====\n"
              << "1. Login\n"
              << "2. Register\n"
              << "0. Exit\n"
              << "===================================\n";
}

/**
 * @brief Отображает меню для администратора, предоставляя различные опции управления.
 */
void showAdminMenu() {
    std::cout << "\n===== Admin Menu =====\n"
              << "1. View All Bookings\n"
              << "2. Manage Booking Status\n"
              << "3. Add Service to Booking\n"
              << "4. Calculate Bill\n"
              << "5. Manage User Roles\n"
              << "6. Register New User\n"
              << "7. View All Rooms\n"
              << "8. Add New Room\n"
              << "9. View All Services\n"
              << "10. Add New Service\n"
              << "0. Logout\n"
              << "======================\n";
}

/**
 * @brief Отображает меню для менеджера, предоставляя опции управления бронированиями, номерами и услугами.
 */
void showManagerMenu() {
    std::cout << "\n===== Manager Menu =====\n"
              << "1. View All Bookings\n"
              << "2. Manage Booking Status\n"
              << "3. Calculate Bill\n"
              << "4. View All Rooms\n"
              << "5. Add New Room\n"
              << "6. View All Services\n"
              << "7. Add New Service\n"
              << "0. Logout\n"
              << "========================\n";
}

/**
 * @brief Отображает меню для обычного пользователя, предоставляя опции просмотра доступных номеров, бронирования и просмотра своих бронирований.
 */
void showUserMenu() {
    std::cout << "\n===== User Menu =====\n"
              << "1. View Available Rooms\n"
              << "2. Make a Booking\n"
              << "3. View My Bookings\n"
              << "0. Logout\n"
              << "=====================\n";
}


/**
 * @brief Обрабатывает процесс входа пользователя в систему.
 * Запрашивает логин и пароль, затем пытается аутентифицировать пользователя через DBManager.
 * Если аутентификация успешна, устанавливает текущего пользователя.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 * @return True, если вход выполнен успешно, иначе false.
 */
bool login(DBManager& db) {
    std::string login, password;
    std::cout << "\n===== Login =====\nUsername: ";
    std::cin >> login;
    std::cout << "Password: ";
    std::cin >> password;
    
    if (User::authenticate(db, login, password)) {
        std::cout << "Login successful!" << std::endl;
        return true;
    }
    std::cout << "Invalid username or password." << std::endl;
    return false;
}

/**
 * @brief Обрабатывает процесс регистрации нового пользователя.
 * Если текущий пользователь является администратором, позволяет выбрать роль для нового пользователя.
 * В противном случае новый пользователь регистрируется с ролью USER по умолчанию.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 * @return True, если регистрация успешна, иначе false.
 */
bool registerUser(DBManager& db) {
    std::string login, password;
    UserRole role = UserRole::USER; // Default role

    User* admin = User::getCurrentUser();
    if (admin != nullptr && admin->getRole() == UserRole::ADMIN) {
        std::cout << "\n===== Admin: Register New User =====\n";
        int roleChoice = 0;
        std::cout << "Select a role for the new user:\n1. Admin\n2. Manager\n3. User\nEnter choice: ";
        std::cin >> roleChoice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input." << std::endl;
            return false;
        }

        switch (roleChoice) {
            case 1: role = UserRole::ADMIN; break;
            case 2: role = UserRole::MANAGER; break;
            case 3: role = UserRole::USER; break;
            default: std::cout << "Invalid role choice. Defaulting to USER.\n"; role = UserRole::USER; break;
        }

    } else {
        std::cout << "\n===== Registration =====\n";
    }

    std::cout << "Enter new username: ";
    std::cin >> login;
    std::cout << "Enter new password: ";
    std::cin >> password;
    
    if (User::addUser(db, login, password, role)) {
        std::cout << "Registration successful! You can now log in." << std::endl;
        return true;
    }
    return false;
}

/**
 * @brief Просматривает и отображает все бронирования, хранящиеся в базе данных.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewAllBookings(DBManager& db) {
    std::cout << "\n--- All Bookings ---" << std::endl;
    std::vector<Booking> bookings = Booking::getAllBookings(db);
    if (bookings.empty()) {
        std::cout << "No bookings found." << std::endl;
        return;
    }
    for (auto& booking : bookings) {
        displayBooking(db, booking);
    }
}

/**
 * @brief Позволяет менеджеру или администратору управлять статусом конкретного бронирования.
 * Запрашивает ID бронирования и новый статус.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void manageBooking(DBManager& db) {
    int bookingId;
    std::cout << "Enter booking ID to manage: ";
    std::cin >> bookingId;
    auto booking = Booking::findBookingById(db, bookingId);

    if (!booking) {
        std::cout << "Booking not found." << std::endl;
        return;
    }

    std::cout << "Current status: " << booking->getStatusString() << std::endl;
    std::cout << "Select new status: 1. Confirmed, 2. Cancelled, 3. Completed: ";
    int choice;
    std::cin >> choice;
    BookingStatus newStatus;
    switch (choice) {
        case 1: newStatus = BookingStatus::CONFIRMED; break;
        case 2: newStatus = BookingStatus::CANCELLED; break;
        case 3: newStatus = BookingStatus::COMPLETED; break;
        default: std::cout << "Invalid choice." << std::endl; return;
    }
    booking->updateStatus(db, newStatus);
    std::cout << "Booking status updated." << std::endl;
}

/**
 * @brief Позволяет менеджеру или администратору добавлять услуги к существующему бронированию.
 * Запрашивает ID бронирования, ID услуги и количество.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void addServiceToBooking(DBManager& db) {
    int bookingId, serviceId, quantity;
    std::cout << "Enter booking ID: ";
    std::cin >> bookingId;
    auto booking = Booking::findBookingById(db, bookingId);
     if (!booking) {
        std::cout << "Booking not found." << std::endl;
        return;
     }

    std::vector<Service> services = Service::getAllServices(db);
    for(const auto& s : services) displayService(s);

    std::cout << "Enter service ID to add: ";
    std::cin >> serviceId;
    std::cout << "Enter quantity: ";
    std::cin >> quantity;

    booking->addService(db, serviceId, quantity);
    std::cout << "Service added." << std::endl;
}

/**
 * @brief Рассчитывает и отображает итоговый счет для конкретного бронирования.
 * Учитывает стоимость номера и стоимость добавленных услуг.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void calculateBill(DBManager& db) {
    int bookingId;
    std::cout << "Enter booking ID to calculate bill: ";
    std::cin >> bookingId;
    auto booking = Booking::findBookingById(db, bookingId);
    if (!booking) {
        std::cout << "Booking not found." << std::endl;
        return;
    }

    auto room = Room::findRoomById(db, booking->getRoomId());
    if (!room) {
        std::cout << "Room associated with booking not found." << std::endl;
        return;
    }

    long days = 1;
    double roomCost = room->getPricePerDay() * days;
    double servicesCost = 0;

    std::cout << "\n--- Bill for Booking #" << booking->getId() << " ---" << std::endl;
    std::cout << "Room: " << room->getNumber() << " (" << room->getType() << ") for " << days << " day(s): $" << roomCost << std::endl;
    
    std::map<int, int> bookingServices = booking->getServices(db);
    if (!bookingServices.empty()) {
        std::cout << "Services:" << std::endl;
        for (auto const& [s_id, qty] : bookingServices) {
            auto service = Service::findServiceById(db, s_id);
            if (service) {
                double cost = service->getPrice() * qty;
                servicesCost += cost;
                std::cout << "  - " << service->getName() << " (x" << qty << "): $" << cost << std::endl;
            }
        }
    }
    std::cout << "--------------------" << std::endl;
    std::cout << "Total cost: $" << (roomCost + servicesCost) << std::endl;
}

/**
 * @brief Просматривает доступные номера в отеле на заданные даты.
 * Запрашивает даты заезда и выезда у пользователя.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewAvailableRooms(DBManager& db) {
    std::string dateFrom, dateTo;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter check-in date (YYYY-MM-DD): ";
    while (true) {
        std::getline(std::cin, dateFrom);
        if (isValidDate(dateFrom)) {
            break;
        }
        std::cout << "Invalid format. Please use YYYY-MM-DD: ";
    }

    std::cout << "Enter check-out date (YYYY-MM-DD): ";
    while (true) {
        std::getline(std::cin, dateTo);
        if (isValidDate(dateTo)) {
            break;
        }
        std::cout << "Invalid format. Please use YYYY-MM-DD: ";
    }

    std::cout << "\n--- Available Rooms ---" << std::endl;
    std::vector<Room> allRooms = Room::getAllRooms(db);
    bool anyAvailable = false;
    for (const auto& room : allRooms) {
        if (Booking::isRoomAvailable(db, room.getId(), dateFrom, dateTo)) {
            displayRoom(room);
            anyAvailable = true;
        }
    }
    if (!anyAvailable) {
        std::cout << "No rooms available for the selected dates." << std::endl;
    }
}

/**
 * @brief Позволяет пользователю создать новое бронирование.
 * Запрашивает ID комнаты и даты заезда/выезда, затем пытается создать бронирование.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void makeBooking(DBManager& db) {
    User* currentUser = User::getCurrentUser();
    if (!currentUser) {
        std::cout << "Error: You must be logged in to make a booking." << std::endl;
        return;
    }
    
    int roomId;
    std::string dateFrom, dateTo;

    std::cout << "Enter room ID to book: ";
    std::cin >> roomId;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid room ID entered." << std::endl;
        return;
    }
    
    auto room = Room::findRoomById(db, roomId);
    if (!room) {
        std::cout << "Room with ID " << roomId << " not found." << std::endl;
        return;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    std::cout << "Enter check-in date (YYYY-MM-DD): ";
    while (true) {
        std::getline(std::cin, dateFrom);
        if (isValidDate(dateFrom)) {
            break;
        }
        std::cout << "Invalid format. Please use YYYY-MM-DD: ";
    }

    std::cout << "Enter check-out date (YYYY-MM-DD): ";
    while (true) {
        std::getline(std::cin, dateTo);
        if (isValidDate(dateTo)) {
            break;
        }
        std::cout << "Invalid format. Please use YYYY-MM-DD: ";
    }

    try {
        auto newBooking = Booking::createBooking(db, currentUser->getId(), roomId, dateFrom, dateTo);
        if (newBooking) {
            std::cout << "Booking successful! Your booking ID is " << newBooking->getId() << std::endl;
        } else {
            std::cout << "Booking failed. The room is not available for these dates." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Booking error: " << e.what() << std::endl;
    }
}

/**
 * @brief Просматривает все бронирования текущего вошедшего пользователя.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewMyBookings(DBManager& db) {
    User* currentUser = User::getCurrentUser();
    if (!currentUser) {
        std::cout << "Error: You must be logged in to view bookings." << std::endl;
        return;
    }
    
    std::cout << "\n--- My Bookings ---" << std::endl;
    std::vector<Booking> bookings = Booking::findBookingsByUserId(db, currentUser->getId());
    if (bookings.empty()) {
        std::cout << "You have no bookings." << std::endl;
        return;
    }
    for (auto& booking : bookings) {
        displayBooking(db, booking);
    }
}

/**
 * @brief Отображает подробную информацию о номере.
 * @param room Объект Room для отображения.
 */
void displayRoom(const Room& room) {
    std::cout << "Room ID: " << room.getId() 
              << ", Number: " << room.getNumber()
              << ", Type: " << room.getType()
              << ", Price: $" << room.getPricePerDay() << std::endl;
}

/**
 * @brief Отображает подробную информацию о бронировании, включая связанный номер и статус.
 * @param db Ссылка на объект DBManager для получения информации о номере.
 * @param booking Объект Booking для отображения.
 */
void displayBooking(DBManager& db, Booking& booking) {
    std::cout << "\n--------------------" << std::endl;
    std::cout << "Booking ID: " << booking.getId() << std::endl;
    auto room = Room::findRoomById(db, booking.getRoomId());
    std::cout << "Room: " << (room ? room->getNumber() : "N/A") << std::endl;
    std::cout << "Dates: " << booking.getDateFrom() << " to " << booking.getDateTo() << std::endl;
    std::cout << "Status: " << booking.getStatusString() << std::endl;
    std::cout << "--------------------" << std::endl;
}

/**
 * @brief Отображает подробную информацию об услуге.
 * @param service Объект Service для отображения.
 */
void displayService(const Service& service) {
     std::cout << "Service ID: " << service.getId() 
              << ", Name: " << service.getName()
              << ", Price: $" << service.getPrice() << std::endl;
}

/**
 * @brief Проверяет, соответствует ли строка формату даты YYYY-MM-DD.
 * @param date Строка для проверки.
 * @return True, если формат даты корректен, иначе false.
 */
bool isValidDate(const std::string& date) {
    const std::regex pattern(R"((\d{4})-(\d{2})-(\d{2}))");
    return std::regex_match(date, pattern);
}

/**
 * @brief Позволяет администратору управлять ролями пользователей.
 * Отображает список пользователей и предлагает изменить их роли.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void manageUserRoles(DBManager& db) {
    std::cout << "\n--- User Role Management ---" << std::endl;
    std::vector<User> users = User::getAllUsers(db);

    if (users.empty()) {
        std::cout << "No users found in the system." << std::endl;
        return;
    }

    std::cout << "Users List:" << std::endl;
    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Login" << std::setw(10) << "Role" << std::endl;
    std::cout << "------------------------------------" << std::endl;
    for (const auto& user : users) {
        std::cout << std::left << std::setw(5) << user.getId() 
                  << std::setw(20) << user.getLogin() 
                  << std::setw(10) << user.getRoleString() << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;

    int userId;
    std::cout << "Enter User ID to modify (-1 to cancel): ";
    std::cin >> userId;

    if (std::cin.fail() || userId == -1) {
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Role management cancelled." << std::endl;
        return;
    }

    auto userToModify = User::findUserById(db, userId);
    if (!userToModify) {
        std::cout << "User with ID " << userId << " not found." << std::endl;
        return;
    }

    std::cout << "Selected user: " << userToModify->getLogin() << " (" << userToModify->getRoleString() << ")" << std::endl;
    std::cout << "Select new role: " << std::endl;
    std::cout << "1. Admin" << std::endl;
    std::cout << "2. Manager" << std::endl;
    std::cout << "3. User" << std::endl;
    std::cout << "0. Cancel" << std::endl;
    std::cout << "Enter your choice: ";
    int choice;
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input." << std::endl;
        return;
    }

    UserRole newRole;
    bool roleChanged = false;
    switch (choice) {
        case 1:
            newRole = UserRole::ADMIN;
            roleChanged = true;
            break;
        case 2:
            newRole = UserRole::MANAGER;
            roleChanged = true;
            break;
        case 3:
            newRole = UserRole::USER;
            roleChanged = true;
            break;
        case 0:
            std::cout << "Cancelled." << std::endl;
            break;
        default:
            std::cout << "Invalid choice." << std::endl;
            break;
    }

    if (roleChanged) {
        if (userToModify->updateRole(db, newRole)) {
            std::cout << "User role updated successfully." << std::endl;
        } else {
            std::cout << "Failed to update user role." << std::endl;
        }
    }
}

/**
 * @brief Просматривает и отображает список всех номеров в отеле.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewAllRooms(DBManager& db) {
    std::cout << "\n--- All Rooms ---" << std::endl;
    std::vector<Room> rooms = Room::getAllRooms(db);
    if (rooms.empty()) {
        std::cout << "No rooms found." << std::endl;
        return;
    }
    
    std::cout << std::left << std::setw(5) << "ID" << std::setw(10) << "Number" 
              << std::setw(15) << "Type" << std::setw(10) << "Price/Day" 
              << std::setw(30) << "Description" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    
    for (const auto& room : rooms) {
        std::cout << std::left << std::setw(5) << room.getId() 
                  << std::setw(10) << room.getNumber()
                  << std::setw(15) << room.getType()
                  << std::setw(10) << "$" + std::to_string(room.getPricePerDay())
                  << std::setw(30) << room.getDescription() << std::endl;
    }
}

/**
 * @brief Позволяет администратору или менеджеру добавить новый номер в отель.
 * Запрашивает детали нового номера у пользователя.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void addRoom(DBManager& db) {
    std::string number, type, description;
    double pricePerDay;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "\n--- Add New Room ---" << std::endl;
    
    std::cout << "Enter room number: ";
    std::getline(std::cin, number);
    
    std::cout << "Enter room type (single/double/suite/etc.): ";
    std::getline(std::cin, type);
    
    std::cout << "Enter price per day: $";
    std::cin >> pricePerDay;
    
    if (std::cin.fail() || pricePerDay < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid price entered." << std::endl;
        return;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter room description: ";
    std::getline(std::cin, description);
    
    if (Room::addRoom(db, number, type, pricePerDay, description)) {
        std::cout << "Room added successfully!" << std::endl;
    } else {
        std::cout << "Failed to add room. Room number might already exist." << std::endl;
    }
}

/**
 * @brief Просматривает и отображает список всех доступных услуг.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void viewAllServices(DBManager& db) {
    std::cout << "\n--- All Services ---" << std::endl;
    std::vector<Service> services = Service::getAllServices(db);
    if (services.empty()) {
        std::cout << "No services found." << std::endl;
        return;
    }
    
    std::cout << std::left << std::setw(5) << "ID" << std::setw(30) << "Name" 
              << std::setw(10) << "Price" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    
    for (const auto& service : services) {
        std::cout << std::left << std::setw(5) << service.getId() 
                  << std::setw(30) << service.getName()
                  << std::setw(10) << "$" + std::to_string(service.getPrice()) << std::endl;
    }
}

/**
 * @brief Позволяет администратору или менеджеру добавить новую услугу.
 * Запрашивает название и цену новой услуги у пользователя.
 * @param db Ссылка на объект DBManager для взаимодействия с базой данных.
 */
void addService(DBManager& db) {
    std::string name;
    double price;
    
    std::cout << "\n--- Add New Service ---" << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter service name: ";
    std::getline(std::cin, name);
    
    std::cout << "Enter service price: $";
    std::cin >> price;
    
    if (std::cin.fail() || price < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid price entered." << std::endl;
        return;
    }
    
    if (Service::addService(db, name, price)) {
        std::cout << "Service added successfully!" << std::endl;
    } else {
        std::cout << "Failed to add service. Service name might already exist." << std::endl;
    }
} 
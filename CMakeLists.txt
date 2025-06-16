cmake_minimum_required(VERSION 3.15)
project(HotelManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PostgreSQL REQUIRED)

file(GLOB CORE_SOURCES
    DBManager.cpp
    User.cpp
    Room.cpp
    Service.cpp
    Booking.cpp
    UIManager.cpp
)

add_library(hotel_system_core ${CORE_SOURCES})
target_include_directories(hotel_system_core PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(hotel_system_core PRIVATE ${PostgreSQL_LIBRARIES})
add_executable(hotel_management main.cpp)
target_include_directories(hotel_management PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(hotel_management PRIVATE hotel_system_core)
install(TARGETS hotel_management DESTINATION bin)
find_package(GTest REQUIRED)

if (GTest_FOUND)
    enable_testing()

    add_executable(all_tests
    tests/Booking_test.cpp
    tests/DBManager_test.cpp
    tests/Room_test.cpp
    tests/Service_test.cpp
    tests/User_test.cpp
)

    target_include_directories(all_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}  # это путь к корневой папке, где лежат .h (например Booking.h)
        ${PostgreSQL_INCLUDE_DIRS}
    )

    target_link_libraries(all_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        hotel_system_core
    )

    include(GoogleTest)
    gtest_discover_tests(all_tests)
endif()

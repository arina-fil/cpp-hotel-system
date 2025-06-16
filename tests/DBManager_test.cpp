#include "gtest/gtest.h"
#include "DBManager.h"

TEST(DBManagerTest, ConstructorInitializesCorrectly) {
    DBManager dbManager("localhost", "user", "password", "database", 5432);
    ASSERT_FALSE(dbManager.isConnected());
}

TEST(DBManagerTest, ExecuteQueryThrowsWhenNotConnected) {
    DBManager dbManager("localhost", "user", "password", "database", 5432);
    EXPECT_THROW({
        dbManager.executeQuery("SELECT 1;");
    }, std::runtime_error);
}

TEST(DBManagerTest, ExecuteUpdateThrowsWhenNotConnected) {
    DBManager dbManager("localhost", "user", "password", "database", 5432);
    EXPECT_THROW({
        dbManager.executeUpdate("INSERT INTO users VALUES (1);");
    }, std::runtime_error);
} 
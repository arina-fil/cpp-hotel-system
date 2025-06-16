#include "gtest/gtest.h"
#include "User.h"

TEST(UserTest, ConstructorAndGetters) {
    User user(1, "testuser", "testpass", UserRole::USER);

    ASSERT_EQ(user.getId(), 1);
    ASSERT_EQ(user.getLogin(), "testuser");
    ASSERT_EQ(user.getRole(), UserRole::USER);
}

TEST(UserTest, GetRoleStringConversion) {
    User adminUser(1, "admin", "pass", UserRole::ADMIN);
    User managerUser(2, "manager", "pass", UserRole::MANAGER);
    User regularUser(3, "user", "pass", UserRole::USER);

    ASSERT_EQ(adminUser.getRoleString(), "admin");
    ASSERT_EQ(managerUser.getRoleString(), "manager");
    ASSERT_EQ(regularUser.getRoleString(), "user");
} 
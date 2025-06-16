#include "gtest/gtest.h"
#include "Service.h"

TEST(ServiceTest, ConstructorAndGetters) {
    Service service(1, "Breakfast", 15.0);

    ASSERT_EQ(service.getId(), 1);
    ASSERT_EQ(service.getName(), "Breakfast");
    ASSERT_EQ(service.getPrice(), 15.0);
} 
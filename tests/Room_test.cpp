#include "gtest/gtest.h"
#include "Room.h"

TEST(RoomTest, ConstructorAndGetters) {
    Room room(1, "101", "Single", 50.0, "A cozy single room.");

    ASSERT_EQ(room.getId(), 1);
    ASSERT_EQ(room.getNumber(), "101");
    ASSERT_EQ(room.getType(), "Single");
    ASSERT_EQ(room.getPricePerDay(), 50.0);
    ASSERT_EQ(room.getDescription(), "A cozy single room.");
} 
#include "gtest/gtest.h"
#include "Booking.h"

BookingStatus toBookingStatus(const std::string& statusStr);

TEST(BookingTest, ConstructorAndGetters) {
    Booking booking(1, 101, 201, "2023-01-01", "2023-01-05", BookingStatus::PENDING);

    ASSERT_EQ(booking.getId(), 1);
    ASSERT_EQ(booking.getUserId(), 101);
    ASSERT_EQ(booking.getRoomId(), 201);
    ASSERT_EQ(booking.getDateFrom(), "2023-01-01");
    ASSERT_EQ(booking.getDateTo(), "2023-01-05");
    ASSERT_EQ(booking.getStatus(), BookingStatus::PENDING);
}

TEST(BookingTest, ToBookingStatusConversion) {
    ASSERT_EQ(toBookingStatus("pending"), BookingStatus::PENDING);
    ASSERT_EQ(toBookingStatus("confirmed"), BookingStatus::CONFIRMED);
    ASSERT_EQ(toBookingStatus("cancelled"), BookingStatus::CANCELLED);
    ASSERT_EQ(toBookingStatus("completed"), BookingStatus::COMPLETED);
    ASSERT_EQ(toBookingStatus("unknown"), BookingStatus::PENDING); // Default case
    ASSERT_EQ(toBookingStatus("anything_else"), BookingStatus::PENDING); // Another default case
}

TEST(BookingTest, GetStatusStringConversion) {
    Booking bookingPending(1, 1, 1, "d", "d", BookingStatus::PENDING);
    Booking bookingConfirmed(1, 1, 1, "d", "d", BookingStatus::CONFIRMED);
    Booking bookingCancelled(1, 1, 1, "d", "d", BookingStatus::CANCELLED);
    Booking bookingCompleted(1, 1, 1, "d", "d", BookingStatus::COMPLETED);

    ASSERT_EQ(bookingPending.getStatusString(), "pending");
    ASSERT_EQ(bookingConfirmed.getStatusString(), "confirmed");
    ASSERT_EQ(bookingCancelled.getStatusString(), "cancelled");
    ASSERT_EQ(bookingCompleted.getStatusString(), "completed");
} 
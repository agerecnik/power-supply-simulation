#include <gtest/gtest.h>
#include "../src/ramped_power_supply_impl.h"

TEST(ramped_power_supply_impl_test, on)
{
    RampedPowerSupplyImpl rpsi;
    EXPECT_TRUE(rpsi.on());
    EXPECT_TRUE(rpsi.isOn());
    EXPECT_FALSE(rpsi.on());
    EXPECT_TRUE(rpsi.isOn());
}

TEST(ramped_power_supply_impl_test, off)
{
    RampedPowerSupplyImpl rpsi;
    EXPECT_FALSE(rpsi.off());
    EXPECT_FALSE(rpsi.isOn());
    rpsi.on();
    EXPECT_TRUE(rpsi.off());
    EXPECT_FALSE(rpsi.isOn());
}

TEST(ramped_power_supply_impl_test, get_set)
{
    RampedPowerSupplyImpl rpsi;
    double current = 1000000;
    EXPECT_FALSE(rpsi.get(current));
    EXPECT_FALSE(rpsi.set(50));
    rpsi.on();
    EXPECT_TRUE(rpsi.get(current));
    EXPECT_EQ(current, 0);
    EXPECT_TRUE(rpsi.set(50));
    rpsi.get(current);
    EXPECT_EQ(current, 50);
    EXPECT_FALSE(rpsi.set(-1));
    rpsi.get(current);
    EXPECT_EQ(current, 50);
    rpsi.off();
    EXPECT_FALSE(rpsi.get(current));
    EXPECT_FALSE(rpsi.set(50));
}

TEST(ramped_power_supply_impl_test, current_zero_after_off)
{
    RampedPowerSupplyImpl rpsi;
    double current = 1000000;
    rpsi.on();
    rpsi.set(50);
    rpsi.get(current);
    EXPECT_EQ(current, 50);
    rpsi.off();
    rpsi.on();
    rpsi.get(current);
    EXPECT_EQ(current, 0);
}

TEST(ramped_power_supply_impl_test, ramp)
{
    RampedPowerSupplyImpl rpsi;
    std::vector<double> rampValues = {2, 3.5, 4.2, 6.7};
    std::uint32_t msec = 2000;
    double current = 0;
    EXPECT_FALSE(rpsi.loadRamp(rampValues));
    EXPECT_FALSE(rpsi.startRamp(msec));
    rpsi.on();
    EXPECT_FALSE(rpsi.startRamp(msec));
    EXPECT_TRUE(rpsi.loadRamp(rampValues));
    EXPECT_TRUE(rpsi.startRamp(msec));
    rpsi.get(current);
    EXPECT_EQ(current, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    EXPECT_FALSE(rpsi.off());
    EXPECT_FALSE(rpsi.reset());
    EXPECT_FALSE(rpsi.set(20));
    EXPECT_FALSE(rpsi.loadRamp(rampValues));
    EXPECT_FALSE(rpsi.startRamp(msec));
    rpsi.get(current);
    EXPECT_EQ(current, 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    rpsi.get(current);
    EXPECT_EQ(current, 3.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    rpsi.get(current);
    EXPECT_EQ(current, 4.2);
    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    rpsi.get(current);
    EXPECT_EQ(current, 6.7);

    rampValues = {2, 3.5, 4.2, 8};
    msec = 1;
    EXPECT_TRUE(rpsi.loadRamp(rampValues));
    EXPECT_TRUE(rpsi.startRamp(msec));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    rpsi.get(current);
    EXPECT_EQ(current, 8);

}

TEST(ramped_power_supply_impl_test, reset)
{
    RampedPowerSupplyImpl rpsi;
    std::vector<double> rampValues = {2, 3.5, 4.2, 6.7};
    std::uint32_t msec = 10;
    double current = 1000000;
    EXPECT_FALSE(rpsi.reset());
    rpsi.on();
    rpsi.loadRamp(rampValues);
    EXPECT_TRUE(rpsi.startRamp(msec));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    rpsi.get(current);
    EXPECT_EQ(current, 6.7);
    EXPECT_TRUE(rpsi.reset());
    rpsi.get(current);
    EXPECT_EQ(current, 0);
    EXPECT_FALSE(rpsi.startRamp(msec));
    rpsi.off();
    EXPECT_FALSE(rpsi.reset());
}
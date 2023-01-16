#include <gtest/gtest.h>
#include "../src/power_supply_impl.h"

TEST(power_supply_impl_test, on)
{
    PowerSupplyImpl psi;
    EXPECT_TRUE(psi.on());
    EXPECT_TRUE(psi.isOn());
    EXPECT_FALSE(psi.on());
    EXPECT_TRUE(psi.isOn());
}

TEST(power_supply_impl_test, off)
{
    PowerSupplyImpl psi;
    EXPECT_FALSE(psi.off());
    EXPECT_FALSE(psi.isOn());
    psi.on();
    EXPECT_TRUE(psi.off());
    EXPECT_FALSE(psi.isOn());
}

TEST(power_supply_impl_test, get_set)
{
    PowerSupplyImpl psi;
    double current = 1000000;
    EXPECT_FALSE(psi.get(current));
    EXPECT_FALSE(psi.set(50));
    psi.on();
    EXPECT_TRUE(psi.get(current));
    EXPECT_EQ(current, 0);
    EXPECT_TRUE(psi.set(50));
    psi.get(current);
    EXPECT_EQ(current, 50);
    EXPECT_FALSE(psi.set(-1));
    psi.get(current);
    EXPECT_EQ(current, 50);
    psi.off();
    EXPECT_FALSE(psi.get(current));
    EXPECT_FALSE(psi.set(50));
}

TEST(power_supply_impl_test, current_zero_after_off)
{
    PowerSupplyImpl psi;
    double current = 1000000;
    psi.on();
    psi.set(50);
    psi.get(current);
    EXPECT_EQ(current, 50);
    psi.off();
    psi.on();
    psi.get(current);
    EXPECT_EQ(current, 0);
}

TEST(power_supply_impl_test, reset)
{
    PowerSupplyImpl psi;
    double current = 1000000;
    EXPECT_FALSE(psi.reset());
    psi.on();
    psi.set(23);
    psi.get(current);
    EXPECT_EQ(current, 23);
    EXPECT_TRUE(psi.reset());
    psi.get(current);
    EXPECT_EQ(current, 0);
    psi.off();
    EXPECT_FALSE(psi.reset());
}
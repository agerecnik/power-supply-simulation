#include <gtest/gtest.h>
#include "../src/power_supply_narrow.h"
#include "../src/power_supply_impl.h"

TEST(power_supply_narrow_test, on_off)
{
    auto psi = std::make_shared<PowerSupplyImpl>();
    PowerSupplyNarrow psn(psi);
    std::vector<double> params = {};
    double currentValue;

    EXPECT_FALSE(psn.execute("on", params, currentValue));
    EXPECT_TRUE(psn.execute("off", params, currentValue));
    EXPECT_FALSE(psn.execute("off", params, currentValue));
    EXPECT_TRUE(psn.execute("on", params, currentValue));
    params.push_back(5.5);
    EXPECT_THROW(psn.execute("on", params, currentValue), std::invalid_argument);
    EXPECT_THROW(psn.execute("off", params, currentValue), std::invalid_argument);
}

TEST(power_supply_narrow_test, reset)
{
    auto psi = std::make_shared<PowerSupplyImpl>();
    PowerSupplyNarrow psn(psi);
    std::vector<double> params = {};
    double currentValue;

    EXPECT_TRUE(psn.execute("off", params, currentValue));
    EXPECT_EQ(psn.getState(), CircuitDevice::State::OK);
    EXPECT_FALSE(psn.execute("current_get", params, currentValue));
    EXPECT_EQ(psn.getState(), CircuitDevice::State::ERROR);
    EXPECT_FALSE(psn.execute("reset", params, currentValue));
    EXPECT_TRUE(psn.execute("on", params, currentValue));
    EXPECT_EQ(psn.getState(), CircuitDevice::State::ERROR);
    EXPECT_TRUE(psn.execute("reset", params, currentValue));
    EXPECT_EQ(psn.getState(), CircuitDevice::State::OK);
    params.push_back(5.5);
    EXPECT_THROW(psn.execute("reset", params, currentValue), std::invalid_argument);
}

TEST(power_supply_narrow_test, get_set)
{
    auto psi = std::make_shared<PowerSupplyImpl>();
    PowerSupplyNarrow psn(psi);
    std::vector<double> params1 = {};
    std::vector<double> params2 = {5.5};
    double currentValue = 10000;

    EXPECT_TRUE(psn.execute("current_get", params1, currentValue));
    EXPECT_EQ(currentValue, 0);
    psn.execute("off", params1, currentValue);
    EXPECT_EQ(psn.getState(), CircuitDevice::State::OK);
    EXPECT_FALSE(psn.execute("current_get", params1, currentValue));
    EXPECT_EQ(psn.getState(), CircuitDevice::State::ERROR);
    psn.execute("on", params1, currentValue);
    psn.execute("reset", params1, currentValue);

    EXPECT_TRUE(psn.execute("current_set", params2, currentValue));
    EXPECT_TRUE(psn.execute("current_get", params1, currentValue));
    EXPECT_EQ(currentValue, 5.5);
    psn.execute("off", params1, currentValue);
    EXPECT_EQ(psn.getState(), CircuitDevice::State::OK);
    EXPECT_FALSE(psn.execute("current_set", params2, currentValue));
    EXPECT_EQ(psn.getState(), CircuitDevice::State::ERROR);
    psn.execute("on", params1, currentValue);
    psn.execute("reset", params1, currentValue);

    params2.pop_back();
    params2.push_back(0);
    EXPECT_TRUE(psn.execute("current_set", params2, currentValue));
    EXPECT_TRUE(psn.execute("on", params1, currentValue));

    EXPECT_THROW(psn.execute("current_set", params1, currentValue), std::invalid_argument);
    params1.push_back(2.3);
    EXPECT_THROW(psn.execute("current_get", params1, currentValue), std::invalid_argument);
    params1.push_back(5.8);
    EXPECT_THROW(psn.execute("current_set", params1, currentValue), std::invalid_argument);
}

TEST(power_supply_narrow_test, error_state)
{
    auto psi = std::make_shared<PowerSupplyImpl>();
    PowerSupplyNarrow psn(psi);
    std::vector<double> params = {};
    double currentValue;

    EXPECT_EQ(psn.getState(), CircuitDevice::State::OK);
    EXPECT_TRUE(psn.execute("off", params, currentValue));
    EXPECT_FALSE(psn.execute("current_get", params, currentValue));
    EXPECT_EQ(psn.getState(), CircuitDevice::State::ERROR);
    EXPECT_THROW(psn.execute("current_set", params, currentValue), std::runtime_error);
    EXPECT_THROW(psn.execute("current_get", params, currentValue), std::runtime_error);
    EXPECT_EQ(psn.getState(), CircuitDevice::State::ERROR);
    EXPECT_TRUE(psn.execute("on", params, currentValue));
    EXPECT_TRUE(psn.execute("reset", params, currentValue));
    EXPECT_EQ(psn.getState(), CircuitDevice::State::OK);
}

TEST(power_supply_narrow_test, non_existing_command)
{
    auto psi = std::make_shared<PowerSupplyImpl>();
    PowerSupplyNarrow psn(psi);
    std::vector<double> params = {};
    double currentValue;

    EXPECT_THROW(psn.execute("non_existing_command", params, currentValue), std::invalid_argument);
}
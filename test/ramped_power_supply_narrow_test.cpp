#include <gtest/gtest.h>
#include "../src/ramped_power_supply_narrow.h"
#include "../src/ramped_power_supply_impl.h"

TEST(ramped_power_supply_narrow_test, on_off)
{
    auto rpsi = std::make_shared<RampedPowerSupplyImpl>();
    RampedPowerSupplyNarrow rpsn(rpsi);
    std::vector<double> params = {};
    double currentValue;

    EXPECT_FALSE(rpsn.execute("on", params, currentValue));
    EXPECT_TRUE(rpsn.execute("off", params, currentValue));
    EXPECT_FALSE(rpsn.execute("off", params, currentValue));
    EXPECT_TRUE(rpsn.execute("on", params, currentValue));
    params.push_back(5.5);
    EXPECT_THROW(rpsn.execute("on", params, currentValue), std::invalid_argument);
    EXPECT_THROW(rpsn.execute("off", params, currentValue), std::invalid_argument);
}

TEST(ramped_power_supply_narrow_test, reset)
{
    auto rpsi = std::make_shared<RampedPowerSupplyImpl>();
    RampedPowerSupplyNarrow rpsn(rpsi);
    std::vector<double> params = {};
    double currentValue;

    EXPECT_TRUE(rpsn.execute("off", params, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::OK);
    EXPECT_FALSE(rpsn.execute("current_get", params, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::ERROR);
    EXPECT_FALSE(rpsn.execute("reset", params, currentValue));
    EXPECT_TRUE(rpsn.execute("on", params, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::ERROR);
    EXPECT_TRUE(rpsn.execute("reset", params, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::OK);
    params.push_back(5.5);
    EXPECT_THROW(rpsn.execute("reset", params, currentValue), std::invalid_argument);
}

TEST(ramped_power_supply_narrow_test, get_set)
{
    auto rpsi = std::make_shared<RampedPowerSupplyImpl>();
    RampedPowerSupplyNarrow rpsn(rpsi);
    std::vector<double> params1 = {};
    std::vector<double> params2 = {5.5};
    double currentValue = 10000;

    EXPECT_TRUE(rpsn.execute("current_get", params1, currentValue));
    EXPECT_EQ(currentValue, 0);
    rpsn.execute("off", params1, currentValue);
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::OK);
    EXPECT_FALSE(rpsn.execute("current_get", params1, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::ERROR);
    rpsn.execute("on", params1, currentValue);
    rpsn.execute("reset", params1, currentValue);

    EXPECT_TRUE(rpsn.execute("current_set", params2, currentValue));
    EXPECT_TRUE(rpsn.execute("current_get", params1, currentValue));
    EXPECT_EQ(currentValue, 5.5);
    rpsn.execute("off", params1, currentValue);
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::OK);
    EXPECT_FALSE(rpsn.execute("current_set", params2, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::ERROR);
    rpsn.execute("on", params1, currentValue);
    rpsn.execute("reset", params1, currentValue);

    params2.pop_back();
    params2.push_back(0);
    EXPECT_TRUE(rpsn.execute("current_set", params2, currentValue));
    EXPECT_TRUE(rpsn.execute("on", params1, currentValue));

    EXPECT_THROW(rpsn.execute("current_set", params1, currentValue), std::invalid_argument);
    params1.push_back(2.3);
    EXPECT_THROW(rpsn.execute("current_get", params1, currentValue), std::invalid_argument);
    params1.push_back(5.8);
    EXPECT_THROW(rpsn.execute("current_set", params1, currentValue), std::invalid_argument);
}

TEST(ramped_power_supply_narrow_test, ramp_load_ramp_start)
{
    auto rpsi = std::make_shared<RampedPowerSupplyImpl>();
    RampedPowerSupplyNarrow rpsn(rpsi);
    std::vector<double> params1 = {2000};
    std::vector<double> params2 = {3.5, 5.7, 9.8, 11};
    std::vector<double> params3 = {};
    double currentValue = 10000;

    EXPECT_TRUE(rpsn.execute("ramp_load", params2, currentValue));
    rpsn.execute("off", params3, currentValue);
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::OK);
    EXPECT_FALSE(rpsn.execute("ramp_load", params2, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::ERROR);
    rpsn.execute("on", params3, currentValue);
    rpsn.execute("reset", params3, currentValue);

    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::OK);
    EXPECT_FALSE(rpsn.execute("ramp_start", params1, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::ERROR);
    rpsn.execute("reset", params3, currentValue);

    EXPECT_TRUE(rpsn.execute("ramp_load", params2, currentValue));
    EXPECT_TRUE(rpsn.execute("ramp_start", params1, currentValue));
    EXPECT_TRUE(rpsn.execute("current_get", params3, currentValue));
    EXPECT_EQ(currentValue, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    EXPECT_TRUE(rpsn.execute("current_get", params3, currentValue));
    EXPECT_EQ(currentValue, 3.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    EXPECT_TRUE(rpsn.execute("current_get", params3, currentValue));
    EXPECT_EQ(currentValue, 5.7);
    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    EXPECT_TRUE(rpsn.execute("current_get", params3, currentValue));
    EXPECT_EQ(currentValue, 9.8);
    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    EXPECT_TRUE(rpsn.execute("current_get", params3, currentValue));
    EXPECT_EQ(currentValue, 11);

    rpsn.execute("off", params3, currentValue);
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::OK);
    EXPECT_FALSE(rpsn.execute("ramp_start", params1, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::ERROR);
    rpsn.execute("on", params3, currentValue);
    rpsn.execute("reset", params3, currentValue);

    EXPECT_THROW(rpsn.execute("ramp_load", params3, currentValue), std::invalid_argument);
    EXPECT_THROW(rpsn.execute("ramp_start", params3, currentValue), std::invalid_argument);
}

TEST(ramped_power_supply_narrow_test, error_state)
{
    auto rpsi = std::make_shared<RampedPowerSupplyImpl>();
    RampedPowerSupplyNarrow rpsn(rpsi);
    std::vector<double> params = {};
    double currentValue;

    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::OK);
    EXPECT_TRUE(rpsn.execute("off", params, currentValue));
    EXPECT_FALSE(rpsn.execute("current_get", params, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::ERROR);
    EXPECT_THROW(rpsn.execute("current_set", params, currentValue), std::runtime_error);
    EXPECT_THROW(rpsn.execute("current_get", params, currentValue), std::runtime_error);
    EXPECT_THROW(rpsn.execute("ramp_load", params, currentValue), std::runtime_error);
    EXPECT_THROW(rpsn.execute("ramp_start", params, currentValue), std::runtime_error);
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::ERROR);
    EXPECT_TRUE(rpsn.execute("on", params, currentValue));
    EXPECT_TRUE(rpsn.execute("reset", params, currentValue));
    EXPECT_EQ(rpsn.getState(), CircuitDevice::State::OK);
}

TEST(ramped_power_supply_narrow_test, non_existing_command)
{
    auto rpsi = std::make_shared<RampedPowerSupplyImpl>();
    RampedPowerSupplyNarrow rpsn(rpsi);
    std::vector<double> params = {};
    double currentValue;

    EXPECT_THROW(rpsn.execute("non_existing_command", params, currentValue), std::invalid_argument);
}
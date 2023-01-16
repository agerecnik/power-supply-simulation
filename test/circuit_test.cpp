#include <gtest/gtest.h>
#include "../src/circuit.h"
#include "../src/power_supply_impl.h"
#include "../src/power_supply_narrow.h"
#include "../src/ramped_power_supply_impl.h"
#include "../src/ramped_power_supply_narrow.h"

TEST(circuit_test, add_device_execute_command)
{
    Circuit circuit;
    std::vector<double> params1 = {};
    std::vector<double> params2 = {3.5};
    std::vector<double> params3 = {3.5, 5.7, 9.8, 11};
    std::vector<double> params4 = {10};
    double currentValue = 10000;

    auto rpsi = std::make_shared<RampedPowerSupplyImpl>();
    std::string name1 = "rps";

    auto psi = std::make_shared<PowerSupplyImpl>();
    std::string name2 = "ps";

    circuit.add(name1, std::make_shared<RampedPowerSupplyNarrow>(rpsi));
    EXPECT_THROW(circuit.add(name1, std::make_shared<PowerSupplyNarrow>(psi)), std::invalid_argument);
    circuit.add(name2, std::make_shared<PowerSupplyNarrow>(psi));

    EXPECT_THROW(
        circuit.execute("non_existing_device", "on", params1, currentValue),
        std::invalid_argument);

    EXPECT_FALSE(circuit.execute(name1, "on", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "current_set", params2, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "current_get", params1, currentValue));
    EXPECT_EQ(currentValue, 3.5);
    EXPECT_TRUE(circuit.execute(name1, "ramp_load", params3, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "ramp_start", params4, currentValue));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(circuit.execute(name1, "current_get", params1, currentValue));
    EXPECT_EQ(currentValue, 11);
    EXPECT_TRUE(circuit.execute(name1, "off", params1, currentValue));
    EXPECT_FALSE(circuit.execute(name1, "reset", params1, currentValue));
    EXPECT_FALSE(circuit.execute(name1, "current_set", params2, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "on", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "reset", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "off", params1, currentValue));
    EXPECT_FALSE(circuit.execute(name1, "current_get", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "on", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "reset", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "off", params1, currentValue));
    EXPECT_FALSE(circuit.execute(name1, "ramp_load", params3, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "on", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "reset", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name1, "off", params1, currentValue));
    EXPECT_FALSE(circuit.execute(name1, "ramp_start", params4, currentValue));

    EXPECT_FALSE(circuit.execute(name2, "on", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name2, "current_set", params2, currentValue));
    EXPECT_TRUE(circuit.execute(name2, "current_get", params1, currentValue));
    EXPECT_EQ(currentValue, 3.5);
    EXPECT_TRUE(circuit.execute(name2, "off", params1, currentValue));
    EXPECT_FALSE(circuit.execute(name2, "reset", params1, currentValue));
    EXPECT_FALSE(circuit.execute(name2, "current_set", params2, currentValue));
    EXPECT_TRUE(circuit.execute(name2, "on", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name2, "reset", params1, currentValue));
    EXPECT_TRUE(circuit.execute(name2, "off", params1, currentValue));
    EXPECT_FALSE(circuit.execute(name2, "current_get", params1, currentValue));
}
#ifndef POWER_SUPPLY_NARROW_H
#define POWER_SUPPLY_NARROW_H

#include "circuit_device.h"
#include "power_supply.h"
#include <memory>

class PowerSupplyNarrow final : public CircuitDevice
{
public:
    PowerSupplyNarrow(std::shared_ptr<PowerSupply> p);
    ~PowerSupplyNarrow();
    bool execute(
        const std::string &command, const std::vector<double> &parameters,
        double &currentValue) override;
    State getState() override;

private:
    std::shared_ptr<PowerSupply> ps;
    State state;
};

#endif
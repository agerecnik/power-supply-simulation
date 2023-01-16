#ifndef RAMPED_POWER_SUPPLY_NARROW_H
#define RAMPED_POWER_SUPPLY_NARROW_H

#include "circuit_device.h"
#include "ramped_power_supply.h"
#include <memory>

class RampedPowerSupplyNarrow final : public CircuitDevice
{
public:
    RampedPowerSupplyNarrow(std::shared_ptr<RampedPowerSupply> r);
    ~RampedPowerSupplyNarrow();
    bool execute(
        const std::string &command, const std::vector<double> &parameters,
        double &currentValue) override;
    State getState() override;

private:
    std::shared_ptr<RampedPowerSupply> rps;
    State state;
};

#endif
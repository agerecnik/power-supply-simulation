#ifndef RAMPED_POWER_SUPPLY_IMPL_H
#define RAMPED_POWER_SUPPLY_IMPL_H

#include "ramped_power_supply.h"
#include <thread>
#include <mutex>

class RampedPowerSupplyImpl final : public RampedPowerSupply
{
public:
    RampedPowerSupplyImpl();
    ~RampedPowerSupplyImpl();
    bool on() override;
    bool off() override;
    bool isOn() override;
    bool reset() override;
    bool get(double &c) override;
    bool set(const double value) override;
    bool loadRamp(const std::vector<double> &values) override;
    bool startRamp(const std::uint32_t msec) override;

private:
    std::mutex mutex;
    std::thread rampThread;
};

#endif
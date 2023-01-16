#ifndef RAMPED_POWER_SUPPLY_H
#define RAMPED_POWER_SUPPLY_H

#include "power_supply.h"
#include <vector>
#include <cinttypes>

class RampedPowerSupply : public PowerSupply
{
public:
    RampedPowerSupply(
        const bool os, const double c, const std::vector<double> &rv, const bool ra) :
        PowerSupply(os, c),
        rampValues(rv),
        rampActive(ra){};
    virtual ~RampedPowerSupply() {}
    virtual bool loadRamp(const std::vector<double> &values) = 0;
    virtual bool startRamp(const std::uint32_t msec) = 0;

protected:
    std::vector<double> rampValues;
    bool rampActive;
};

#endif
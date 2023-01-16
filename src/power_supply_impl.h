#ifndef POWER_SUPPLY_IMPL_H
#define POWER_SUPPLY_IMPL_H

#include "power_supply.h"

class PowerSupplyImpl final : public PowerSupply {
    public:
        PowerSupplyImpl();
        bool on() override;
        bool off() override;
        bool isOn() override;
        bool reset() override;
        bool get(double &c) override;
        bool set(const double value) override;
};

#endif
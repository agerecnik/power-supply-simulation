#include "power_supply_impl.h"

PowerSupplyImpl::PowerSupplyImpl() : PowerSupply(false, 0) {}

bool PowerSupplyImpl::on()
{
    if (!onState) {
        onState = true;
        return true;
    }
    return false;
}

bool PowerSupplyImpl::off()
{
    if (onState) {
        current = 0;
        onState = false;
        return true;
    }
    return false;
}

bool PowerSupplyImpl::isOn()
{
    return onState;
}

bool PowerSupplyImpl::reset()
{
    if (onState) {
        current = 0;
        return true;
    }
    return false;
}

bool PowerSupplyImpl::get(double &c)
{
    if (onState) {
        c = current;
        return true;
    }
    return false;
}

bool PowerSupplyImpl::set(const double value)
{
    if (onState && value >= 0) {
        current = value;
        return true;
    }
    return false;
}
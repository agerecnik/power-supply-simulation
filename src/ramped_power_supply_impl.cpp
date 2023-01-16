#include "ramped_power_supply_impl.h"
#include <iostream>

RampedPowerSupplyImpl::RampedPowerSupplyImpl() : RampedPowerSupply(false, 0, {}, false) {}

RampedPowerSupplyImpl::~RampedPowerSupplyImpl()
{
    if (rampThread.joinable()) {
        rampThread.join();
    }
}

bool RampedPowerSupplyImpl::on()
{
    if (!onState) {
        onState = true;
        return true;
    }
    return false;
}

bool RampedPowerSupplyImpl::off()
{
    std::lock_guard<std::mutex> guard(mutex);
    if (onState && !rampActive) {
        current = 0;
        onState = false;
        return true;
    }
    return false;
}

bool RampedPowerSupplyImpl::isOn()
{
    return onState;
}

bool RampedPowerSupplyImpl::reset()
{
    std::lock_guard<std::mutex> guard(mutex);
    if (onState && !rampActive) {
        current = 0;
        rampValues.clear();
        return true;
    }
    return false;
}

bool RampedPowerSupplyImpl::get(double &c)
{
    if (onState) {
        c = current;
        return true;
    }
    return false;
}

bool RampedPowerSupplyImpl::set(const double value)
{
    std::lock_guard<std::mutex> guard(mutex);
    if (onState && !rampActive && value >= 0) {
        current = value;
        return true;
    }
    return false;
}

bool RampedPowerSupplyImpl::loadRamp(const std::vector<double> &values)
{
    std::lock_guard<std::mutex> guard(mutex);
    if (onState && !rampActive) {
        rampValues = values;
        return true;
    }
    return false;
}

bool RampedPowerSupplyImpl::startRamp(const std::uint32_t msec)
{
    std::lock_guard<std::mutex> guard(mutex);
    if (onState && !rampActive && !rampValues.empty()) {
        if (rampThread.joinable()) {
            rampThread.join();
							
        }
        rampActive = true;
        rampThread = std::thread([this, msec] {
            for (auto value : rampValues) {
                std::this_thread::sleep_for(std::chrono::milliseconds(msec));
                mutex.lock();
                current = value;
                mutex.unlock();
            }
            mutex.lock();
            rampActive = false;
            mutex.unlock();
        });
        return true;
    }
    return false;
}
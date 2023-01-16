#include "ramped_power_supply_narrow.h"
#include "ramped_power_supply_impl.h"

#include <stdexcept>

RampedPowerSupplyNarrow::RampedPowerSupplyNarrow(std::shared_ptr<RampedPowerSupply> r) :
    rps{r},
    state{State::OK}
{
    rps->on();
    if (!rps->off() || !rps->on()) {
        state = State::ERROR;
    }
}

RampedPowerSupplyNarrow::~RampedPowerSupplyNarrow() = default;

bool RampedPowerSupplyNarrow::execute(
    const std::string &command, const std::vector<double> &parameters, double &currentValue)
{
    if (command == "on") {
        if (parameters.size() != 0) {
            throw std::invalid_argument("The command 'on' takes 0 arguments.");
        }
        return rps->on();
    } else if (command == "off") {
        if (parameters.size() != 0) {
            throw std::invalid_argument("The command 'off' takes 0 arguments.");
        }
        return rps->off();
    } else if (command == "reset") {
        if (parameters.size() != 0) {
            throw std::invalid_argument("The command 'reset' takes 0 arguments.");
        }
        if (rps->reset()) {
            state = State::OK;
            return true;
        } else {
            return false;
        }
    }
    if (state == State::OK) {
        if (command == "current_get") {
            if (parameters.size() != 0) {
                throw std::invalid_argument("The command 'current_get' takes 0 arguments.");
            }
            if (rps->get(currentValue)) {
                return true;
            } else {
                state = State::ERROR;
                return false;
            }

        } else if (command == "current_set") {
            if (parameters.size() != 1) {
                throw std::invalid_argument("The command 'current_set' takes 1 argument.");
            }
            if (rps->set(parameters[0])) {
                if (parameters[0] == 0) {
                    rps->off();
                }
                return true;
            } else {
                state = State::ERROR;
                return false;
            }
        } else if (command == "ramp_load") {
            if (parameters.size() < 1) {
                throw std::invalid_argument(
                    "The command 'ramp_load' takes 1 or more arguments.");
            }
            if (rps->loadRamp(parameters)) {
                return true;
            } else {
                state = State::ERROR;
                return false;
            }
        } else if (command == "ramp_start") {
            if (parameters.size() != 1) {
                throw std::invalid_argument("The command 'ramp_start' takes 1 argument.");
            }
            if (rps->startRamp(parameters[0])) {
                return true;
            } else {
                state = State::ERROR;
                return false;
            }
        }
        throw std::invalid_argument("The command " + command + " does not exist");
    }
    throw std::runtime_error("Device is in error state. Reset the device.");
}

CircuitDevice::State RampedPowerSupplyNarrow::getState()
{
    return state;
}
#include "power_supply_narrow.h"
#include "power_supply_impl.h"

#include <stdexcept>

PowerSupplyNarrow::PowerSupplyNarrow(std::shared_ptr<PowerSupply> p) : ps{p}, state{State::OK}
{
    ps->on();
    if (!ps->off() || !ps->on()) {
        state = State::ERROR;
    }
}

PowerSupplyNarrow::~PowerSupplyNarrow() = default;

bool PowerSupplyNarrow::execute(
    const std::string &command, const std::vector<double> &parameters, double &currentValue)
{
    if (command == "on") {
        if (parameters.size() != 0) {
            throw std::invalid_argument("The command 'on' takes 0 arguments.");
        }
        return ps->on();
    } else if (command == "off") {
        if (parameters.size() != 0) {
            throw std::invalid_argument("The command 'off' takes 0 arguments.");
        }
        return ps->off();
    } else if (command == "reset") {
        if (parameters.size() != 0) {
            throw std::invalid_argument("The command 'reset' takes 0 arguments.");
        }
        if (ps->reset()) {
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
            if (ps->get(currentValue)) {
                return true;
            } else {
                state = State::ERROR;
                return false;
            }

        } else if (command == "current_set") {
            if (parameters.size() != 1) {
                throw std::invalid_argument("The command 'current_set' takes 1 argument.");
            }
            if (ps->set(parameters[0])) {
                if (parameters[0] == 0) {
                    ps->off();
                }
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

CircuitDevice::State PowerSupplyNarrow::getState()
{
    return state;
}
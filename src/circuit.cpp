#include "circuit.h"

Circuit::Circuit() = default;

Circuit::~Circuit() = default;

void Circuit::add(const std::string &deviceName, std::shared_ptr<CircuitDevice> device)
{
    auto ret = devices.insert({deviceName, device});
    if (ret.second == false) {
        throw std::invalid_argument("Device name already exists.");
    }
}

bool Circuit::execute(
    const std::string &deviceName, const std::string &command,
    const std::vector<double> &parameters, double &currentValue)
{
    auto ret = devices.find(deviceName);
    if (ret == devices.end()) {
        throw std::invalid_argument("Device does not exist.");
    }

    return ret->second->execute(command, parameters, currentValue);
}
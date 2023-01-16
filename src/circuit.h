#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "circuit_device.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

class Circuit
{
public:
    Circuit();
    ~Circuit();
    void add(const std::string &deviceName, std::shared_ptr<CircuitDevice> device);
    bool execute(
        const std::string &deviceName, const std::string &command,
        const std::vector<double> &parameters, double &currentValue);

private:
    std::map<std::string, std::shared_ptr<CircuitDevice>> devices;
};

#endif
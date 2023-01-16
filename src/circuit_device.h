#ifndef CIRCUIT_DEVICE_H
#define CIRCUIT_DEVICE_H

#include <vector>
#include <string>

class CircuitDevice
{
public:
    enum State { OK, ERROR };
    virtual ~CircuitDevice() {}
    virtual bool execute(
        const std::string &command, const std::vector<double> &parameters,
        double &currentValue) = 0;
    virtual State getState() = 0;
};

#endif
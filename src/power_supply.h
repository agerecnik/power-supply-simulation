#ifndef POWER_SUPPLY_H
#define POWER_SUPPLY_H

class PowerSupply
{
public:
    PowerSupply(const bool os, const double c) : onState(os), current(c){};
    virtual ~PowerSupply() {}
    virtual bool on() = 0;
    virtual bool off() = 0;
    virtual bool isOn() = 0;
    virtual bool reset() = 0;
    virtual bool get(double &c) = 0;
    virtual bool set(const double value) = 0;

protected:
    bool onState;
    double current;
};

#endif
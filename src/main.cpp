#include "circuit.h"
#include "power_supply_impl.h"
#include "power_supply_narrow.h"
#include "ramped_power_supply_impl.h"
#include "ramped_power_supply_narrow.h"
#include <iostream>
#include <sstream>
#include <vector>

int main()
{
    Circuit circuit;

    std::string name1 = "ps1";
    auto psi1 = std::make_shared<PowerSupplyImpl>();
    auto psn1 = std::make_shared<PowerSupplyNarrow>(psi1);
    circuit.add(name1, psn1);

    std::string name2 = "ps2";
    auto psi2 = std::make_shared<PowerSupplyImpl>();
    auto psn2 = std::make_shared<PowerSupplyNarrow>(psi2);
    circuit.add(name2, psn2);

    std::string name3 = "rps1";
    auto rpsi1 = std::make_shared<RampedPowerSupplyImpl>();
    auto rpsn1 = std::make_shared<RampedPowerSupplyNarrow>(rpsi1);
    circuit.add(name3, rpsn1);

    std::cout << "Enter command in the following format: deviceName command "
                 "parametersSeparatedWithSpaces"
              << std::endl;
    std::cout << "To exit enter: x" << std::endl;
    std::string input;
    while (getline(std::cin, input)) {
        std::istringstream iss(input);

        std::string deviceName;
        iss >> deviceName;
        if (deviceName == "x") {
            break;
        }

        std::string command;
        iss >> command;

        std::vector<double> parameters;
        std::string parameter;
        try {
            while (iss >> parameter) {
                parameters.push_back(stod(parameter));
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid parameter format." << '\n';
            continue;
        }

        double currentValue;

        try {
            if (circuit.execute(deviceName, command, parameters, currentValue)) {
                std::cout << "Device has executed the command " << command << "." << std::endl;
                if (command == "current_get") {
                    std::cout << "The current is: " << currentValue << std::endl;
                }
            } else {
                std::cout << "Device has NOT executed the command " << command << "."
                          << std::endl;
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << '\n';
            continue;
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << '\n';
            continue;
        }
    }

    return 0;
}
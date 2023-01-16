cmake_minimum_required(VERSION 3.14)
project(power_supply_simulation)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(power_supply_simulation_test test/power_supply_impl_test.cpp test/ramped_power_supply_impl_test.cpp src/power_supply.h src/power_supply_impl.h src/power_supply_impl.cpp src/ramped_power_supply.h src/ramped_power_supply_impl.h src/ramped_power_supply_impl.cpp test/power_supply_narrow_test.cpp test/ramped_power_supply_narrow_test.cpp test/circuit_test.cpp src/power_supply_narrow.h src/power_supply_narrow.cpp src/circuit_device.h src/ramped_power_supply_narrow.cpp src/ramped_power_supply_narrow.h src/circuit.cpp src/circuit.h)
add_executable(main src/main.cpp src/power_supply.h src/power_supply_impl.h src/power_supply_impl.cpp src/ramped_power_supply.h src/ramped_power_supply_impl.h src/ramped_power_supply_impl.cpp src/power_supply_narrow.h src/power_supply_narrow.cpp src/circuit_device.h src/ramped_power_supply_narrow.cpp src/ramped_power_supply_narrow.h src/circuit.cpp src/circuit.h)
target_link_libraries(power_supply_simulation_test gtest_main)
include(GoogleTest)
gtest_discover_tests(power_supply_simulation_test)
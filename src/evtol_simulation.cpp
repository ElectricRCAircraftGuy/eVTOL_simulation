/*
Main simulation file
*/

// C++ includes
#include <iostream>
#include <vector>

// C includes
//////
// #include <cstdint>  // For `uint8_t`, `int8_t`, etc.
// #include <cstdio>   // For `printf()`

class Company
{
public:

}

class Vehicle
{
public:
    constexpr char* company_name;
    constexpr double cruise_speed_mph;
    constexpr double battery_capacity_kwh;
    constexpr double time_to_charge_hrs;
    constexpr double energy_in_cruise_kwh_per_mile;
    constexpr uint16_t max_passenger_cnt;
    /// mean probability of faults per hour
    constexpr double prob_fault_per_hr;

    // constructor
    Vehicle(double )

    {}
}

class Simulation
{
public:
    void add_aircraft(Vehicle vehicle)
    {
        _aircraft.push_back(vehicle);
    }

private:
    std::vector<Vehicle> _aircraft;
}

int main()
{
    std::cout << "Running simulation\n\n";

    Simulation simulation;

    // Populate all vehicle
    simulation.add_aircraft({"Alpha", 120, 320, 0.6, 1.6, 4, 0.25})

    return 0;
}

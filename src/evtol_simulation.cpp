/*
Main simulation file
*/

// C++ includes
#include <iostream>
#include <string>
#include <vector>

// C includes
//////
// #include <cstdint>  // For `uint8_t`, `int8_t`, etc.
// #include <cstdio>   // For `printf()`

constexpr uint32_t NUM_VEHICLES = 20;

class Vehicle
{
public:
    // Vehicle constants
    // TODO: move to its own struct
    const double cruise_speed_mph;
    const double battery_capacity_kwh;
    const double time_to_charge_hrs;
    const double energy_in_cruise_kwh_per_mile;
    const uint16_t max_passenger_cnt;
    /// mean probability of faults per hour
    const double prob_fault_per_hr;

    // // Vehicle stats
    // // TODO: Move to its own struct
    // avg_flight_time_per_flight_hrs;
    // avg_dist_per_flight_miles;
    // avg_

    // constructor
    Vehicle(double cruise_speed_mph_,
            double battery_capacity_kwh_,
            double time_to_charge_hrs_,
            double energy_in_cruise_kwh_per_mile_,
            uint16_t max_passenger_cnt_,
            double prob_fault_per_hr_
        )
        : cruise_speed_mph{cruise_speed_mph_}
        , battery_capacity_kwh{battery_capacity_kwh_}
        , time_to_charge_hrs{time_to_charge_hrs_}
        , energy_in_cruise_kwh_per_mile{energy_in_cruise_kwh_per_mile_}
        , max_passenger_cnt{max_passenger_cnt_}
        , prob_fault_per_hr{prob_fault_per_hr_}
    {}
};

// class Company
// {
// public:
//     // constructor
//     Company(const char* name)
//         : _name{name}
//         {
//         }

//     add_vehicle(Vehicle vehicle)
//     {
//         _vehicles.push_back(vehicle);
//     }

// private:
//     std::string _name;
//     std::vector<Vehicle> _vehicles;
// };

// class Charger
// {

// }

class Simulation
{
public:
    void add_random_vehicle()
    {
        // _vehicles.push_back(vehicle);
    }

    void add_vehicles(uint32_t num_vehicles)
    {
        for (uint32_t i = 0; i < num_vehicles; i++)
        {
            add_random_vehicle();
        }
    }

    // void add_charger()
    // {
    //     _chargers.push_back()
    // }

private:
    std::vector<Vehicle> _vehicles;
};

int main()
{
    std::cout << "Running simulation\n\n";

    Simulation simulation;

    // Populate all vehicles
    simulation.add_vehicles(NUM_VEHICLES);

    // simulation.add_vehicle({"Alpha", 120, 320, 0.6, 1.6, 4, 0.25});

    return 0;
}

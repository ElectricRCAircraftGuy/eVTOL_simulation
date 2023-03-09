/*
Main simulation file
*/

// C++ includes
#include <iostream>
#include <random>
#include <string>
#include <vector>

// C includes
//////
// #include <cstdint>  // For `uint8_t`, `int8_t`, etc.
// #include <cstdio>   // For `printf()`

constexpr uint32_t NUM_VEHICLES = 20;
constexpr uint32_t NUM_CHARGERS = 3;

struct VehicleType
{
    /// vehicle name (same as "company name" in this case, since each company only has one vehicle
    /// type)
    const std::string name;
    const double cruise_speed_mph;
    const double battery_capacity_kwh;
    const double time_to_charge_hrs;
    const double energy_in_cruise_kwh_per_mile;
    const uint16_t max_passenger_cnt;
    /// mean probability of faults per hour
    const double prob_fault_per_hr;

    // constructor
    Vehicle(
            std::string name_,
            double cruise_speed_mph_,
            double battery_capacity_kwh_,
            double time_to_charge_hrs_,
            double energy_used_kwh_per_mile_,
            uint16_t passenger_cnt_,
            double prob_fault_per_hr_
        )
        : name{name_}
        , cruise_speed_mph{cruise_speed_mph_}
        , battery_capacity_kwh{battery_capacity_kwh_}
        , time_to_charge_hrs{time_to_charge_hrs_}
        , energy_in_cruise_kwh_per_mile{energy_used_kwh_per_mile_}
        , max_passenger_cnt{passenger_cnt_}
        , prob_fault_per_hr{prob_fault_per_hr_}
    {}
}

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
            double energy_used_kwh_per_mile_,
            uint16_t passenger_cnt_,
            double prob_fault_per_hr_
        )
        : cruise_speed_mph{cruise_speed_mph_}
        , battery_capacity_kwh{battery_capacity_kwh_}
        , time_to_charge_hrs{time_to_charge_hrs_}
        , energy_in_cruise_kwh_per_mile{energy_used_kwh_per_mile_}
        , max_passenger_cnt{passenger_cnt_}
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

    // constructor
    Simulation()
    {
        // _initialize_uniform_int_rng();
    }

    // TODO: add error handling to check if a certain type (name) was already added
    void add_vehicle_type(
        const char* name,
        double cruise_speed_mph_,
        double battery_capacity_kwh_,
        double time_to_charge_hrs_,
        double energy_used_kwh_per_mile_,
        uint16_t passenger_cnt_,
        double prob_fault_per_hr_)
    {
        /////////////

    }

    void add_random_vehicle()
    {
        // get a random number from index 0 to "number of vehicle types" - 1, and then add a vehicle
        // of this type
        std::uniform_int_distribution<uint32_t> distribution(0, _vehicle_types.size() - 1);
        uint32_t i = distribution(_generator);
        Vehicle random_vehicle_type{_vehicle_types[i]};

        _vehicles.push_back(random_vehicle_type);
    }

    void populate_vehicles(uint32_t num_vehicles)
    {
        for (uint32_t i = 0; i < num_vehicles; i++)
        {
            add_random_vehicle();
        }
    }

    void set_num_chargers(uint32_t num_chargers)
    {
        _num_chargers = num_chargers;
    }


    // void add_charger()
    // {
    //     _chargers.push_back()
    // }

private:
    std::vector<VehicleType> _vehicle_types;
    std::vector<Vehicle> _vehicles;
    uint32_t _num_chargers = 0;

    static const std::array<std::string>

    /// Standard mersenne_twister_engine used for random number generation
    std::mt19937 _generator{std::random_device};

    // // Initialize the uniform integer random number generator
    // void _initialize_uniform_int_rng()
    // {
    //     std::random_device dev;
    //     _generator = std::mt19937(dev);
    // }
};

int main()
{
    std::cout << "Running simulation\n\n";

    Simulation simulation;

    // Add the various company vehicle types and stats
    // TODO: store and read these from a .yaml file.
    // clang-format off
    simulation.add_vehicle_type("Alpha",    120, 320, 0.6,  1.6, 4, 0.25);
    simulation.add_vehicle_type("Bravo",    100, 100, 0.2,  1.5, 5, 0.10);
    simulation.add_vehicle_type("Charlie",  160, 220, 0.8,  2.2, 3, 0.05);
    simulation.add_vehicle_type("Delta",    90,  120, 0.62, 0.8, 2, 0.22);
    simulation.add_vehicle_type("Echo",     30,  150, 0.3,  5.8, 2, 0.61);
    // clang-format on

    simulation.set_num_chargers(NUM_CHARGERS);

    // Randomly populate the correct number of vehicles
    simulation.populate_vehicles(NUM_VEHICLES);

    return 0;
}

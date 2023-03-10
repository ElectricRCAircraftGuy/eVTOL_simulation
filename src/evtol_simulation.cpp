/*
Main simulation file

Todo: break apart into multiple modules, each with a header and source file.
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

// TODO: read these as input arguments to `main()`.
constexpr uint32_t NUM_VEHICLES = 20;
constexpr uint32_t NUM_CHARGERS = 3;
constexpr double SIMULATION_TIME_HRS = 3.0;

struct Vehicle_type
{
    /// vehicle name (same as "company name" in this case, since each company only has one vehicle
    /// type)
    const std::string name;
    const double cruise_speed_mph;
    const double battery_capacity_kwh;
    const double time_to_charge_hrs;
    const double energy_used_kwh_per_mile;
    const uint16_t passenger_cnt;
    /// mean probability of faults per hour
    const double prob_fault_per_hr;

    // constructor
    Vehicle_type(
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
        , energy_used_kwh_per_mile{energy_used_kwh_per_mile_}
        , passenger_cnt{passenger_cnt_}
        , prob_fault_per_hr{prob_fault_per_hr_}
    {}

    void print() const
    {
        printf("%-10s %8.2f %8.2f %6.2f %6.2f %4u %6.2f\n",
            name.c_str(),
            cruise_speed_mph,
            battery_capacity_kwh,
            time_to_charge_hrs,
            energy_used_kwh_per_mile,
            passenger_cnt,
            prob_fault_per_hr
        );
    }
};

struct Vehicle_stats
{
    // // Vehicle stats
    // // TODO: Move to its own struct
    // avg_flight_time_per_flight_hrs;
    // avg_dist_per_flight_miles;
    // avg_

};

class Vehicle
{
public:
    // constructor
    Vehicle(Vehicle_type vehicle_type)
        : _vehicle_type{vehicle_type}
    {}

private:
    Vehicle_type _vehicle_type;
    Vehicle_stats _vehicle_stats;

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
    }

    // TODO: add error handling to check if a certain type (name) was already added. Do this in
    // O(1) time by changing the `_vehicle_types` type to an unordered_map instead of a vector.
    // Update all other parts of the code as necessary for this type change.
    void add_vehicle_type(Vehicle_type vehicle_type)
    {
        _vehicle_types.push_back(vehicle_type);
    }

    void add_random_vehicle() //////// refactor: make private; move the rng stuff to populate_vehicles
    {
        // get a random number from index 0 to "number of vehicle types" - 1, and then add a vehicle
        // of this type
        std::uniform_int_distribution<uint32_t> distribution(0, _vehicle_types.size() - 1);
        uint32_t i = distribution(_generator);
        Vehicle random_vehicle{_vehicle_types[i]};

        _vehicles.push_back(random_vehicle);
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

    void print_vehicle_types()
    {
        std::cout << "Vehicle types:" << "\n";
        for (const Vehicle_type& vehicle_type : _vehicle_types)
        {
            vehicle_type.print();
        }
    }

    void print_vehicles()
    {
        /////////
    }


    // void add_charger()
    // {
    //     _chargers.push_back()
    // }

private:
    std::vector<Vehicle_type> _vehicle_types;
    std::vector<Vehicle> _vehicles;
    uint32_t _num_chargers = 0;

    // For random number generation

    std::random_device _random_device;
    /// Standard mersenne_twister_engine
    std::mt19937 _generator{_random_device()};
};

int main()
{
    std::cout << "Running simulation\n\n";

    Simulation simulation;

    // Add the various company vehicle types and stats
    // TODO: store and read these from a .yaml file.
    // clang-format off
    simulation.add_vehicle_type({"Alpha",    120, 320, 0.6,  1.6, 4, 0.25});
    simulation.add_vehicle_type({"Bravo",    100, 100, 0.2,  1.5, 5, 0.10});
    simulation.add_vehicle_type({"Charlie",  160, 220, 0.8,  2.2, 3, 0.05});
    simulation.add_vehicle_type({"Delta",    90,  120, 0.62, 0.8, 2, 0.22});
    simulation.add_vehicle_type({"Echo",     30,  150, 0.3,  5.8, 2, 0.61});
    // clang-format on

    simulation.print_vehicle_types();

    simulation.set_num_chargers(NUM_CHARGERS);
    // simulation.set_simulation_time(SIMULATION_TIME_HRS);//////////

    // Randomly populate the correct number of vehicles
    simulation.populate_vehicles(NUM_VEHICLES);
    simulation.print_vehicles(); /////// index and name only

    //////////
    // simulation.run();
    // simulation.print_results();

    return 0;
}

//////state machine: fly, wait for charger, charge, repeat

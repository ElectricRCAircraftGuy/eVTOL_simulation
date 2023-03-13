/*
Simulation class module
*/

#pragma once

// local includes
#include "utils.h"
#include "vehicle.h"

// 3rd-party library includes
#include <gtest/gtest_prod.h>  // for `FRIEND_TEST()` macro

// Linux includes
// NA

// C++ includes
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

/// The main class required to create vehicles and run the whole simulation.
/// \note  Running many simulations at once could easily be parallelized as part of a larger
///        Monte Carlo method simulation by creating and running one `Simulation` class per
///        hardware thread.
class Simulation
{
public:
    // constructor
    Simulation(
        uint32_t num_chargers,
        double simulation_duration_hrs,
        double simulation_step_size_hrs);

    // returns true if successful and false otherwise
    // TODO: improve error handling; ex: return enums instead. Ex: see `enum class Error_code`
    // and `struct Error` in my code here:
    // https://github.com/ElectricRCAircraftGuy/eRCaGuy_hello_world/blob/master/cpp/curl_lib_WIP.h#L49-L87
    bool add_vehicle_type(Vehicle_type vehicle_type);

    void populate_vehicles(uint32_t num_vehicles);

    void print_vehicle_types();

    void print_vehicles();

    /// Run the whole simulation for all vehicles
    void run();

    /// Print required simulation results
    void print_results();

private:
    std::vector<Vehicle_type> _vehicle_types;
    std::vector<Vehicle> _vehicles;
    /// Used to keep track of whether or not a particular vehicle type has already been added
    std::unordered_set<std::string> _vehicle_type_names;

    const uint32_t _num_chargers;
    const double _simulation_duration_hrs;
    const double _simulation_step_size_hrs;

    uint32_t _num_chargers_available;

    // For random number generation

    std::random_device _random_device;
    /// Standard mersenne_twister_engine
    std::mt19937 _generator{_random_device()};

    /// Random number generator of `double` numbers from 0.0 to 1.0.
    std::uniform_real_distribution<double> _dist_0_to_1{0.0, 1.0};

    /// Check for a simulated fault this time step (while flying only)
    void check_for_fault(Vehicle* vehicle);

    /// Start charging now if a charger is free; otherwise, get in line to charge
    void try_to_charge(Vehicle* vehicle);

    /// Iterate one time step forward in the simulation for one vehicle
    void iterate(Vehicle* vehicle);

    // for unit testing private members of this class

    friend class SimulationTestFixture;
    FRIEND_TEST(SimulationTestFixture, EndToEndTest);
    FRIEND_TEST(Simulation, TrivialEndToEnd);
};

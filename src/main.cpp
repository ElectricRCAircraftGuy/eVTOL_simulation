/*
Main simulation file
*/

// local includes
#include "simulation.h"
#include "vehicle.h"

// Linux includes
// NA

// C++ includes
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>


#define DEBUG // comment this out to disable debug prints

/// Debug printf function.
/// See https://stackoverflow.com/a/67667132/4561887
#ifdef DEBUG
    #define DEBUG_PRINTF(...) printf("DEBUG: " __VA_ARGS__)
#else
    #define DEBUG_PRINTF(...) \
        do                    \
        {                     \
        } while (0)
#endif

#define SECONDS_PER_HR 3600UL

// TODO: read these as input arguments to `main()`, to rapidly run a variety of simulations.
constexpr uint32_t NUM_VEHICLES = 20;
constexpr uint32_t NUM_CHARGERS = 3;
constexpr double SIMULATION_DURATION_HRS = 3.0;
constexpr double SIMULATION_STEP_SIZE_HRS = 1.0/(double)SECONDS_PER_HR; /// 1 second time step size

int main()
{
    std::cout << "Running simulation\n\n";

    Simulation simulation{NUM_CHARGERS, SIMULATION_DURATION_HRS, SIMULATION_STEP_SIZE_HRS};

    // Add the various company vehicle types and stats
    // TODO: store and read these from a .yaml file. Specify the yaml file path as an argument to
    // `main()`. Then read them in from the yaml file instead.
    // clang-format off
    simulation.add_vehicle_type({"Alpha",    120, 320, 0.6,  1.6, 4, 0.25});
    simulation.add_vehicle_type({"Bravo",    100, 100, 0.2,  1.5, 5, 0.10});
    simulation.add_vehicle_type({"Charlie",  160, 220, 0.8,  2.2, 3, 0.05});
    simulation.add_vehicle_type({"Delta",    90,  120, 0.62, 0.8, 2, 0.22});
    simulation.add_vehicle_type({"Echo",     30,  150, 0.3,  5.8, 2, 0.61});
    // clang-format on

    simulation.print_vehicle_types();

    // Randomly populate the correct total number of vehicles
    simulation.populate_vehicles(NUM_VEHICLES);
    simulation.print_vehicles();

    simulation.run();
    simulation.print_results();

    return 0;
}

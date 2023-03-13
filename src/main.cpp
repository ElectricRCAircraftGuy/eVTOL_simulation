/*
Main simulation file
*/

// local includes
#include "simulation.h"
#include "simulation_params.h"

// Linux includes
// NA

// C++ includes
#include <iostream>

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

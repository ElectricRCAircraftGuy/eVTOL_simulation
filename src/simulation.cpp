#include "simulation.h"

Simulation::Simulation(
    uint32_t num_chargers,
    double simulation_duration_hrs,
    double simulation_step_size_hrs)
    : _num_chargers{num_chargers}
    , _simulation_duration_hrs{simulation_duration_hrs}
    , _simulation_step_size_hrs{simulation_step_size_hrs}
    , _num_chargers_available{_num_chargers}
{
}


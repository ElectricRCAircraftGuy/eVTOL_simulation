/*
Simulation Parameters
*/

#pragma once

// Local includes
#include "utils.h"

// 3rd-party library includes
// NA

// Linux includes
// NA

// C and C++ includes
// NA

// TODO: read these as input arguments to `main()`, to rapidly run a variety of simulations.
constexpr uint32_t NUM_VEHICLES = 20;
constexpr uint32_t NUM_CHARGERS = 3;
constexpr double SIMULATION_DURATION_HRS = 3.0;
constexpr double SIMULATION_STEP_SIZE_HRS =
    1.0 / (double)SECONDS_PER_HR;  /// 1 second time step size

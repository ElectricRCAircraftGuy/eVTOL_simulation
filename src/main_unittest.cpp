/*
Gtest unit tests.

To install gtest, see my instructions here: https://stackoverflow.com/a/75718815/4561887

To learn to use gtest, here are some helpful docs to get started:
1. https://github.com/google/googletest/tree/main/docs - see all of the docs herein.
    1. Note: I wrote this one:
       https://github.com/google/googletest/blob/main/docs/community_created_documentation.md
1. https://github.com/google/googletest/blob/main/docs/advanced.md#testing-private-code
1. List of gtest `EXPECT_EQ()` and `ASSERT_EQ()` type test macros:
   *****+ https://github.com/google/googletest/blob/main/docs/reference/assertions.md#expect_eq-expect_eq

*/

// Local includes
#include "simulation.h"
#include "simulation_params.h"

// 3rd-party library includes
#include "gtest/gtest.h"

// Linux includes
// NA

// C and C++ includes
// NA


/// Test fixture class; it is a friend to `class Simulation`, so it can access its private members
/// to test them!
///
/// `TEST_F()` test fixture tests inherit from this class and are therefore subclasses of this
/// class, getting access to its public and protected members.
class SimulationTestFixture : public ::testing::Test
{
protected:
    // provide getters to expose private members of class `Simulation`, for testing

    std::vector<Vehicle_type>* get_vehicle_types(Simulation* simulation)
    {
        return &simulation->_vehicle_types;
    }

    std::vector<Vehicle> _vehicles;
};

// anonymous namespace
namespace
{

/// Do general end-to-end testing checks, as able, to ensure the whole simulation produces expected
/// results
TEST_F(SimulationTestFixture, EndToEndTest)
{
    Simulation simulation{NUM_CHARGERS, SIMULATION_DURATION_HRS, SIMULATION_STEP_SIZE_HRS};



    // Add the various company vehicle types and stats
    // clang-format off
    simulation.add_vehicle_type({"Alpha",    120, 320, 0.6,  1.6, 4, 0.25});
    simulation.add_vehicle_type({"Bravo",    100, 100, 0.2,  1.5, 5, 0.10});
    simulation.add_vehicle_type({"Charlie",  160, 220, 0.8,  2.2, 3, 0.05});
    simulation.add_vehicle_type({"Delta",    90,  120, 0.62, 0.8, 2, 0.22});
    simulation.add_vehicle_type({"Echo",     30,  150, 0.3,  5.8, 2, 0.61});
    // clang-format on

    simulation.print_vehicle_types();

    std::vector<Vehicle_type>* vehicle_types = get_vehicle_types(&simulation);

    // Ensure they were added properly

    EXPECT_EQ(vehicle_types->size(), 5) << "This many vehicles should be in the vector.";

    // primary values
    EXPECT_EQ(vehicle_types->name, "Alpha");
    EXPECT_FLOAT_EQ(vehicle_types->cruise_speed_mph, 120);
    EXPECT_FLOAT_EQ(vehicle_types->battery_capacity_kwh, 320);
    EXPECT_FLOAT_EQ(vehicle_types->time_to_charge_hrs, 0.6);
    EXPECT_FLOAT_EQ(vehicle_types->energy_used_kwh_per_mile, 1.6);
    EXPECT_EQ(vehicle_types->passengers_per_vehicle, 4);
    EXPECT_FLOAT_EQ(vehicle_types->prob_fault_per_hr, 0.25);
    // derived values
    EXPECT_FLOAT_EQ(vehicle_types->max_range_miles, 320.0/1.6); // kwh / khw/mile = miles
    EXPECT_FLOAT_EQ(vehicle_types->max_flight_time_hrs, 320.0/1.6/120); // miles / miles/hr = hr
    EXPECT_FLOAT_EQ(vehicle_types->cruise_power_kw, 320.0/(320.0/1.6/120));




    // Check the first one
    // get_vehicle_types

    // Randomly populate the correct total number of vehicles
    simulation.populate_vehicles(NUM_VEHICLES);
    simulation.print_vehicles();

    simulation.run();
    simulation.print_results();
}

////// Unit test: ensure flight time + wait time + charge time for all 3 vehicles is ~ 3 hrs (+/- 0.01 hrs)

} // anonymous namespace


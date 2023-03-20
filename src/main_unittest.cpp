/*
Gtest unit tests.

To install gtest, see my instructions here: https://stackoverflow.com/a/75718815/4561887

To learn to use gtest, here are some helpful docs to get started:
1. https://github.com/google/googletest/tree/main/docs - see all of the docs herein.
    1. Note: I wrote this one:
       https://github.com/google/googletest/blob/main/docs/community_created_documentation.md
1. https://github.com/google/googletest/blob/main/docs/advanced.md#testing-private-code
1. List of gtest `EXPECT_EQ()` and `ASSERT_EQ()` type test macros:
   *****+
https://github.com/google/googletest/blob/main/docs/reference/assertions.md#expect_eq-expect_eq

*/

// Local includes
#include "simulation.h"
#include "simulation_params.h"

// 3rd-party library includes
#include "gmock/gmock.h"
#include "gtest/gtest.h"

// Linux includes
// NA

// C++ includes
// NA


/// Expect or assert that value `val` is within the range of `min` to `max`,
/// inclusive. ie: `val` is tested to be >= `min` and <= `max`.
/// See:
/// 1. googletest `matchers.md` document under the "Composite Matchers" section,
///    here:
///    https://github.com/google/googletest/blob/main/docs/reference/matchers.md#composite-matchers
/// 1. [My answer with this code] https://stackoverflow.com/a/75786774/4561887
#define EXPECT_RANGE(val, min, max) EXPECT_THAT((val), \
    ::testing::AllOf(::testing::Ge((min)), ::testing::Le((max))))
#define ASSERT_RANGE(val, min, max) ASSERT_THAT((val), \
    ::testing::AllOf(::testing::Ge((min)), ::testing::Le((max))))

/// Test fixture class; it is a friend to `class Simulation`, so it can access its private members
/// to test them!
///
/// `TEST_F()` test fixture tests inherit from this class and are therefore subclasses of this
/// class, getting access to its public and protected members.
class SimulationTestFixture : public ::testing::Test
{
protected:
    // Runs once at the beginning of each test utilizing this test fixture.
    void SetUp() override
    {
        // TODO
    }

    // Runs once at the end of each test utilizing this test fixture.
    void TearDown() override
    {
        // TODO
    }

    // provide getters to expose private members of class `Simulation`, for testing

    std::vector<Vehicle_type>* get_vehicle_types(Simulation* simulation)
    {
        return &simulation->_vehicle_types;
    }

    // TODO: expose other private members of the `Simulation` class here, so that many tests can
    // utilize this test fixture rather than having to make every `TEST()` class a friend
    // to the `Simulation` class.
};

/// Do general end-to-end testing checks, as able, to ensure the whole simulation produces expected
/// results
///
/// See TODO just above. For now I'll just make this test a friend to the `Simulation` class so I
/// don't have to write a ton of getters to private members in the test fixture. It makes sense
/// to write the getters in the text fixture instead, however, if it is used tons of times.
TEST_F(SimulationTestFixture, EndToEndTest)
{
    constexpr uint32_t num_chargers = 3;
    constexpr double simulation_duration_hrs = 3.0;
    // 1 second time step size
    constexpr double simulation_step_size_hrs = 1.0 / (double)SECONDS_PER_HR;

    Simulation simulation{num_chargers, simulation_duration_hrs, simulation_step_size_hrs};

    EXPECT_EQ(simulation._num_chargers, 3);
    EXPECT_EQ(simulation._simulation_duration_hrs, 3.0);
    EXPECT_EQ(simulation._simulation_step_size_hrs, 1.0 / (double)SECONDS_PER_HR);

    // Add the various company vehicle types and stats
    // clang-format off
    simulation.add_vehicle_type({"Alpha",    120, 320, 0.6,  1.6, 4, 0.25});
    simulation.add_vehicle_type({"Bravo",    100, 100, 0.2,  1.5, 5, 0.10});
    simulation.add_vehicle_type({"Charlie",  160, 220, 0.8,  2.2, 3, 0.05});
    simulation.add_vehicle_type({"Delta",    90,  120, 0.62, 0.8, 2, 0.22});
    simulation.add_vehicle_type({"Echo",     30,  150, 0.3,  5.8, 2, 0.61});
    // clang-format on

    // simulation.print_vehicle_types(); // debugging

    std::vector<Vehicle_type>* vehicle_types = get_vehicle_types(&simulation);

    // Check the whole vector

    EXPECT_EQ(vehicle_types->size(), 5) << "This many vehicles should be in the vector.";
    EXPECT_EQ((*vehicle_types)[0].name, "Alpha");
    EXPECT_EQ((*vehicle_types)[1].name, "Bravo");
    EXPECT_EQ((*vehicle_types)[2].name, "Charlie");
    EXPECT_EQ((*vehicle_types)[3].name, "Delta");
    EXPECT_EQ((*vehicle_types)[4].name, "Echo");

    // Check individual values of just one vehicle type in the vector

    Vehicle_type* vehicle_alpha = &((*vehicle_types)[0]);
    // primary values
    EXPECT_EQ(vehicle_alpha->name, "Alpha");
    EXPECT_FLOAT_EQ(vehicle_alpha->cruise_speed_mph, 120);
    EXPECT_FLOAT_EQ(vehicle_alpha->battery_capacity_kwh, 320);
    EXPECT_FLOAT_EQ(vehicle_alpha->time_to_charge_hrs, 0.6);
    EXPECT_FLOAT_EQ(vehicle_alpha->energy_used_kwh_per_mile, 1.6);
    EXPECT_EQ(vehicle_alpha->passengers_per_vehicle, 4);
    EXPECT_FLOAT_EQ(vehicle_alpha->prob_fault_per_hr, 0.25);
    // derived values
    EXPECT_FLOAT_EQ(vehicle_alpha->max_range_miles, 320.0 / 1.6);  // kwh / khw/mile = miles
    EXPECT_FLOAT_EQ(
        vehicle_alpha->max_flight_time_hrs,
        320.0 / 1.6 / 120);  // miles / miles/hr = hr
    EXPECT_FLOAT_EQ(vehicle_alpha->cruise_power_kw, 320.0 / (320.0 / 1.6 / 120));

    // Randomly populate the correct total number of vehicles
    simulation.populate_vehicles(NUM_VEHICLES);
    // simulation.print_vehicles(); // debugging
    EXPECT_EQ(simulation._vehicles.size(), 20);

    simulation.run();
    // simulation.print_results(); // debugging

    // Ensure each vehicle has a total flight+waiting+charging time duration equal to the simulation
    // duration, +/- some delta
    constexpr double allowed_delta_hrs = 0.01;
    for (size_t i = 0; i < simulation._vehicles.size(); i++)
    {
        const Vehicle_stats& stats = simulation._vehicles[i].stats;
        EXPECT_RANGE(
            simulation_duration_hrs,
            stats.flight_time_hrs + stats.wait_time_hrs + stats.charge_time_hrs - allowed_delta_hrs,
            stats.flight_time_hrs + stats.wait_time_hrs + stats.charge_time_hrs + allowed_delta_hrs
            ) << "i = " << i << "\n";
    }
}

/// Do a trivial end-to-end simulation where all the vehicles do is fly and charge (no waiting).
/// Ensure that the total flight and charge times are as expected.
TEST(Simulation, TrivialEndToEnd)
{
    constexpr uint32_t num_chargers = 3;
    constexpr double simulation_duration_hrs = 3.0;
    constexpr double simulation_step_size_hrs = 1.0 / (double)SECONDS_PER_HR;

    Simulation simulation{num_chargers, simulation_duration_hrs, simulation_step_size_hrs};

    // clang-format off
    simulation.add_vehicle_type({"Alpha",    120, 320, 0.6,  1.6, 4, 0.25});
    simulation.add_vehicle_type({"Bravo",    100, 100, 0.2,  1.5, 5, 0.10});
    simulation.add_vehicle_type({"Charlie",  160, 220, 0.8,  2.2, 3, 0.05});
    // clang-format on

    // simulation.print_vehicle_types(); // debugging

    // Force 1 of each vehicle above
    simulation._vehicles.emplace_back(Vehicle{&simulation._vehicle_types[0]});
    simulation._vehicles.emplace_back(Vehicle{&simulation._vehicle_types[1]});
    simulation._vehicles.emplace_back(Vehicle{&simulation._vehicle_types[2]});

    // Now run the simulation and check for expected results

    simulation.run();
    // simulation.print_results(); // debugging

    constexpr double allowed_error = 0.01;

    const Vehicle_type_stats* stats = nullptr;

    // Alpha
    stats = &(simulation._vehicle_types[0].stats);
    EXPECT_EQ(stats->total_num_flights, 2);
    EXPECT_RANGE(
        stats->total_num_passenger_miles, 1151.87 - allowed_error, 1151.87 + allowed_error);
    // TODO: check for other expected values in `stats` here.

    // Bravo
    stats = &(simulation._vehicle_types[1].stats);
    EXPECT_EQ(stats->total_num_flights, 4);
    EXPECT_RANGE(
        stats->total_num_passenger_miles, 1199.58 - allowed_error, 1199.58 + allowed_error);
    // TODO: check for other expected values in `stats` here.

    // Charlie
    stats = &(simulation._vehicle_types[2].stats);
    EXPECT_EQ(stats->total_num_flights, 3);
    EXPECT_RANGE(stats->total_num_passenger_miles, 671.60 - allowed_error, 671.60 + allowed_error);
    // TODO: check for other expected values in `stats` here.
}

/*
Gtest unit tests.

To install gtest, see my instructions here: https://stackoverflow.com/a/75718815/4561887

To learn to use gtest, here are some helpful docs to get started:
1. https://github.com/google/googletest/tree/main/docs - see all of the docs herein.
    1. Note: I wrote this one:
       https://github.com/google/googletest/blob/main/docs/community_created_documentation.md
1. https://github.com/google/googletest/blob/main/docs/advanced.md#testing-private-code
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
class SimulationTestFixture : public testing::Test
{
protected:
    // provide getters to expose private members of class `Simulation`, for testing

    std::vector<Vehicle_type>* get_vehicle_types(Simulation* simulation)
    {
        return &simulation->_vehicle_types;
    }
};

// anonymous namespace
namespace
{

TEST_F(SimulationTestFixture, Todo)
{

}

////// Unit test: ensure flight time + wait time + charge time for all 3 vehicles is ~ 3 hrs (+/- 0.01 hrs)

} // anonymous namespace


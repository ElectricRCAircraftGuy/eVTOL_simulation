/*
Main simulation file

Todo:
1. [ ] break apart into multiple modules, each with a header and source file.
1. [ ] add setters and getters, perhaps

Provided Assumptions
- Each vehicle starts the simulation with a fully-charged battery
- Each vehicle instantaneously reaches Cruise Speed
- Each vehicle is airborne for the full use of the battery, and is immediately in line for the
  charger after running out of battery power.

Additional Assumptions:
- faults don't make the vehicle stop flying; it keeps flying; you just count the faults
- when the plane runs out of fuel it is instantly at the charger without adding distance
- only charge when the battery is empty (at 0% State of Charge (SoC))
- "the probability of fault per hour" statistic should be evenly spread across the time steps, to
  provide a more-realistic "real-life" simulation, providing for the ability to run this simulation
  "real time" if you wanted to train ground support personal to actually respond to the emergencies
  and operations at the predicted real-time cadence.
- once a vehicle gets on the charger, it will stay there until full

*/

// C++ includes
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>


#define SECONDS_PER_HR 3600UL

// TODO: read these as input arguments to `main()`, to rapidly run a variety of simulations.
constexpr uint32_t NUM_VEHICLES = 20;
constexpr uint32_t NUM_CHARGERS = 3;
constexpr double SIMULATION_DURATION_HRS = 3.0;
constexpr double SIMULATION_STEP_SIZE_HRS = 1.0/(double)SECONDS_PER_HR; /// 1 second time step size


struct Vehicle_type_stats
{
    // required to publish
    double avg_flight_time_per_flight_hrs = 0;
    double avg_distance_per_flight_miles = 0;
    double avg_charge_time_per_session_hrs = 0;
    uint32_t total_num_faults = 0;
    uint32_t total_num_passenger_miles = 0;

    // extras
    // - required to calculate the values above
    uint32_t total_num_flights = 0;
    double total_flight_time_hrs = 0;
    double total_distance_miles = 0;
    uint32_t total_num_charges = 0;
    double total_charge_time_hrs = 0;
};

struct Vehicle_type
{
    // primary values

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

    // derived values

    const double max_range_miles; // on a single charge
    const double max_flight_time_hrs; // on a single charge
    const double cruise_power_kw;
    /// mean probability of faults per second
    const double prob_fault_per_sec;

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
        // primary values
        , cruise_speed_mph{cruise_speed_mph_}
        , battery_capacity_kwh{battery_capacity_kwh_}
        , time_to_charge_hrs{time_to_charge_hrs_}
        , energy_used_kwh_per_mile{energy_used_kwh_per_mile_}
        , passenger_cnt{passenger_cnt_}
        , prob_fault_per_hr{prob_fault_per_hr_}
        // derived values
        , max_range_miles{battery_capacity_kwh/energy_used_kwh_per_mile}
        , max_flight_time_hrs{max_range_miles/cruise_speed_mph}
        , cruise_power_kw{battery_capacity_kwh/max_flight_time_hrs}
        , prob_fault_per_sec{prob_fault_per_hr/(double)SECONDS_PER_HR}
    {}

    void print() const
    {
        printf(
            "%-10s\n"
            "Primary values:\n"
            "%8.2f %8.2f %6.2f %6.2f %4u %6.2f\n"
            "Derived values:\n"
            "%6.2f, %6.2f, %6.2f, %6.2f\n",
            name.c_str(),
            // primary values
            cruise_speed_mph,
            battery_capacity_kwh,
            time_to_charge_hrs,
            energy_used_kwh_per_mile,
            passenger_cnt,
            prob_fault_per_hr,
            // derived values
            max_range_miles,
            max_flight_time_hrs,
            cruise_power_kw,
            prob_fault_per_sec
        );
    }
};

/// State machine for each vehicle
enum class Vehicle_state
{
    FLYING = 0,
    WAITING_FOR_CHARGER,
    CHARGING,
};

struct Vehicle_stats
{
    // cumulative stats

    uint32_t num_flights = 0;
    double flight_time_hrs = 0;
    double distance_traveled_miles = 0;

    uint32_t num_charge_sessions = 0;
    double charge_time_hrs = 0;

    uint32_t num_faults = 0;

    Vehicle_state vehicle_state = Vehicle_state::FLYING;
};

class Vehicle
{
public:
    // constructor
    Vehicle(Vehicle_type* vehicle_type_)
    : vehicle_type{vehicle_type_}
    {}

    Vehicle_type* vehicle_type; // ptr to the vehicle type this vehicle is
    Vehicle_stats stats;
private:
};

class Simulation
{
public:
    // constructor
    Simulation(
        uint32_t num_chargers,
        double simulation_duration_hrs,
        double simulation_step_size_hrs)
        : _num_chargers{num_chargers}
        , _simulation_duration_hrs{simulation_duration_hrs}
        , _simulation_step_size_hrs{simulation_step_size_hrs}
    {
    }

    // returns true if successful and false otherwise
    // TODO: improve error handling; ex: return enums instead. Ex: see `enum class Error_code`
    // and `struct Error` in my code here:
    // https://github.com/ElectricRCAircraftGuy/eRCaGuy_hello_world/blob/master/cpp/curl_lib_WIP.h#L49-L87
    bool add_vehicle_type(Vehicle_type vehicle_type)
    {
        // don't add the same vehicle type more than once
        if (_vehicle_type_names.count(vehicle_type.name) == 1)
        {
            printf("Error: this vehicle type (%s) was already added.\n", vehicle_type.name.c_str());
            return false;
        }

        _vehicle_type_names.insert(vehicle_type.name);
        _vehicle_types.push_back(vehicle_type);
        return true;
    }

    void populate_vehicles(uint32_t num_vehicles)
    {
        std::uniform_int_distribution<uint32_t> distribution(0, _vehicle_types.size() - 1);

        for (uint32_t i = 0; i < num_vehicles; i++)
        {
            // get a random number from the index range in the distribution, and then add a vehicle
            // of this type
            uint32_t i_vehicle_type = distribution(_generator);
            Vehicle random_vehicle{&_vehicle_types[i_vehicle_type]};
            _vehicles.push_back(random_vehicle);
        }
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
        printf("Vehicles:\n");
        for (size_t i = 0; i < _vehicles.size(); i++)
        {
            printf("%3lu: %s\n", i, _vehicles[i].vehicle_type->name.c_str());
        }
        printf("\n");
    }

private:
    std::vector<Vehicle_type> _vehicle_types;
    std::vector<Vehicle> _vehicles;
    /// Used to keep track of whether or not a particular vehicle type has already been added
    std::unordered_set<std::string> _vehicle_type_names;

    const uint32_t _num_chargers;
    const double _simulation_duration_hrs;
    const double _simulation_step_size_hrs;

    // For random number generation

    std::random_device _random_device;
    /// Standard mersenne_twister_engine
    std::mt19937 _generator{_random_device()};
};

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

    // Randomly populate the correct number of vehicles
    simulation.populate_vehicles(NUM_VEHICLES);
    simulation.print_vehicles(); /////// index and name only

    //////////
    // simulation.run();
    // simulation.print_results();

    return 0;
}

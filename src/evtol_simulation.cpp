/*
Main simulation file

Provided Assumptions
- Each vehicle starts the simulation with a fully-charged battery
- Each vehicle instantaneously reaches Cruise Speed
- Each vehicle is airborne for the full use of the battery, and is immediately in line for the
  charger after running out of battery power.

Additional Assumptions:
- faults can only occur during **flying**, NOT during waiting for a charger, or charging.
- faults don't make the vehicle stop flying; it keeps flying; you just count the faults
- when the plane runs out of fuel it is instantly at the charger without adding distance
- only charge when the battery is empty (at 0% State of Charge (SoC))
- "the probability of fault per hour" statistic should be evenly spread across the time steps, to
  provide a more-realistic "real-life" simulation, providing for the ability to run this simulation
  "real time" if you wanted to train ground support personal to actually respond to the emergencies
  and operations at the predicted real-time cadence.
- once a vehicle gets on the charger, it will stay there until full
- each vehicle is fully loaded with passengers every flight

*/

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


struct Vehicle_type_stats
{
    // required to publish
    double avg_flight_time_per_flight_hrs = 0;
    double avg_distance_per_flight_miles = 0;
    double avg_charge_time_per_session_hrs = 0;
    uint32_t total_num_faults = 0;
    double total_num_passenger_miles = 0;

    // extras
    // - required to calculate the values above
    uint32_t total_num_flights = 0;
    double total_flight_time_hrs = 0;
    double total_distance_miles = 0;
    uint32_t total_num_charges = 0;
    double total_charge_time_hrs = 0;
    uint32_t num_vehicles = 0; /// the total number of vehicles of this type
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
    const uint32_t passengers_per_vehicle;
    /// mean probability of faults per hour
    const double prob_fault_per_hr;

    // derived values

    const double max_range_miles; // on a single charge
    const double max_flight_time_hrs; // on a single charge
    const double cruise_power_kw;
    /// mean probability of faults per second
    const double prob_fault_per_sec;

    Vehicle_type_stats stats;

    // constructor
    Vehicle_type(
            std::string name_,
            double cruise_speed_mph_,
            double battery_capacity_kwh_,
            double time_to_charge_hrs_,
            double energy_used_kwh_per_mile_,
            uint32_t passengers_per_vehicle_,
            double prob_fault_per_hr_
        )
        : name{name_}
        // primary values
        , cruise_speed_mph{cruise_speed_mph_}
        , battery_capacity_kwh{battery_capacity_kwh_}
        , time_to_charge_hrs{time_to_charge_hrs_}
        , energy_used_kwh_per_mile{energy_used_kwh_per_mile_}
        , passengers_per_vehicle{passengers_per_vehicle_}
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
            "%-10s Primary values: %8.2f %8.2f %6.2f %6.8f %4u %6.2f\n"
            "           Derived values: %8.2f %8.2f %6.2f %6.8f\n\n",
            name.c_str(),
            // primary values
            cruise_speed_mph,
            battery_capacity_kwh,
            time_to_charge_hrs,
            energy_used_kwh_per_mile,
            passengers_per_vehicle,
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
    double distance_miles = 0;

    uint32_t num_charges = 0; /// number of charge sessions
    double charge_time_hrs = 0;

    uint32_t num_faults = 0;

    // other info

    /// how full the battery currently is, this flight
    double battery_state_of_charge_kwh;

    Vehicle_state state = Vehicle_state::FLYING;
    Vehicle_state last_state = Vehicle_state::CHARGING;
};

struct Vehicle
{
    // constructor
    Vehicle(Vehicle_type* type_)
    : type{type_}
    {
        // start the vehicle out with a fully-charged battery
        stats.battery_state_of_charge_kwh = type->battery_capacity_kwh;
    }

    // ptr to the vehicle type this vehicle is; don't copy the whole `Vehicle_type` struct into
    // each vehicle; just point to the data
    Vehicle_type* type;
    Vehicle_stats stats;
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
        , _num_chargers_available{_num_chargers}
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
        printf(
            "Vehicles:\n"
            "  i  name\n"
            "----------\n");
        for (size_t i = 0; i < _vehicles.size(); i++)
        {
            printf("%3lu: %s\n",
                i, _vehicles[i].type->name.c_str());
        }
        printf("\n\n");
    }

    /// Run the whole simulation for all vehicles
    void run()
    {
        uint32_t num_steps = _simulation_duration_hrs/_simulation_step_size_hrs;
        DEBUG_PRINTF("num_steps = %u\n", num_steps);

        // for all time steps
        for (uint32_t i = 0; i < num_steps; i++)
        {
            // for all vehicles
            for (Vehicle& vehicle : _vehicles)
            {
                iterate(&vehicle);
            }
        }

        printf("Done running simulation. Calculating results.\n\n");

        // For all vehicles, sum up the stats by vehicle type.
        size_t i = 0;
        for (Vehicle& vehicle : _vehicles)
        {
            DEBUG_PRINTF(
                "%3lu: %10s, num_flights = %u, flight_time_hrs = %f, distance_miles = %f, num_charges = %u, "
                "charge_time_hrs = %f, num_faults = %u\n",
                i,
                vehicle.type->name.c_str(),
                vehicle.stats.num_flights,
                vehicle.stats.flight_time_hrs,
                vehicle.stats.distance_miles,
                vehicle.stats.num_charges,
                vehicle.stats.charge_time_hrs,
                vehicle.stats.num_faults
            );

            // sum the totals by vehicle type
            (vehicle.type->stats.num_vehicles)++;
            vehicle.type->stats.total_num_flights += vehicle.stats.num_flights;
            vehicle.type->stats.total_flight_time_hrs += vehicle.stats.flight_time_hrs;
            vehicle.type->stats.total_distance_miles += vehicle.stats.distance_miles;
            vehicle.type->stats.total_num_charges += vehicle.stats.num_charges;
            vehicle.type->stats.total_charge_time_hrs += vehicle.stats.charge_time_hrs;
            vehicle.type->stats.total_num_faults += vehicle.stats.num_faults;

            i++;
        }

        // calculate additional compound stats by vehicle type
        for (Vehicle_type& vehicle_type : _vehicle_types)
        {
            // passenger miles = num_passengers * num_miles
            vehicle_type.stats.total_num_passenger_miles = vehicle_type.stats.num_vehicles*
                vehicle_type.passengers_per_vehicle*vehicle_type.stats.total_distance_miles;

            vehicle_type.stats.avg_flight_time_per_flight_hrs
                = vehicle_type.stats.total_flight_time_hrs/vehicle_type.stats.total_num_flights;
            vehicle_type.stats.avg_distance_per_flight_miles
                = vehicle_type.stats.total_distance_miles/vehicle_type.stats.total_num_flights;
            vehicle_type.stats.avg_charge_time_per_session_hrs
                = vehicle_type.stats.total_charge_time_hrs/vehicle_type.stats.total_num_charges;
        }
    }

    /// Print required simulation results
    void print_results()
    {
        printf("\nResults by vehicle type:\n\n");

        for (Vehicle_type& vehicle_type : _vehicle_types)
        {
            printf(
                "Vehicle type: %s\n"
                "Extra data:\n"
                "  num_vehicles                     = %u\n"
                "  total_num_flights                = %u\n"
                "  total_flight_time_hrs            = %f\n"
                "  total_distance_miles             = %f\n"
                "  total_num_charges                = %u\n"
                "  total_charge_time_hrs            = %f\n"
                /////////// avg passenger miles per vehicle
                "Required data:\n"
                "  avg_flight_time_per_flight_hrs   = %f\n"
                "  avg_distance_per_flight_miles    = %f\n"
                "  avg_charge_time_per_session_hrs  = %f\n"
                "  total_num_faults                 = %u\n"
                "  total_num_passenger_miles        = %f\n\n",
                vehicle_type.name.c_str(),
                // extra data
                vehicle_type.stats.num_vehicles,
                vehicle_type.stats.total_num_flights,
                vehicle_type.stats.total_flight_time_hrs,
                vehicle_type.stats.total_distance_miles,
                vehicle_type.stats.total_num_charges,
                vehicle_type.stats.total_charge_time_hrs,
                // required data
                vehicle_type.stats.avg_flight_time_per_flight_hrs,
                vehicle_type.stats.avg_distance_per_flight_miles,
                vehicle_type.stats.avg_charge_time_per_session_hrs,
                vehicle_type.stats.total_num_faults,
                vehicle_type.stats.total_num_passenger_miles
            );
        }
    }

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
    void check_for_fault(Vehicle* vehicle)
    {
        double random_num = _dist_0_to_1(_generator);
        double prob_fault_this_iteration
            = vehicle->type->prob_fault_per_hr*_simulation_step_size_hrs;
        if (random_num <= prob_fault_this_iteration)
        {
            (vehicle->stats.num_faults)++;
        }
    }

    /// Start charging now if a charger is free; otherwise, get in line to charge
    void try_to_charge(Vehicle* vehicle)
    {
        if (_num_chargers_available > 0)
        {
            // start charging
            _num_chargers_available--;
            vehicle->stats.state = Vehicle_state::CHARGING;
            (vehicle->stats.num_charges)++;
            return;
        }

        // get in the charge line
        vehicle->stats.state = Vehicle_state::WAITING_FOR_CHARGER;
    }

    /// Iterate one time step forward in the simulation for one vehicle
    void iterate(Vehicle* vehicle)
    {
        Vehicle_state state_at_start = vehicle->stats.state;

        switch (vehicle->stats.state)
        {
        case Vehicle_state::FLYING:
        {
            if (vehicle->stats.last_state == Vehicle_state::CHARGING)
            {
                // We just started a new flight, so increment the flight counter
                (vehicle->stats.num_flights)++;
            }

            vehicle->stats.flight_time_hrs += _simulation_step_size_hrs;

            double distance_this_itn_miles =
                vehicle->type->cruise_speed_mph*_simulation_step_size_hrs;
            vehicle->stats.distance_miles += distance_this_itn_miles;

            check_for_fault(vehicle);

            // check for conditions of next state, which are that if the vehicle is out of battery
            // (it has traveled its max range in this case), then it must recharge or get in line
            // to recharge

            // Note: alternatively, I could calculate the discharge rate here, in kW, and
            // use that to adjust the energy used this iteration.
            double energy_used_this_iteration_kwh
                = distance_this_itn_miles*vehicle->type->energy_used_kwh_per_mile;
            vehicle->stats.battery_state_of_charge_kwh -= energy_used_this_iteration_kwh;

            if (vehicle->stats.battery_state_of_charge_kwh <= 0)
            {
                try_to_charge(vehicle);
            }

            break;
        }
        case Vehicle_state::WAITING_FOR_CHARGER:
        {
            try_to_charge(vehicle);
            break;
        }
        case Vehicle_state::CHARGING:
        {
            vehicle->stats.charge_time_hrs += _simulation_step_size_hrs;

            double charge_rate_kw
                = vehicle->type->battery_capacity_kwh/vehicle->type->time_to_charge_hrs;

            double charge_energy_this_itn_kwh = charge_rate_kw*_simulation_step_size_hrs;

            vehicle->stats.battery_state_of_charge_kwh += charge_energy_this_itn_kwh;

            // if you're fully charged, get off the charger and start flying again!
            if (vehicle->stats.battery_state_of_charge_kwh >= vehicle->type->battery_capacity_kwh)
            {
                _num_chargers_available++;
                vehicle->stats.state = Vehicle_state::FLYING;
            }

            break;
        }
        }

        vehicle->stats.last_state = state_at_start;
    }
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

    // Randomly populate the correct total number of vehicles
    simulation.populate_vehicles(NUM_VEHICLES);
    simulation.print_vehicles();

    simulation.run();
    simulation.print_results();

    return 0;
}

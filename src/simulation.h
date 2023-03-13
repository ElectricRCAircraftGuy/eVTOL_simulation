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
                "%3lu: %10s, num_flights = %u, flight_time_hrs = %f, distance_miles = %f, "
                "num_times_waiting = %u, wait_time_hrs = %f, num_charges = %u, "
                "charge_time_hrs = %f, flight+wait+charge-time(hrs) = %f, num_faults = %u\n",
                i,
                vehicle.type->name.c_str(),
                vehicle.stats.num_flights,
                vehicle.stats.flight_time_hrs,
                vehicle.stats.distance_miles,
                vehicle.stats.num_times_waiting,
                vehicle.stats.wait_time_hrs,
                vehicle.stats.num_charges,
                vehicle.stats.charge_time_hrs,
                vehicle.stats.flight_time_hrs + vehicle.stats.wait_time_hrs
                    + vehicle.stats.charge_time_hrs,
                vehicle.stats.num_faults
            );

            // sum the totals by vehicle type
            (vehicle.type->stats.num_vehicles)++;
            vehicle.type->stats.total_num_flights += vehicle.stats.num_flights;
            vehicle.type->stats.total_flight_time_hrs += vehicle.stats.flight_time_hrs;
            vehicle.type->stats.total_distance_miles += vehicle.stats.distance_miles;
            vehicle.type->stats.total_num_times_waiting += vehicle.stats.num_times_waiting;
            vehicle.type->stats.total_wait_time_hrs += vehicle.stats.wait_time_hrs;
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
        printf("\nResults by vehicle type:\n"
               "- The most important results, in my opinion, are marked with \"<====\".\n\n");

        for (Vehicle_type& vehicle_type : _vehicle_types)
        {
            printf(
                "Vehicle type: %s\n"
                "  Extra data:\n"
                "    num_vehicles                     = %u\n"
                "    total_num_flights                = %u\n"
                "    total_flight_time_hrs            = %f\n"
                "    total_distance_miles             = %f\n"
                "    total_num_charges                = %u\n"
                "    total_charge_time_hrs            = %f\n"
                "    total_num_times_waiting          = %u\n"
                "    total_wait_time_hrs              = %f\n"
                "    sum of all 3 times (hrs)         = %f\n"
                "    avg faults per vehicle           = %f  <==\n"
                "    avg passenger miles per vehicle  = %f  <====\n"
                "  Required data:\n"
                "    avg_flight_time_per_flight_hrs   = %f\n"
                "    avg_distance_per_flight_miles    = %f\n"
                "    avg_charge_time_per_session_hrs  = %f\n"
                "    total_num_faults                 = %u\n"
                "    total_num_passenger_miles        = %f\n\n",
                vehicle_type.name.c_str(),
                // extra data
                vehicle_type.stats.num_vehicles,
                vehicle_type.stats.total_num_flights,
                vehicle_type.stats.total_flight_time_hrs,
                vehicle_type.stats.total_distance_miles,
                vehicle_type.stats.total_num_charges,
                vehicle_type.stats.total_charge_time_hrs,
                vehicle_type.stats.total_num_times_waiting,
                vehicle_type.stats.total_wait_time_hrs,
                vehicle_type.stats.total_flight_time_hrs + vehicle_type.stats.total_charge_time_hrs
                    + vehicle_type.stats.total_wait_time_hrs,
                (double)(vehicle_type.stats.total_num_faults)/vehicle_type.stats.num_vehicles,
                vehicle_type.stats.total_num_passenger_miles/vehicle_type.stats.num_vehicles,
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
        }
        else
        {
            // get in the charge line
            vehicle->stats.state = Vehicle_state::WAITING_FOR_CHARGER;
        }
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
            if (vehicle->stats.last_state != Vehicle_state::WAITING_FOR_CHARGER)
            {
                // We just started waiting, so increment the wait counter
                (vehicle->stats.num_times_waiting)++;
            }

            vehicle->stats.wait_time_hrs += _simulation_step_size_hrs;

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

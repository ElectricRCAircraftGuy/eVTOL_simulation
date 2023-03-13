#include "vehicle.h"

Vehicle_type::Vehicle_type(std::string name_, double cruise_speed_mph_,
                           double battery_capacity_kwh_, double time_to_charge_hrs_,
                           double energy_used_kwh_per_mile_, uint32_t passengers_per_vehicle_,
                           double prob_fault_per_hr_) :
    name{name_}  // primary values
    ,
    cruise_speed_mph{cruise_speed_mph_},
    battery_capacity_kwh{battery_capacity_kwh_},
    time_to_charge_hrs{time_to_charge_hrs_},
    energy_used_kwh_per_mile{energy_used_kwh_per_mile_},
    passengers_per_vehicle{passengers_per_vehicle_},
    prob_fault_per_hr{prob_fault_per_hr_}  // derived values
    ,
    max_range_miles{battery_capacity_kwh / energy_used_kwh_per_mile},
    max_flight_time_hrs{max_range_miles / cruise_speed_mph},
    cruise_power_kw{battery_capacity_kwh / max_flight_time_hrs}
{
}

void Vehicle_type::print() const
{
    printf("%-10s Primary values: %8.2f %8.2f %6.2f %6.8f %4u %6.2f\n"
           "           Derived values: max_range_miles=%.2f max_flight_time_hrs=%.2f "
           "cruise_power_kw=%.2f\n\n",
           name.c_str(),
           // primary values
           cruise_speed_mph, battery_capacity_kwh, time_to_charge_hrs, energy_used_kwh_per_mile,
           passengers_per_vehicle, prob_fault_per_hr,
           // derived values
           max_range_miles, max_flight_time_hrs, cruise_power_kw);
}

Vehicle::Vehicle(Vehicle_type* type_) : type{type_}
{
    // start the vehicle out with a fully-charged battery
    stats.battery_state_of_charge_kwh = type->battery_capacity_kwh;
}

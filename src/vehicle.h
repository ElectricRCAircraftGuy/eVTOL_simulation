/*
Vehicle class module for vehicle types and vehicles.
*/

#pragma once

// local includes
#include "utils.h"

// Linux includes
// NA

// C++ includes
#include <iostream>
#include <string>


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
    uint32_t total_num_times_waiting = 0;
    double total_wait_time_hrs = 0;
    uint32_t total_num_charges = 0;
    double total_charge_time_hrs = 0;
    uint32_t num_vehicles = 0; /// the total number of vehicles of this type
};

/// You need one of these objects per vehicle type
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
        );

    void print() const;
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

    uint32_t num_times_waiting = 0;
    double wait_time_hrs = 0;

    uint32_t num_charges = 0; /// number of charge sessions
    double charge_time_hrs = 0;

    uint32_t num_faults = 0;

    // other info

    /// how full the battery currently is, this flight
    double battery_state_of_charge_kwh;

    Vehicle_state state = Vehicle_state::FLYING;
    Vehicle_state last_state = Vehicle_state::CHARGING;
};

/// You need one of these objects per vehicle
struct Vehicle
{
    // constructor
    Vehicle(Vehicle_type* type_);

    // ptr to the vehicle type this vehicle is; don't copy the whole `Vehicle_type` struct into each
    // vehicle; just point to the data; this way you don't unnecessarily duplicate `Vehicle_type`
    // objects.
    Vehicle_type* type;
    Vehicle_stats stats;
};

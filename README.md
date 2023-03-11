# eVTOL_simulation
a crude simulator to simulate some battery usage, range, recharging and stuff


# Build

```bash
# get this repo
git clone https://github.com/ElectricRCAircraftGuy/eVTOL_simulation.git
cd eVTOL_simulation
git lfs pull
git submodule update --init --recursive

# install dependencies

sudo apt install ccache
# git lfs; see: https://packagecloud.io/github/git-lfs/install
curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash




# build and run gtest unit tests

# build and run the simulation
```


```bash
# build and run
./build.sh

# just run
bin/evtol_simulation
```


# Sample run and output


## Run 1

```bash
eVTOL_simulation$ ./build.sh 
Building and running evtol_simulation.
Building...

real    0m0.954s
user    0m0.865s
sys 0m0.089s

Running...
Running simulation

Vehicle types:
Alpha      Primary values:   120.00   320.00   0.60 1.60000000    4   0.25
           Derived values:   200.00     1.67 192.00 0.00006944

Bravo      Primary values:   100.00   100.00   0.20 1.50000000    5   0.10
           Derived values:    66.67     0.67 150.00 0.00002778

Charlie    Primary values:   160.00   220.00   0.80 2.20000000    3   0.05
           Derived values:   100.00     0.62 352.00 0.00001389

Delta      Primary values:    90.00   120.00   0.62 0.80000000    2   0.22
           Derived values:   150.00     1.67  72.00 0.00006111

Echo       Primary values:    30.00   150.00   0.30 5.80000000    2   0.61
           Derived values:    25.86     0.86 174.00 0.00016944

Vehicles:
  i  name
----------
  0: Charlie
  1: Charlie
  2: Charlie
  3: Alpha
  4: Charlie
  5: Bravo
  6: Echo
  7: Charlie
  8: Echo
  9: Bravo
 10: Echo
 11: Delta
 12: Alpha
 13: Bravo
 14: Echo
 15: Alpha
 16: Echo
 17: Charlie
 18: Charlie
 19: Delta


DEBUG: num_steps = 10800
Done running simulation. Calculating results.

DEBUG:   0:    Charlie, num_flights = 2, flight_time_hrs = 1.250556, distance_miles = 200.088889, num_charges = 1, charge_time_hrs = 0.800556, num_faults = 0
DEBUG:   1:    Charlie, num_flights = 2, flight_time_hrs = 1.250556, distance_miles = 200.088889, num_charges = 1, charge_time_hrs = 0.800556, num_faults = 0
DEBUG:   2:    Charlie, num_flights = 2, flight_time_hrs = 1.250556, distance_miles = 200.088889, num_charges = 1, charge_time_hrs = 0.800556, num_faults = 0
DEBUG:   3:      Alpha, num_flights = 1, flight_time_hrs = 1.666667, distance_miles = 200.000000, num_charges = 0, charge_time_hrs = 0.000000, num_faults = 0
DEBUG:   4:    Charlie, num_flights = 2, flight_time_hrs = 1.250556, distance_miles = 200.088889, num_charges = 1, charge_time_hrs = 0.800556, num_faults = 0
DEBUG:   5:      Bravo, num_flights = 2, flight_time_hrs = 1.334444, distance_miles = 133.444444, num_charges = 1, charge_time_hrs = 0.200278, num_faults = 1
DEBUG:   6:       Echo, num_flights = 2, flight_time_hrs = 1.725000, distance_miles = 51.750000, num_charges = 1, charge_time_hrs = 0.300278, num_faults = 3
DEBUG:   7:    Charlie, num_flights = 2, flight_time_hrs = 1.198611, distance_miles = 191.777778, num_charges = 1, charge_time_hrs = 0.800556, num_faults = 0
DEBUG:   8:       Echo, num_flights = 2, flight_time_hrs = 1.725000, distance_miles = 51.750000, num_charges = 1, charge_time_hrs = 0.300278, num_faults = 0
DEBUG:   9:      Bravo, num_flights = 2, flight_time_hrs = 1.334444, distance_miles = 133.444444, num_charges = 1, charge_time_hrs = 0.200278, num_faults = 0
DEBUG:  10:       Echo, num_flights = 2, flight_time_hrs = 1.335556, distance_miles = 40.066667, num_charges = 1, charge_time_hrs = 0.300278, num_faults = 1
DEBUG:  11:      Delta, num_flights = 2, flight_time_hrs = 1.820000, distance_miles = 163.800000, num_charges = 1, charge_time_hrs = 0.620278, num_faults = 0
DEBUG:  12:      Alpha, num_flights = 1, flight_time_hrs = 1.666667, distance_miles = 200.000000, num_charges = 1, charge_time_hrs = 0.573333, num_faults = 1
DEBUG:  13:      Bravo, num_flights = 2, flight_time_hrs = 0.939722, distance_miles = 93.972222, num_charges = 1, charge_time_hrs = 0.200278, num_faults = 0
DEBUG:  14:       Echo, num_flights = 1, flight_time_hrs = 0.862222, distance_miles = 25.866667, num_charges = 1, charge_time_hrs = 0.273056, num_faults = 1
DEBUG:  15:      Alpha, num_flights = 1, flight_time_hrs = 1.666667, distance_miles = 200.000000, num_charges = 1, charge_time_hrs = 0.153056, num_faults = 0
DEBUG:  16:       Echo, num_flights = 1, flight_time_hrs = 0.862222, distance_miles = 25.866667, num_charges = 0, charge_time_hrs = 0.000000, num_faults = 2
DEBUG:  17:    Charlie, num_flights = 1, flight_time_hrs = 0.625278, distance_miles = 100.044444, num_charges = 0, charge_time_hrs = 0.000000, num_faults = 0
DEBUG:  18:    Charlie, num_flights = 1, flight_time_hrs = 0.625278, distance_miles = 100.044444, num_charges = 0, charge_time_hrs = 0.000000, num_faults = 0
DEBUG:  19:      Delta, num_flights = 1, flight_time_hrs = 1.666944, distance_miles = 150.025000, num_charges = 0, charge_time_hrs = 0.000000, num_faults = 1
Results by vehicle type:

Vehicle type: Alpha
Extra data:
  num_vehicles                     = 3
  total_num_flights                = 3
  total_flight_time_hrs            = 5.000000
  total_distance_miles             = 600.000000
  total_num_charges                = 2
  total_charge_time_hrs            = 0.726389
Required data:
  avg_flight_time_per_flight_hrs   = 1.666667
  avg_distance_per_flight_miles    = 200.000000
  avg_charge_time_per_session_hrs  = 0.363194
  total_num_faults                 = 1
  total_num_passenger_miles        = 7200.000000

Vehicle type: Bravo
Extra data:
  num_vehicles                     = 3
  total_num_flights                = 6
  total_flight_time_hrs            = 3.608611
  total_distance_miles             = 360.861111
  total_num_charges                = 3
  total_charge_time_hrs            = 0.600833
Required data:
  avg_flight_time_per_flight_hrs   = 0.601435
  avg_distance_per_flight_miles    = 60.143519
  avg_charge_time_per_session_hrs  = 0.200278
  total_num_faults                 = 1
  total_num_passenger_miles        = 5412.916667

Vehicle type: Charlie
Extra data:
  num_vehicles                     = 7
  total_num_flights                = 12
  total_flight_time_hrs            = 7.451389
  total_distance_miles             = 1192.222222
  total_num_charges                = 5
  total_charge_time_hrs            = 4.002778
Required data:
  avg_flight_time_per_flight_hrs   = 0.620949
  avg_distance_per_flight_miles    = 99.351852
  avg_charge_time_per_session_hrs  = 0.800556
  total_num_faults                 = 0
  total_num_passenger_miles        = 25036.666667

Vehicle type: Delta
Extra data:
  num_vehicles                     = 2
  total_num_flights                = 3
  total_flight_time_hrs            = 3.486944
  total_distance_miles             = 313.825000
  total_num_charges                = 1
  total_charge_time_hrs            = 0.620278
Required data:
  avg_flight_time_per_flight_hrs   = 1.162315
  avg_distance_per_flight_miles    = 104.608333
  avg_charge_time_per_session_hrs  = 0.620278
  total_num_faults                 = 1
  total_num_passenger_miles        = 1255.300000

Vehicle type: Echo
Extra data:
  num_vehicles                     = 5
  total_num_flights                = 8
  total_flight_time_hrs            = 6.510000
  total_distance_miles             = 195.300000
  total_num_charges                = 4
  total_charge_time_hrs            = 1.173889
Required data:
  avg_flight_time_per_flight_hrs   = 0.813750
  avg_distance_per_flight_miles    = 24.412500
  avg_charge_time_per_session_hrs  = 0.293472
  total_num_faults                 = 7
  total_num_passenger_miles        = 1953.000000
```


## Run 2

```bash
eVTOL_simulation$ ./build.sh 
Building and running evtol_simulation.
Building...

real    0m0.957s
user    0m0.885s
sys 0m0.072s

Running...
Running simulation

Vehicle types:
Alpha      Primary values:   120.00   320.00   0.60 1.60000000    4   0.25
           Derived values:   200.00     1.67 192.00 0.00006944

Bravo      Primary values:   100.00   100.00   0.20 1.50000000    5   0.10
           Derived values:    66.67     0.67 150.00 0.00002778

Charlie    Primary values:   160.00   220.00   0.80 2.20000000    3   0.05
           Derived values:   100.00     0.62 352.00 0.00001389

Delta      Primary values:    90.00   120.00   0.62 0.80000000    2   0.22
           Derived values:   150.00     1.67  72.00 0.00006111

Echo       Primary values:    30.00   150.00   0.30 5.80000000    2   0.61
           Derived values:    25.86     0.86 174.00 0.00016944

Vehicles:
  i  name
----------
  0: Delta
  1: Delta
  2: Charlie
  3: Bravo
  4: Echo
  5: Delta
  6: Alpha
  7: Alpha
  8: Delta
  9: Alpha
 10: Echo
 11: Bravo
 12: Echo
 13: Charlie
 14: Delta
 15: Bravo
 16: Echo
 17: Echo
 18: Bravo
 19: Echo


DEBUG: num_steps = 10800
Done running simulation. Calculating results.

DEBUG:   0:      Delta, num_flights = 2, flight_time_hrs = 1.919722, distance_miles = 172.775000, num_charges = 1, charge_time_hrs = 0.620278, num_faults = 0
DEBUG:   1:      Delta, num_flights = 2, flight_time_hrs = 1.819722, distance_miles = 163.775000, num_charges = 1, charge_time_hrs = 0.620278, num_faults = 0
DEBUG:   2:    Charlie, num_flights = 2, flight_time_hrs = 1.250556, distance_miles = 200.088889, num_charges = 2, charge_time_hrs = 1.211944, num_faults = 0
DEBUG:   3:      Bravo, num_flights = 3, flight_time_hrs = 1.386944, distance_miles = 138.694444, num_charges = 2, charge_time_hrs = 0.400556, num_faults = 0
DEBUG:   4:       Echo, num_flights = 2, flight_time_hrs = 1.725000, distance_miles = 51.750000, num_charges = 2, charge_time_hrs = 0.453056, num_faults = 1
DEBUG:   5:      Delta, num_flights = 1, flight_time_hrs = 1.666944, distance_miles = 150.025000, num_charges = 1, charge_time_hrs = 0.052500, num_faults = 0
DEBUG:   6:      Alpha, num_flights = 1, flight_time_hrs = 1.666667, distance_miles = 200.000000, num_charges = 0, charge_time_hrs = 0.000000, num_faults = 1
DEBUG:   7:      Alpha, num_flights = 1, flight_time_hrs = 1.666667, distance_miles = 200.000000, num_charges = 0, charge_time_hrs = 0.000000, num_faults = 0
DEBUG:   8:      Delta, num_flights = 1, flight_time_hrs = 1.666944, distance_miles = 150.025000, num_charges = 0, charge_time_hrs = 0.000000, num_faults = 0
DEBUG:   9:      Alpha, num_flights = 1, flight_time_hrs = 1.666667, distance_miles = 200.000000, num_charges = 0, charge_time_hrs = 0.000000, num_faults = 0
DEBUG:  10:       Echo, num_flights = 2, flight_time_hrs = 1.725000, distance_miles = 51.750000, num_charges = 1, charge_time_hrs = 0.300278, num_faults = 2
DEBUG:  11:      Bravo, num_flights = 2, flight_time_hrs = 1.334444, distance_miles = 133.444444, num_charges = 1, charge_time_hrs = 0.200278, num_faults = 2
DEBUG:  12:       Echo, num_flights = 2, flight_time_hrs = 1.725000, distance_miles = 51.750000, num_charges = 1, charge_time_hrs = 0.300278, num_faults = 2
DEBUG:  13:    Charlie, num_flights = 2, flight_time_hrs = 1.250556, distance_miles = 200.088889, num_charges = 1, charge_time_hrs = 0.800556, num_faults = 0
DEBUG:  14:      Delta, num_flights = 2, flight_time_hrs = 2.278889, distance_miles = 205.100000, num_charges = 1, charge_time_hrs = 0.620278, num_faults = 0
DEBUG:  15:      Bravo, num_flights = 3, flight_time_hrs = 1.746111, distance_miles = 174.611111, num_charges = 2, charge_time_hrs = 0.400556, num_faults = 0
DEBUG:  16:       Echo, num_flights = 2, flight_time_hrs = 1.725000, distance_miles = 51.750000, num_charges = 1, charge_time_hrs = 0.300278, num_faults = 0
DEBUG:  17:       Echo, num_flights = 2, flight_time_hrs = 1.725000, distance_miles = 51.750000, num_charges = 1, charge_time_hrs = 0.300278, num_faults = 0
DEBUG:  18:      Bravo, num_flights = 2, flight_time_hrs = 1.334444, distance_miles = 133.444444, num_charges = 1, charge_time_hrs = 0.200278, num_faults = 0
DEBUG:  19:       Echo, num_flights = 2, flight_time_hrs = 1.635556, distance_miles = 49.066667, num_charges = 1, charge_time_hrs = 0.300278, num_faults = 2
Results by vehicle type:

Vehicle type: Alpha
Extra data:
  num_vehicles                     = 3
  total_num_flights                = 3
  total_flight_time_hrs            = 5.000000
  total_distance_miles             = 600.000000
  total_num_charges                = 0
  total_charge_time_hrs            = 0.000000
Required data:
  avg_flight_time_per_flight_hrs   = 1.666667
  avg_distance_per_flight_miles    = 200.000000
  avg_charge_time_per_session_hrs  = -nan
  total_num_faults                 = 1
  total_num_passenger_miles        = 7200.000000

Vehicle type: Bravo
Extra data:
  num_vehicles                     = 4
  total_num_flights                = 10
  total_flight_time_hrs            = 5.801944
  total_distance_miles             = 580.194444
  total_num_charges                = 6
  total_charge_time_hrs            = 1.201667
Required data:
  avg_flight_time_per_flight_hrs   = 0.580194
  avg_distance_per_flight_miles    = 58.019444
  avg_charge_time_per_session_hrs  = 0.200278
  total_num_faults                 = 2
  total_num_passenger_miles        = 11603.888889

Vehicle type: Charlie
Extra data:
  num_vehicles                     = 2
  total_num_flights                = 4
  total_flight_time_hrs            = 2.501111
  total_distance_miles             = 400.177778
  total_num_charges                = 3
  total_charge_time_hrs            = 2.012500
Required data:
  avg_flight_time_per_flight_hrs   = 0.625278
  avg_distance_per_flight_miles    = 100.044444
  avg_charge_time_per_session_hrs  = 0.670833
  total_num_faults                 = 0
  total_num_passenger_miles        = 2401.066667

Vehicle type: Delta
Extra data:
  num_vehicles                     = 5
  total_num_flights                = 8
  total_flight_time_hrs            = 9.352222
  total_distance_miles             = 841.700000
  total_num_charges                = 4
  total_charge_time_hrs            = 1.913333
Required data:
  avg_flight_time_per_flight_hrs   = 1.169028
  avg_distance_per_flight_miles    = 105.212500
  avg_charge_time_per_session_hrs  = 0.478333
  total_num_faults                 = 0
  total_num_passenger_miles        = 8417.000000

Vehicle type: Echo
Extra data:
  num_vehicles                     = 6
  total_num_flights                = 12
  total_flight_time_hrs            = 10.260556
  total_distance_miles             = 307.816667
  total_num_charges                = 7
  total_charge_time_hrs            = 1.954444
Required data:
  avg_flight_time_per_flight_hrs   = 0.855046
  avg_distance_per_flight_miles    = 25.651389
  avg_charge_time_per_session_hrs  = 0.279206
  total_num_faults                 = 7
  total_num_passenger_miles        = 3693.800000
```

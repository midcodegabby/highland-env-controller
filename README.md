# highland-env-controller
A standalone system for controlling irrigation, humidity, and temperature built on FreeRTOS.

### Use-case
This system is able to control the environmental conditions of a 3'x3'x6' grow tent using IR sensors and relays.  
I grow carnivorous plant with very strict environmental requirements (highland plants need low night temperatures,  
frequent watering, and high humidity) in my grow tent. I use this system to automate the environmental controls  
for this setup. 

### Architecture
#### Hardware:
-Nucleo-H723ZG Reference Board

#### Software:
|-FreeRTOS Kernel
|--Additional drivers
-IR transmission for AC
----IR transmission for Humidifier
----Relays for pumps and valves
----

### Supported Equipment
This system supports the following machines:
- Runs on a Nucleo-H723ZG Reference Board
- `insert model` AC
- `insert model` Humidifier
- `insert model` Pump
- `insert model` Valves

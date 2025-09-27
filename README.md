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
```
|- FreeRTOS Kernel
|- Additional drivers
| |-IR transmission for AC
----IR transmission for Humidifier
----Relays for pumps and valves
```

### Supported Equipment
This system supports the following machines:
- Runs on a Nucleo-H723ZG Reference Board
- `insert model` AC
- `insert model` Humidifier
- `insert model` Pump
- `insert model` Valves

### Memory overview
I-cache enabled for all regions; D-cache enabled for D1 domain
- D1 Domain should have D-cache enabled - this is where program data etc. are stored; increase performance
    - NOTE: use WB (default), but need to invalidate/clean for each read/write to MMIO
- D2 Domain should have D-cache disabled (RAM_D2) - no use for MMIO to use caching
    - Use for MMIO DMA buffers: I2C, UART, Ethernet, USB, etc.
- D3 Domain should have D-cache disabled (RAM_D3) - no use for MMIO to use caching
    - Not a lot of need for MMIO DMA buffers, but can use for I2C4, LPUART1, etc.

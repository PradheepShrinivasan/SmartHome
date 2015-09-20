# SmartHome
This repository contains a smart home implementation using 802.15.4
![board](https://lh3.googleusercontent.com/-WD9xhxDIJRA/VcG7q0gcjiI/AAAAAAAAPXw/Qb3Ayk3SpT4/s800-Ic42/DSC00058.JPG)

##Overview

##Hardware 

There were 2 hardware boards that was developed for this project. One was an general purpose AVR development board (shown below) and the other was a board that would connect to the Zigbee device using SPI bus.

![AVR general dev board] (https://lh3.googleusercontent.com/-xXU5mJjgmrk/VcG7TZ-O89I/AAAAAAAAPRM/-IVA6IYXRnE/s640-Ic42/devboard_features.JPG)

### Micro controller
The micro controller that is used for this project is AVR ATmega series(ATmega32/ATmega64/ATmega324P). Atmel controller was chosen because of the integrated peripheral, the availability of open source tools(compiler, programmer)etc. The L version of the atmel controllers was used as CC2420 chips works at 3.3v and 5v.

### Zigbee /802.15.4 radio module
The Zigbee module that was used in this project is the [Easybee module](http://www.rfsolutions.co.uk/acatalog/info_EASYBEE.html) which contains the  [chipcon cc2420](http://www.ti.com/product/cc2420).

### Sensors
The key component of any smart home is the sensors and the devices that the smart home can control. In this project a seperate board for sensor board was created with the below sensors.
![sensor board](https://lh3.googleusercontent.com/-jbCEjTFZGEw/VcG7s6y0ddI/AAAAAAAAPYE/NPLIGWmi_Ag/s800-Ic42/DSC00062.JPG)

#### Temperature Sensor
In order to accurately measure the temperate DS1624 from maxim was used. The reason for choosing this sensor is that its an integrated temperature sensor and connects easily via the I2C bus.

#### Real time clock
The time keeping was performed by the RTC DS1307.

#### PIR sensor
PIR sensor was used to detect the presence of person and based on occupancy, the lights were controlled.

### Ligthing control



### Programmer


##Nodes

### Coordinator

### Router

### Mobile node



##Software

The software was written in atmel studio with WinAVR package.

##Video


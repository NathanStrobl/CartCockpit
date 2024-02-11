//Defines the average distance that the cart can travel on a single charge in meters; if no previous value is known, enter 10000.
#define AVG_CHARGE_DISTANCE_METERS 10000

//Defines the odometers of the cart in meters; if new install, set to 0; otherwise, enter cart's current odometer value.
#define ODOMETER_VALUE_METERS 1234000

/*
Defines the circumfrence of the tire in meters; used for proper speed and distance tracking. Use the formulas below to assist in setting this number:
When measuring tire diagonally using inches: ((PI * INCHES) * 25.4) / 1000
When measuring tire diagonally using centimeters: (PI * CENTIMETERS) * 10
*/
#define TIRE_CIRCUMFRENCE_METERS 1.75

/*
Defines the number of magnets used to measure speed; use the formula below to determine the correct amount of magnets to use:
(TIRE CIRCUMFRENCE IN METERS / 0.2778) * (1000 / SPEED SENSOR INTERVAL MILLISECONDS)
*/
#define MAGNET_QUANTITY 14

//Defines the amount of time between speed measurements; set higher to reduce magents needed; set lower to improve accuracy of readings. 
#define SPEED_SENSOR_INTERVAL_MILLIS 450

//Serial number of the cart.
#define CART_SERIAL_NUMBER "AQ0334-311948"
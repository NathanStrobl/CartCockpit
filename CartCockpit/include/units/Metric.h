/*
Include this file in CartCockpit.ino to have the Arudino display metric units.
*/

//1000 meters to a kilometer.
#define METERS_CONVERSION 1000

//Converts raw speed sensor values to km/h.
#define VELOCITY_CONVERSION 3.6

//Defines metric units and symbols.
#define DISTANCE_UNIT "km"
#define TEMPERATURE_UNIT "oC"
#define BATTERY_FULL_SYMBOL "1"
#define BATTERY_EMPTY_SYMBOL "0"
#define VELOCITY_UNIT "km/h"
#define DISTANCE_PER_CHARGE_UNIT "km/chg."

//Analog input to temperature conversion. 
#define TEMPERATURE_CONVERSION_MULTIPLIER 0.48828125
#define TEMPERATURE_FAHRENHEIT_OFFSET 0

//Sets thresholds for temperature and range displays.
#define FREEZING_TEMPERATURE 4 //Temperatures under 4ºC make temperature indicator blue.
#define HOT_TEMPERATURE 32 //Temperatures about 32ºC make temperature indicator orange.
#define LOW_RANGE_VALUE 5 //Range values under 5km indicate low range.
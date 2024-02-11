/*
Include this file in CartCockpit.ino to have the Arudino display imperial units.
*/

//1609.34 meters to a mile.
#define METERS_CONVERSION 1609.34

//Converts raw speed sensor values to mph.
#define VELOCITY_CONVERSION 2.23694

//Defines imperial units and symbols.
#define DISTANCE_UNIT "mi"
#define TEMPERATURE_UNIT "oF"
#define BATTERY_FULL_SYMBOL "F"
#define BATTERY_EMPTY_SYMBOL "E"
#define VELOCITY_UNIT "mph"
#define DISTANCE_PER_CHARGE_UNIT "mi/chg."

//Analog input to temperature conversion. 
#define TEMPERATURE_CONVERSION_MULTIPLIER 0.878906
#define TEMPERATURE_FAHRENHEIT_OFFSET 32

//Sets thresholds for temperature and range displays.
#define FREEZING_TEMPERATURE 39 //Temperatures under 39ºF make temperature indicator blue.
#define HOT_TEMPERATURE 90 //Temperatures about 90ºF make temperature indicator orange.
#define LOW_RANGE_VALUE 3 //Range values under 3mi indicate low range.
/*
This software is used initalize the Arduino's EEPROM with cart-specific details; the values of these entries can be changed by modiifying the "CartConfiguration.h" file.
*/

//EEPROM drivers and definitions
#include <EEPROM.h>
#define EEPROM_CURRENT_SCREEN_ADDRESS 0
#define EEPROM_AVG_CHG_DIST_ADDRESS 100
#define EEPROM_ODOMETER_ADDRESS 200
#define EEPROM_TIRE_CIRCUMFRENCE_ADDRESS 300
#define EEPROM_MANGET_QUANTITY_ADDRESS 400
#define EEPROM_SPEED_SENSOR_INTERVAL_MILLIS_ADDRESS 500
#define EEPROM_CART_SERIAL_NUMBER_ADDRESS 600

//display drivers, pin definitions, and initialization
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#define DISPLAY_RST 8
#define DISPLAY_DC_RS 9
#define DISPLAY_CS 10
Adafruit_ILI9341 lcd = Adafruit_ILI9341(DISPLAY_CS, DISPLAY_DC_RS, DISPLAY_RST);

//includes configuration file
#include "CartConfiguration.h"

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.setRotation(1);
  lcd.fillScreen(ILI9341_BLACK);

  EEPROM.write(EEPROM_CURRENT_SCREEN_ADDRESS, 4);
  EEPROM.put(EEPROM_AVG_CHG_DIST_ADDRESS, AVG_CHARGE_DISTANCE_METERS);
  EEPROM.put(EEPROM_ODOMETER_ADDRESS, ODOMETER_VALUE_METERS);
  EEPROM.put(EEPROM_TIRE_CIRCUMFRENCE_ADDRESS, TIRE_CIRCUMFRENCE_METERS);
  EEPROM.put(EEPROM_MANGET_QUANTITY_ADDRESS, MAGNET_QUANTITY);
  EEPROM.put(EEPROM_SPEED_SENSOR_INTERVAL_MILLIS_ADDRESS, SPEED_SENSOR_INTERVAL_MILLIS);
  EEPROM.put(EEPROM_CART_SERIAL_NUMBER_ADDRESS, CART_SERIAL_NUMBER);

  int averageChargeDistanceMeters;
  unsigned long odometerMeters;
  float tireCircumfrenceMeters;
  int magnetQuantity;
  int speedSensorMillis;
  char cartSerialNumber[16];

  EEPROM.get(EEPROM_AVG_CHG_DIST_ADDRESS, averageChargeDistanceMeters);
  EEPROM.get(EEPROM_ODOMETER_ADDRESS, odometerMeters);
  EEPROM.get(EEPROM_TIRE_CIRCUMFRENCE_ADDRESS, tireCircumfrenceMeters);
  EEPROM.get(EEPROM_MANGET_QUANTITY_ADDRESS, magnetQuantity);
  EEPROM.get(EEPROM_SPEED_SENSOR_INTERVAL_MILLIS_ADDRESS, speedSensorMillis);
  EEPROM.get(EEPROM_CART_SERIAL_NUMBER_ADDRESS, cartSerialNumber);

  String completionMessage = 
    "CartCockpit Configuration Tool\n"
    "\n"
    "\n"
    "Data successfully written to EEPROM!\n"
    "\n"
    "Values written:\n"
    "Average cart distance per charge: " + String(averageChargeDistanceMeters) + " meters\n"
    "Cart odometer: " + String(odometerMeters) + " meters\n"
    "Tire circumfrence: " + String(tireCircumfrenceMeters) + " meters\n"
    "Speed sensing magnets quantity: " + String(magnetQuantity) + "\n"
    "Speed sensing interval: " + String(speedSensorMillis) + " milliseconds\n"
    "Cart serial number: " + cartSerialNumber + "\n"
    "\n"
    "\n"
    "Please reload the CartCockpit software to continue.";

  lcd.setCursor(0, 0);
  lcd.print(completionMessage);
  Serial.println(completionMessage);
}

void loop() {}

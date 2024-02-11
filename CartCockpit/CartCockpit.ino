/*
This file contains all of the screen drawing routines and serves as the main file for this sketch.

This software was designed for a display with these specifications:
Size: 3.2" diagonal
Resolution: 320x240
Driver: ILI9341
*/


/*
Arduino hardware locations.
*/
//digital pins
#define HALL_SENSOR 2
#define MENU_CHANGE_BUTTON 3
#define HEADLIGHT_CONTROL_BUTTON 5
#define DISPLAY_RST 8
#define DISPLAY_DC_RS 9
#define DISPLAY_CS 10

//analog pins
#define BATTERY_SENSOR A0
#define PHOTOCELL A1
#define TEMPERATURE_SENSOR A2

//EEPROM
#include <EEPROM.h>
#define EEPROM_CURRENT_SCREEN_ADDRESS 0
#define EEPROM_AVG_CHG_DIST_ADDRESS 100
#define EEPROM_ODOMETER_ADDRESS 200
#define EEPROM_TIRE_CIRCUMFRENCE_ADDRESS 300
#define EEPROM_MANGET_QUANTITY_ADDRESS 400
#define EEPROM_SPEED_SENSOR_INTERVAL_MILLIS_ADDRESS 500
#define EEPROM_CART_SERIAL_NUMBER_ADDRESS 600


/*
Conversion and unit-specific values (uncomment the desired units file).
*/
#include "Units/Metric.h";
//#include "Units/Imperial.h"; 


/*
Definitions used to improve readability of certain sections of code.
*/
#define BASE_TEN 10
#define SECONDS_IN_AN_HOUR 3600
#define SOFTWARE_VERSION "Version 0.0b"


/*
Display drivers and add-ons.
*/
//display libraries
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
Adafruit_ILI9341 lcd = Adafruit_ILI9341(DISPLAY_CS, DISPLAY_DC_RS, DISPLAY_RST);

//fonts
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/FreeSansBoldNumbersAndSymbols18pt7b.h"
#include "Fonts/CartCockpitGlyphs.h"
#define NINE_PT 0
#define TWELVE_PT 1
#define EIGHTEEN_PT 2
#define SMALL_SPEEDOMETER 3
#define GLYPHS_SHADOWS 4
#define GLYPHS 5
#define LARGE_SPEEDOMETER 6

//glyphs
#define LIGHTS_TOP "0"
#define LIGHTS_BOTTOM_AUTO "1"
#define LIGHTS_BOTTOM_MANUAL "2"
#define BATTERY_HEALTH_TOP "3"
#define BATTERY_HEALTH_BOTTOM_GOOD "4"
#define BATTERY_HEALTH_BOTTOM_HIGH "5"
#define BATTERY_HEALTH_BOTTOM_LOW "6"
#define BATTERY_HEALTH_BOTTOM_SERV "7"
#define POWER_GAUGE_LIGHTNING_BOLT "8"
#define BATTERY_GAUGE_GLYPH "9"

//custom colors
#define CUSTOM_COLOR_SALMON 0xFC0E
#define CUSTOM_COLOR_DARK_RED 0x8800
#define CUSTOM_COLOR_LIGHT_BROWN 0xF52C
#define CUSTOM_COLOR_MEDIUM_BROWN 0xB409
#define CUSTOM_COLOR_LIGHT_BLUE 0x5CF4
#define CUSTOM_COLOR_MEDIUM_BLUE 0x49F1
#define CUSTOM_COLOR_DARK_GREEN 0x03E0

//screen selection numbering
#define FULL_SCREEN_SPEEDOMETER 0
#define DIGITAL_GAUGES_SCREEN 1
#define TRIP_DATA_SCREEN 2
#define BATTERY_STATUS_SCREEN 3
#define SYSTEM_INFO_SCREEN 4


/*
System objects. 
*/
//timekeeping
#include "SystemTime.h"
#include "Timer.h"
SystemTime* systemTime = new SystemTime();
Timer* temperatureFetchTimer = new Timer(systemTime);
Timer* automaticLightsCheckTimer = new Timer(systemTime);
Timer* speedometerIntervalTimer = new Timer(systemTime);

//lights
#include "Lights.h"
Lights* lights = new Lights();

//odometers
#include "Odometer.h"
Odometer* odometer = new Odometer();
Odometer* tripOdometer = new Odometer();
Odometer* sinceStoppedOdometer = new Odometer();

//battery monitoring system
#include "BatteryMonitoringSystem.h"
BatteryMonitoringSystem* batteryMonitoringSystem = new BatteryMonitoringSystem(0, 0);  //dummy instantiator; reinstantiated in the 'setup' function

//display values
#include "DisplayField.h"
IntegerDisplayField* speedField = new IntegerDisplayField();
IntegerDisplayField* temperatureField = new IntegerDisplayField();
FloatDisplayField* rangeField = new FloatDisplayField();
FloatDisplayField* tripOdometerField = new FloatDisplayField();
UnsignedLongDisplayField* odometerField = new UnsignedLongDisplayField();
IntegerDisplayField* PWRValueField = new IntegerDisplayField();
IntegerDisplayField* stateOfChargeField = new IntegerDisplayField();
IntegerDisplayField* uptimeField = new IntegerDisplayField();
FloatDisplayField* averageSpeedField = new FloatDisplayField();
IntegerDisplayField* chargeUsedField = new IntegerDisplayField();
FloatDisplayField* batteryVoltageField = new FloatDisplayField();
FloatDisplayField* averageChargeDistanceField = new FloatDisplayField();
IntegerDisplayField* lightsGlyphField = new IntegerDisplayField();
IntegerDisplayField* batteryHealthGlyphField = new IntegerDisplayField();
IntegerDisplayField* powerGaugeGlyphColor = new IntegerDisplayField();
IntegerDisplayField* batteryGaugeGlyphColor = new IntegerDisplayField();

//screen selection manager
#include "DisplayUpdater.h"
DisplayUpdater* displayUpdater = new DisplayUpdater(EEPROM.read(EEPROM_CURRENT_SCREEN_ADDRESS));

//speedometer
#include "SpeedSensor.h"
SpeedSensor* speedSensor = new SpeedSensor(0, 0, 0);  //dummy instantiator; reinstantiated in the 'setup' function


/*
Screen drawing functions.
*/
//Text drawing helpers.
int repositionDisplayField(int xValueSingleCharacter, float value, bool valueIsWholeNumber, int style, bool center) {
  float xOffset;
  char valueString[16];

  //sets the x-offset based on the selected style; value is set to half the width of the numerical characters of the selected font (ensures text is centered when repositioned)
  switch(style) {
    case NINE_PT:
      xOffset = 5;
      break;
    case TWELVE_PT:
      xOffset = 6.5;
      break;
    case EIGHTEEN_PT:
      xOffset = 9.5;
      break;
    case SMALL_SPEEDOMETER:
      xOffset = 28.5;
    case LARGE_SPEEDOMETER:
      xOffset = 50;
      break;
  }

  //doubles the x-offset if text is not supposed to be centered
  if (center == false) {
    xOffset = xOffset * 2;
  }

  //determines if value is an integer or float, and adjusts x-offset appropriately
  if (valueIsWholeNumber == true) {
    ltoa(value, valueString, BASE_TEN);
    xOffset = ((strlen(valueString) - 1) * xOffset);
  } else {
    dtostrf(value, 1, 1, valueString);
    xOffset = ((strlen(valueString) - 2) * xOffset) + 9;
  }

  //returns the shifted x-value
  return (xValueSingleCharacter - xOffset);
}

void drawText(int style, int color, int cursorX, int cursorY, char string[]) {
  int shadowOffset;
  int yShadowOffsetAdjust = 0;
  lcd.setTextSize(0);

  //style selector
  switch(style) {
    case NINE_PT:
      lcd.setFont(&FreeSans9pt7b);
      shadowOffset = 1;
      break;
    case TWELVE_PT:
      lcd.setFont(&FreeSansBold12pt7b);
      shadowOffset = 1;
      break;
    case EIGHTEEN_PT:
      lcd.setFont(&FreeSansBold18pt7b);
      shadowOffset = 2;
      yShadowOffsetAdjust = 1;
      break;
    case SMALL_SPEEDOMETER:
      lcd.setTextSize(3);
      lcd.setFont(&FreeSansBold18pt7b);
      shadowOffset = 2;
      break;
    case GLYPHS_SHADOWS:
      lcd.setFont(&CartCockpitGlyphs);
      shadowOffset = 1;
      break;
    case GLYPHS:
      lcd.setFont(&CartCockpitGlyphs);
      shadowOffset = 0;
      break;
    case LARGE_SPEEDOMETER:
      lcd.setTextSize(4);
      lcd.setFont(&FreeSansBold18pt7b);
      shadowOffset = 6;
      break;
  }

  //draws text shadow
  switch(color) {
    case CUSTOM_COLOR_SALMON:
      lcd.setTextColor(CUSTOM_COLOR_DARK_RED);
      break;
    case CUSTOM_COLOR_LIGHT_BROWN:
      lcd.setTextColor(CUSTOM_COLOR_MEDIUM_BROWN);
      break;
    case CUSTOM_COLOR_LIGHT_BLUE:
      lcd.setTextColor(CUSTOM_COLOR_MEDIUM_BLUE);
      break;
    case ILI9341_GREEN:
      lcd.setTextColor(CUSTOM_COLOR_DARK_GREEN);
      break;
    default:
      lcd.setTextColor(ILI9341_DARKGREY);
      break;
  }
  lcd.setCursor(cursorX + shadowOffset, cursorY + shadowOffset - yShadowOffsetAdjust);
  lcd.print(string);

  //sets color and cursor position, then draws the text
  lcd.setTextColor(color);
  lcd.setCursor(cursorX, cursorY);
  lcd.print(string);
}

int getBatteryInformationColor() {
  if (batteryMonitoringSystem->GetStateOfCharge() <= 15) {
    return CUSTOM_COLOR_SALMON;
  } else if (batteryMonitoringSystem->GetConvertedRange(METERS_CONVERSION, sinceStoppedOdometer->GetConvertedOdometerValue(METERS_CONVERSION)) < LOW_RANGE_VALUE) {
    return CUSTOM_COLOR_LIGHT_BROWN;
  }
  return ILI9341_WHITE;
}

//Static element drawing functions.
void drawStaticElements() {
  //bottom status area lines
  lcd.drawPixel(0, 210, ILI9341_WHITE);
  lcd.drawPixel(1, 209, ILI9341_WHITE);
  lcd.drawPixel(2, 209, ILI9341_WHITE);
  lcd.drawLine(3, 208, 316, 208, ILI9341_WHITE);
  lcd.drawPixel(317, 209, ILI9341_WHITE);
  lcd.drawPixel(318, 209, ILI9341_WHITE);
  lcd.drawPixel(319, 210, ILI9341_WHITE);

  //trip odometer
  drawText(NINE_PT, ILI9341_LIGHTGREY, 5, 230, "trip");
  drawText(NINE_PT, ILI9341_WHITE, 82, 230, DISTANCE_UNIT);

  //odometer
  drawText(NINE_PT, ILI9341_LIGHTGREY, 190, 230, "odo");
  drawText(NINE_PT, ILI9341_WHITE, 291, 230, DISTANCE_UNIT);

  //outdoor temperature
  drawText(NINE_PT, ILI9341_LIGHTGREY, 2, 15, "temperature");
  drawText(TWELVE_PT, ILI9341_LIGHTGREY, 60, 50, TEMPERATURE_UNIT);

  //range
  drawText(NINE_PT, ILI9341_LIGHTGREY, 245, 15, "dist. to E");
  drawText(TWELVE_PT, ILI9341_LIGHTGREY, 281, 50, DISTANCE_UNIT);

  //power gauge
  lcd.drawLine(11, 73, 36, 73, ILI9341_WHITE);
  lcd.drawLine(11, 194, 36, 194, ILI9341_WHITE);
  lcd.drawLine(11, 194, 1, 133, ILI9341_WHITE);
  lcd.drawLine(1, 133, 11, 73, ILI9341_WHITE);
  lcd.drawLine(36, 194, 26, 133, ILI9341_WHITE);
  lcd.drawLine(26, 133, 36, 73, ILI9341_WHITE);
  drawText(NINE_PT, ILI9341_LIGHTGREY, 39, 203, "0");
  drawText(NINE_PT, ILI9341_LIGHTGREY, 36, 166, "25");
  drawText(NINE_PT, CUSTOM_COLOR_LIGHT_BROWN, 37, 107, "75");
  drawText(NINE_PT, CUSTOM_COLOR_SALMON, 39, 75, "100");

  //battery gauge
  lcd.drawLine(309, 73, 284, 73, ILI9341_WHITE);
  lcd.drawLine(309, 194, 284, 194, ILI9341_WHITE);
  lcd.drawLine(309, 194, 319, 133, ILI9341_WHITE);
  lcd.drawLine(319, 133, 309, 73, ILI9341_WHITE);
  lcd.drawLine(284, 194, 294, 133, ILI9341_WHITE);
  lcd.drawLine(294, 133, 284, 73, ILI9341_WHITE);
  drawText(NINE_PT, ILI9341_LIGHTGREY, 269, 75, BATTERY_FULL_SYMBOL);
  drawText(NINE_PT, ILI9341_LIGHTGREY, 269, 203, BATTERY_EMPTY_SYMBOL);
}

//Screen-specific static element drawing functions.
void drawTopStatusLine(int color = ILI9341_WHITE) {
  //left side lines and curves
  lcd.drawPixel(0, 57, color);
  lcd.drawPixel(1, 58, color);
  lcd.drawPixel(2, 58, color);
  lcd.drawLine(3, 59, 96, 59, color);
  if (displayUpdater->GetScreenSelection() == FULL_SCREEN_SPEEDOMETER || color == ILI9341_BLACK) {
    lcd.drawPixel(97, 58, color);
    lcd.drawPixel(98, 58, color);
    lcd.drawPixel(99, 57, color);
    lcd.drawLine(100, 56, 109, 0, color);
  } else {
    lcd.drawPixel(97, 60, color);
    lcd.drawPixel(98, 60, color);
    lcd.drawPixel(99, 61, color);
    lcd.drawLine(100, 62, 109, 95, color);
    lcd.drawPixel(110, 96, color);
    lcd.drawPixel(111, 97, color);
    lcd.drawPixel(112, 97, color);
  }

  //right side lines and curves
  lcd.drawPixel(319, 57, color);
  lcd.drawPixel(318, 58, color);
  lcd.drawPixel(317, 58, color);
  lcd.drawLine(316, 59, 224, 59, color);
  if (displayUpdater->GetScreenSelection() == FULL_SCREEN_SPEEDOMETER || color == ILI9341_BLACK) {
    lcd.drawPixel(223, 58, color);
    lcd.drawPixel(222, 58, color);
    lcd.drawPixel(221, 57, color);
    lcd.drawLine(220, 56, 211, 0, color);
  } else {
    lcd.drawPixel(223, 60, color);
    lcd.drawPixel(222, 60, color);
    lcd.drawPixel(221, 61, color);
    lcd.drawLine(220, 62, 211, 95, color);
    lcd.drawPixel(210, 96, color);
    lcd.drawPixel(209, 97, color);
    lcd.drawPixel(208, 97, color);
  }

  //connecting line
  if (displayUpdater->GetScreenSelection() != FULL_SCREEN_SPEEDOMETER || color == ILI9341_BLACK) {
    lcd.drawLine(113, 98, 207, 98, color);
  }
}

void drawScreenSpecificStaticElements() {
  //clears previous contents and redraws top status bar
  lcd.fillRect(69, 60, 195, 148, ILI9341_BLACK);
  lcd.fillRect(115, 0, 100, 96, ILI9341_BLACK);
  drawTopStatusLine(ILI9341_BLACK);
  drawTopStatusLine();

  //handles drawing the static text for all situations
  int screenSelection = displayUpdater->GetScreenSelection();
  switch(screenSelection) {
    case FULL_SCREEN_SPEEDOMETER:
      lcd.drawLine(134, 45, 186, 45, ILI9341_LIGHTGREY);
      drawText(NINE_PT, ILI9341_WHITE, 134, 42, "Speed");
      drawText(TWELVE_PT, ILI9341_LIGHTGREY, 136, 200, VELOCITY_UNIT);
      drawText(LARGE_SPEEDOMETER, ILI9341_WHITE, repositionDisplayField(126, 0, true, LARGE_SPEEDOMETER, true), 165, "0");  //draws this so that value is not blank when changing screen selection
      break;
    case DIGITAL_GAUGES_SCREEN:
      lcd.drawLine(101, 120, 219, 120, ILI9341_LIGHTGREY);
      lcd.drawLine(159, 125, 159, 200, ILI9341_LIGHTGREY);
      drawText(NINE_PT, ILI9341_WHITE, 101, 117, "Digital Gauges");
      drawText(TWELVE_PT, ILI9341_WHITE, 80, 200, "Power");
      drawText(TWELVE_PT, ILI9341_WHITE, 165, 200, "Battery");
      break;
    case TRIP_DATA_SCREEN:
      lcd.drawLine(123, 120, 197, 120, ILI9341_LIGHTGREY);
      lcd.drawLine(82, 151, 238, 151, ILI9341_LIGHTGREY);
      lcd.drawLine(82, 180, 238, 180, ILI9341_LIGHTGREY);
      drawText(NINE_PT, ILI9341_WHITE, 123, 117, "Trip Data");
      drawText(NINE_PT, ILI9341_LIGHTGREY, 103, 145, "time:");
      drawText(NINE_PT, ILI9341_WHITE, 197, 145, "h");
      drawText(NINE_PT, ILI9341_LIGHTGREY, 70, 172, "avg. speed:");
      drawText(NINE_PT, ILI9341_WHITE, 217, 172, VELOCITY_UNIT);
      drawText(NINE_PT, ILI9341_LIGHTGREY, 84, 200, "charge used:");
      break;
    case BATTERY_STATUS_SCREEN:
      lcd.drawLine(103, 120, 217, 120, ILI9341_LIGHTGREY);
      lcd.drawLine(82, 164, 238, 164, ILI9341_LIGHTGREY);
      drawText(NINE_PT, ILI9341_WHITE, 103, 117, "Battery Status");
      drawText(TWELVE_PT, ILI9341_LIGHTGREY, 162, 154, "volts");
      drawText(NINE_PT, ILI9341_LIGHTGREY, 68, 198, "avg.");
      drawText(TWELVE_PT, ILI9341_LIGHTGREY, 173, 198, DISTANCE_PER_CHARGE_UNIT);
      break;
    case SYSTEM_INFO_SCREEN:
      char versionNumber[] = SOFTWARE_VERSION;
      char cartSerialNumber[16];
      EEPROM.get(EEPROM_CART_SERIAL_NUMBER_ADDRESS, cartSerialNumber);
      lcd.drawLine(83, 120, 237, 120, ILI9341_LIGHTGREY);
      drawText(NINE_PT, ILI9341_WHITE, 83, 117, "System Information");
      drawText(TWELVE_PT, ILI9341_WHITE, 91, 147, "CartCockpit");
      drawText(NINE_PT, ILI9341_WHITE, (167 - (strlen(versionNumber) * 5)), 170, versionNumber);
      drawText(NINE_PT, ILI9341_LIGHTGREY, (160 - (strlen(cartSerialNumber) * 5)), 197, cartSerialNumber);
      break;
  }

  //handles drawing the speedometer unit for small speedometer only
  if (screenSelection != FULL_SCREEN_SPEEDOMETER) {
    drawText(SMALL_SPEEDOMETER, ILI9341_WHITE, repositionDisplayField(132, 0, true, SMALL_SPEEDOMETER, true), 74, "0");  //draws this so that value is not blank when changing screen selection
    drawText(NINE_PT, ILI9341_LIGHTGREY, 143, 92, VELOCITY_UNIT);
  }
}

//Value drawing functions.
void updateSpeedometerField(byte speed) {
  if (speedField->ValueHasChanged(speed) || displayUpdater->ForceUpdateNeeded()) {
    char string[2];
    itoa(speed, string, BASE_TEN);
    if (displayUpdater->GetScreenSelection() == FULL_SCREEN_SPEEDOMETER) {
      lcd.fillRect(70, 60, 180, 117, ILI9341_BLACK);
      drawText(LARGE_SPEEDOMETER, ILI9341_WHITE, repositionDisplayField(126, speed, true, LARGE_SPEEDOMETER, true), 165, string);
    } else {
      lcd.fillRect(106, 0, 110, 80, ILI9341_BLACK);
      drawText(SMALL_SPEEDOMETER, ILI9341_WHITE, repositionDisplayField(132, speed, true, SMALL_SPEEDOMETER, true), 74, string);
    }
  }
}

void updateTemperatureField() {
  int temperature = getOutsideTemperature();
  if (temperatureField->ValueHasChanged(temperature) || displayUpdater->ForceUpdateNeeded()) {
    int textColor;

    //sets the text color based on temperature
    if (temperature > HOT_TEMPERATURE) {
      textColor = CUSTOM_COLOR_LIGHT_BROWN;
    } else if (temperature < FREEZING_TEMPERATURE) {
      textColor = CUSTOM_COLOR_LIGHT_BLUE;
    } else {
      textColor = ILI9341_WHITE;
    }

    //draws the value
    lcd.fillRect(0, 20, 57, 32, ILI9341_BLACK);
    char string[3];
    itoa(temperature, string, BASE_TEN);
    drawText(EIGHTEEN_PT, textColor, repositionDisplayField(36, temperature, true, EIGHTEEN_PT, false), 50, string);
  }
}

void updateRangeField() {
  float range = batteryMonitoringSystem->GetConvertedRange(METERS_CONVERSION, sinceStoppedOdometer->GetRawOdometerValue());
  if (rangeField->ValueHasChanged(range) || displayUpdater->ForceUpdateNeeded()) {
    //updates saved value first to ensure correct color is chosen by the getBatteryInformationColor function.
    int textColor = getBatteryInformationColor();

    //clears previous text
    lcd.fillRect(229, 20, 49, 32, ILI9341_BLACK);

    //determines whether to display a decimal or whole value based on how much range is left and draws the value
    if (range < 9.9) {
      char string[3];
      dtostrf(range, 3, 1, string);
      drawText(EIGHTEEN_PT, textColor, repositionDisplayField(257, range, false, EIGHTEEN_PT, false), 50, string);
    } else {
      char string[2];
      itoa(int(range), string, BASE_TEN);
      drawText(EIGHTEEN_PT, textColor, repositionDisplayField(257, int(range), true, EIGHTEEN_PT, false), 50, string);
    }
  }
}

void updateTripOdometerField() {
  float convertedTripOdometer = tripOdometer->GetConvertedOdometerValue(METERS_CONVERSION);
  if (tripOdometerField->ValueHasChanged(convertedTripOdometer) || displayUpdater->ForceUpdateNeeded()) {
    lcd.fillRect(35, 211, 48, 29, ILI9341_BLACK);
    char string[5];
    dtostrf(convertedTripOdometer, 3, 1, string);
    drawText(TWELVE_PT, ILI9341_WHITE, repositionDisplayField(70, convertedTripOdometer, false, TWELVE_PT, false), 230, string);
  }
}

void updateOdometerField() {
  float convertedOdometer = odometer->GetConvertedOdometerValue(METERS_CONVERSION);
  if (odometerField->ValueHasChanged(convertedOdometer) || displayUpdater->ForceUpdateNeeded()) {
    lcd.fillRect(223, 211, 68, 29, ILI9341_BLACK);
    char string[6];
    ultoa(convertedOdometer, string, BASE_TEN);
    drawText(TWELVE_PT, ILI9341_WHITE, repositionDisplayField(275, convertedOdometer, true, TWELVE_PT, false), 230, string);
  }
}

void updatePowerGaugeNumericalField() {
  int PWRValue = batteryMonitoringSystem->GetPWRValue();
  if (PWRValueField->ValueHasChanged(PWRValue) || displayUpdater->ForceUpdateNeeded()) {
    int textColor = ILI9341_WHITE;

    //sets the text color based on the power percentage
    if (PWRValue > 88) {
      textColor = CUSTOM_COLOR_SALMON;
    } else if (PWRValue > 75) {
      textColor = CUSTOM_COLOR_LIGHT_BROWN;
    }

    //draws the value
    lcd.fillRect(65, 135, 60, 45, ILI9341_BLACK);
    char string[3];
    itoa(PWRValue, string, BASE_TEN);
    drawText(EIGHTEEN_PT, textColor, repositionDisplayField(102, PWRValue, true, EIGHTEEN_PT, false), 163, strcat(string, "%"));
  }
}

void updateBatteryGaugeNumericalField() {
  int stateOfCharge = batteryMonitoringSystem->GetStateOfCharge();
  if (stateOfChargeField->ValueHasChanged(stateOfCharge) || displayUpdater->ForceUpdateNeeded()) {
    lcd.fillRect(165, 135, 90, 45, ILI9341_BLACK);
    char string[3];
    itoa(stateOfCharge, string, BASE_TEN);
    drawText(EIGHTEEN_PT, getBatteryInformationColor(), 165, 163, strcat(string, "%"));
  }
}

void updateUptimeField() {
  int uptimeInMinutes = systemTime->GetSystemTimeInMinutes();
  if (uptimeField->ValueHasChanged(uptimeInMinutes) || displayUpdater->ForceUpdateNeeded()) {

    //assembles the string in the H:MM format
    char string[5];
    string[0] = (uptimeInMinutes / 60) + '0';
    string[1] = ':';
    string[2] = ((uptimeInMinutes % 60) / 10) + '0';
    string[3] = (uptimeInMinutes % 10) + '0';
    string[4] = '\0';

    //clears the old value and draws the new value
    lcd.fillRect(140, 128, 55, 20, ILI9341_BLACK);
    drawText(TWELVE_PT, ILI9341_WHITE, 147, 145, string);
  }
}

void updateAverageSpeedField() {
  float convertedAverageSpeed = (tripOdometer->GetRawOdometerValue() / (systemTime->GetSystemTime() * 0.001)) * VELOCITY_CONVERSION;
  if (averageSpeedField->ValueHasChanged(convertedAverageSpeed) || displayUpdater->ForceUpdateNeeded()) {
    char string[4];
    dtostrf(convertedAverageSpeed, 2, 1, string);
    lcd.fillRect(162, 153, 55, 20, ILI9341_BLACK);
    drawText(TWELVE_PT, ILI9341_WHITE, repositionDisplayField(203, convertedAverageSpeed, false, TWELVE_PT, false), 173, string);
  }
}

void updateChargeUsedField() {
  int chargeUsed = batteryMonitoringSystem->GetChargeUsedSinceStart();
  if (stateOfChargeField->ValueHasChanged(chargeUsed) || displayUpdater->ForceUpdateNeeded()) {
    lcd.fillRect(185, 183, 75, 20, ILI9341_BLACK);
    char string[3];
    itoa(chargeUsed, string, BASE_TEN);
    drawText(TWELVE_PT, ILI9341_WHITE, 190, 201, strcat(string, "%"));
  }
}

void updateBatteryVoltageField() {
  float batteryVoltage = batteryMonitoringSystem->GetRealtimeVoltage();
  if (batteryVoltageField->ValueHasChanged(batteryVoltage) || displayUpdater->ForceUpdateNeeded()) {
    int textColor;

    //sets color of text based on voltage
    if (batteryVoltage < 49 || batteryVoltage > 55) {
      textColor = CUSTOM_COLOR_LIGHT_BROWN;
    } else {
      textColor = ILI9341_WHITE;
    }

    //clears old value and draws new value
    lcd.fillRect(80, 125, 82, 32, ILI9341_BLACK);
    char string[4];
    dtostrf(batteryVoltage, 4, 1, string);
    drawText(EIGHTEEN_PT, textColor, 92, 154, string);
  }
}

void updateAverageChargeDistanceField() {
  float convertedAverageChargeDistance = batteryMonitoringSystem->GetConvertedAverageChargeDistance(METERS_CONVERSION);
  if (averageChargeDistanceField->ValueHasChanged(convertedAverageChargeDistance) || displayUpdater->ForceUpdateNeeded()) {
    int textColor = ILI9341_WHITE;

    //sets color of text to orange if distance to E is less than 5km/3mi
    if (convertedAverageChargeDistance < LOW_RANGE_VALUE) {
      textColor = CUSTOM_COLOR_LIGHT_BROWN;
    }

    //clears old value and draws new value
    lcd.fillRect(100, 170, 73, 30, ILI9341_BLACK);
    char string[4];
    dtostrf(convertedAverageChargeDistance, 4, 1, string);
    drawText(EIGHTEEN_PT, textColor, repositionDisplayField(131, 14, false, EIGHTEEN_PT, true), 198, string);
  }
}

void updateLightsStatusField() {
  byte lightsStatus = lights->GetLightsStatus();
  if (lightsGlyphField->ValueHasChanged(lightsStatus) || displayUpdater->ForceUpdateNeeded()) {
    lcd.fillRect(107, 225, 37, 25, ILI9341_BLACK);
    switch(lightsStatus) {
      case LIGHTS_OFF_AUTO:
        drawText(GLYPHS_SHADOWS, ILI9341_WHITE, 110, 247, LIGHTS_TOP);
        drawText(GLYPHS_SHADOWS, ILI9341_WHITE, 110, 259, LIGHTS_BOTTOM_AUTO);
        break;
      case LIGHTS_ON_AUTO:
        drawText(GLYPHS_SHADOWS, ILI9341_GREEN, 110, 247, LIGHTS_TOP);
        drawText(GLYPHS_SHADOWS, ILI9341_GREEN, 110, 259, LIGHTS_BOTTOM_AUTO);
        break;
      case LIGHTS_ON_MANUAL:
        drawText(GLYPHS_SHADOWS, ILI9341_GREEN, 110, 247, LIGHTS_TOP);
        drawText(GLYPHS_SHADOWS, ILI9341_GREEN, 110, 259, LIGHTS_BOTTOM_MANUAL);
        break;
    }
  }
}

void updateBatteryHealthField() {
  byte batteryHealth = batteryMonitoringSystem->GetBatteryHealth();
  if (batteryHealthGlyphField->ValueHasChanged(batteryHealth) || displayUpdater->ForceUpdateNeeded()) {
    int glyphColor = ILI9341_GREEN;

    //sets color based on battery health
    if (batteryHealth != HEALTHY) {
      glyphColor = CUSTOM_COLOR_LIGHT_BROWN;
    }

    //clears old glyph and draws the top and bottom of the new glyph
    lcd.fillRect(148, 210, 36, 25, ILI9341_BLACK);
    drawText(GLYPHS, glyphColor, 150, 247, BATTERY_HEALTH_TOP);
    switch(batteryHealth) {
      case LOW_VOLTAGE:
        drawText(GLYPHS, glyphColor, 150, 259, BATTERY_HEALTH_BOTTOM_LOW);
        break;
      case HIGH_VOLTAGE:
        drawText(GLYPHS, glyphColor, 150, 259, BATTERY_HEALTH_BOTTOM_HIGH);
        break;
      case SERVICE_SOON:
        drawText(GLYPHS, glyphColor, 150, 259, BATTERY_HEALTH_BOTTOM_SERV);
        break;
      default:
        drawText(GLYPHS, glyphColor, 150, 259, BATTERY_HEALTH_BOTTOM_GOOD);
        break;
    }
  }
}

//Gauge drawing functions.
int gaugeLinesReposition(int y, bool rightSide) {
  int returnValue;

  //checks to see which segment the line falls on, and repositions the line as needed to match the curvature of the gauge
  if (y > 136) {
    returnValue = (12 - ((193 - y + 3) / 6));
  } else if (y < 131) {
    returnValue = (12 - ((y - 73 + 3) / 6));
  } else if (y <= 136 && y >= 131) {
    returnValue = 2;
  }

  //inverts the value if the line will appear on the right side of the screen
  if (rightSide == true) {
    returnValue = 320 - returnValue;
  }

  //returns the new x-coordinate
  return returnValue;
}

void updatePowerGaugeAndGlyph() {
  int PWRValue = batteryMonitoringSystem->GetPWRValue();
  int color = ILI9341_WHITE;

  //changes the color if power percentage reaches a certain level
  if (PWRValue >= 88) {
    color = CUSTOM_COLOR_SALMON;
  } else if (PWRValue >= 75) {
    color = CUSTOM_COLOR_LIGHT_BROWN;
  }

  //draws lines where necessary; draws 19 black lines on gauge to give appearance of 5% increments
  for (int y = 193; y > 73; y--) {
    if (((y - 73) % 6 != 0 && (193 - y) < (PWRValue * 1.2)) || y == 193) {
      lcd.drawLine(gaugeLinesReposition(y, false), y, gaugeLinesReposition(y, false) + 23, y, color);
    } else {
      lcd.drawLine(gaugeLinesReposition(y, false), y, gaugeLinesReposition(y, false) + 23, y, ILI9341_BLACK);
    }
  }

  if (powerGaugeGlyphColor->ValueHasChanged(color)) {
    drawText(GLYPHS_SHADOWS, color, 32, 144, POWER_GAUGE_LIGHTNING_BOLT);
  }
}

void updateBatteryGaugeAndGlyph() {
  int color = getBatteryInformationColor();

  //draws the lines required to fill the gauge; draws 7 black lines on gauge to give appearance of 1/8 increments
  for (int y = 193; y > 73; y--) {
    if (((y - 73) % 15 != 0 && (193 - y) < (batteryMonitoringSystem->GetStateOfCharge() * 1.2)) || y == 193) {
      lcd.drawLine(gaugeLinesReposition(y, true), y, gaugeLinesReposition(y, true) - 23, y, color);
    } else {
      lcd.drawLine(gaugeLinesReposition(y, true), y, gaugeLinesReposition(y, true) - 23, y, ILI9341_BLACK);
    }
  }

  if (batteryGaugeGlyphColor->ValueHasChanged(color)) {
    drawText(GLYPHS_SHADOWS, color, 270, 146, BATTERY_GAUGE_GLYPH);
  }
}


/*
Interrupt service routines.
*/
void changeScreenSelection() {
  detachInterrupt(digitalPinToInterrupt(MENU_CHANGE_BUTTON));
  displayUpdater->ChangeScreenSelection();
  drawScreenSpecificStaticElements();
  EEPROM.update(EEPROM_CURRENT_SCREEN_ADDRESS, displayUpdater->GetScreenSelection());
  attachInterrupt(digitalPinToInterrupt(MENU_CHANGE_BUTTON), changeScreenSelection, LOW);
}

void recordSpeedSensorHit() {
  speedSensor->RecordSpeedSensorHit();
}

void startNewSpeedometerInterval() {
  float metersTraveledDuringInterval = speedSensor->GetMetersTraveledDuringInterval();
  int millisecondsInInterval = speedometerIntervalTimer->GetTimeSince();
  speedometerIntervalTimer->ResetTimer();
  int speed = getCalculatedAndConvertedVelocity(metersTraveledDuringInterval, millisecondsInInterval);
  updateSpeedometerField(speed);
  incrementAllOdometers(metersTraveledDuringInterval);
  if (speedSensor->WriteOdometerToEEPROM()) {
    EEPROM.put(EEPROM_ODOMETER_ADDRESS, odometer->GetRawOdometerValue());
  }
  if (speedSensor->RunStoppedSequence()) {
    stoppedSequence();
  }
}


/*
Distance, speed, and range calculation related functions.
*/
void incrementAllOdometers(int meters) {
  odometer->Increment(meters);
  tripOdometer->Increment(meters);
  sinceStoppedOdometer->Increment(meters);
}

int getCalculatedAndConvertedVelocity(float metersTraveled, int milliseconds) {
  float metersPerHour = SECONDS_IN_AN_HOUR * (1000 / speedSensor->GetSpeedIntervalDurationMillis()) * metersTraveled;
  return (metersPerHour / METERS_CONVERSION);
}

void stoppedSequence() {
  float stateOfChargeAtBeginningOfSprint = batteryMonitoringSystem->GetStateOfCharge();  //a 'sprint' is defined as the time between the vehicle beginning to move and ceasing movement
  batteryMonitoringSystem->UpdatePeriodicVoltageValue();
  EEPROM.put(EEPROM_AVG_CHG_DIST_ADDRESS, batteryMonitoringSystem->RecordSprintDataAndGetNewAverageChargeDistance(sinceStoppedOdometer->GetRawOdometerValue(), (stateOfChargeAtBeginningOfSprint - batteryMonitoringSystem->GetStateOfCharge())));
  sinceStoppedOdometer = new Odometer();
}


/*
Calculates the outside temperature. 
*/
byte getOutsideTemperature() {
  return (analogRead(TEMPERATURE_SENSOR) * TEMPERATURE_CONVERSION_MULTIPLIER) + TEMPERATURE_FAHRENHEIT_OFFSET;
}


/*
Initializes the Arduino and all connected components.
*/
void setup() {
  //display initalization
  lcd.begin();
  lcd.setRotation(1);
  lcd.fillScreen(ILI9341_BLACK);

  //digital sensors and buttons initialization
  pinMode(HALL_SENSOR, INPUT_PULLUP);
  pinMode(HEADLIGHT_CONTROL_BUTTON, INPUT_PULLUP);
  pinMode(MENU_CHANGE_BUTTON, INPUT_PULLUP);

  //analog sensors initialization
  pinMode(BATTERY_SENSOR, INPUT);
  pinMode(PHOTOCELL, INPUT);
  pinMode(TEMPERATURE_SENSOR, INPUT);
  analogReference(DEFAULT);

  //draws elements that will remain the same for the entirety of the system's uptime
  drawStaticElements();
  drawScreenSpecificStaticElements();

  //interrupts
  attachInterrupt(digitalPinToInterrupt(HALL_SENSOR), recordSpeedSensorHit, RISING);
  attachInterrupt(digitalPinToInterrupt(MENU_CHANGE_BUTTON), changeScreenSelection, LOW);

  //odometer setup
  unsigned long lifetimeOdometer;
  EEPROM.get(EEPROM_ODOMETER_ADDRESS, lifetimeOdometer);
  odometer->Increment(lifetimeOdometer);

  //battery monitoring system is reinstantiated so that it is properly linked with battery sensor's analog pin
  unsigned int averageChargeDistance;
  EEPROM.get(EEPROM_AVG_CHG_DIST_ADDRESS, averageChargeDistance);
  batteryMonitoringSystem = new BatteryMonitoringSystem(averageChargeDistance, analogRead(BATTERY_SENSOR));

  //speed sensor object is reinstantiated so that it can read the proper values from the EEPROM
  float tireCircumfrenceInMeters;
  int magnetQuantity;
  int speedSensorIntervalDurationMillis;
  EEPROM.get(EEPROM_TIRE_CIRCUMFRENCE_ADDRESS, tireCircumfrenceInMeters);
  EEPROM.get(EEPROM_MANGET_QUANTITY_ADDRESS, magnetQuantity);
  EEPROM.get(EEPROM_SPEED_SENSOR_INTERVAL_MILLIS_ADDRESS, speedSensorIntervalDurationMillis);
  speedSensor = new SpeedSensor(tireCircumfrenceInMeters, magnetQuantity, speedSensorIntervalDurationMillis);
}


/*
Main function.
*/
void loop() {
  //detaches the MENU button's interrupt during screen drawing to maintain proper timing needed for display
  detachInterrupt(digitalPinToInterrupt(MENU_CHANGE_BUTTON));

  //updates system time and battery reading
  systemTime->UpdateSystemTime(millis());
  batteryMonitoringSystem->UpdateBatterySensorReading(analogRead(BATTERY_SENSOR));

  //checks surrounding brightness to see if lights need to be on or off every ten seconds
  if (automaticLightsCheckTimer->GetTimeSince() > 10000 && lights->AutomaticLightsAreEnabled()) {
    lights->AutomaticLightsCheck(analogRead(PHOTOCELL));
    automaticLightsCheckTimer->ResetTimer();
  }

  //aims to capture, report, and reset speed data around three times per second
  if (speedometerIntervalTimer->GetTimeSince() >= speedSensor->GetSpeedIntervalDurationMillis()) {
    startNewSpeedometerInterval();
  }

  //updates all of the values that are not screen specific (speedometer is included becuase screen-specificity is included within its function)
  if (temperatureFetchTimer->GetTimeSince() > 60000 || displayUpdater->ForceUpdateNeeded()) {
    updateTemperatureField();
    temperatureFetchTimer->ResetTimer();
  }
  updateRangeField();
  updateTripOdometerField();
  updateOdometerField();
  updatePowerGaugeAndGlyph();
  updateBatteryGaugeAndGlyph();
  // updateLightsStatusField();
  // updateBatteryHealthField();

  //draws screen specific values
  int screenSelection = displayUpdater->GetScreenSelection();
  switch(screenSelection) {
    case DIGITAL_GAUGES_SCREEN:
      updateBatteryGaugeNumericalField();
      updatePowerGaugeNumericalField();
      break;
    case TRIP_DATA_SCREEN:
      updateUptimeField();
      updateAverageSpeedField();
      updateChargeUsedField();
      break;
    case BATTERY_STATUS_SCREEN:
      updateBatteryVoltageField();
      updateAverageChargeDistanceField();
      break;
  }

  //disables force update
  displayUpdater->DisableForceUpdate();

  //reattaches MENU button's interrupt temporarily to capture any button presses
  attachInterrupt(digitalPinToInterrupt(MENU_CHANGE_BUTTON), changeScreenSelection, LOW);
}

# CartCockpit
An Arduino sketch that can turn an Arduino Uno/Nano into a digital instrument cluster for golf carts when paired with a few other components.

CartCockpit / Copyright (C) 2024 Nathan Strobl / nathanstrobl34@gmail.com
All of the software in this repository is covered under version 3.0 of the GNU General Public License; a copy of this license has been included in the root folder. 
This software may be copied, distributed, and/or modified without express permission of the author under the following conditions:
    - The copied, distributed, and/or modified version the software remains open-source, and
    - The copied and/or modified version of the software is distributed free of charge (i.e. users are not allowed to modify the software and then charge a price for others to use it).

If you have any questions or feedback about this software and its license, please reach out to me at the email address listed above.


ABOUT THIS SOFTWARE
====================
This software includes the following functionality:
- Speedometer & odometer
    Assuming a ring of magnets and a Hall Effect sensor is present on one of the wheels, the Arduino can display a speed reading and keep track of the total distance the cart has traveled; setting the tire size and other cart-specific variables can be done using the CartCockpitInit program. 
- Trip computer
    Measures the amount of time, distance traveled, battery charge used, and average speed of the cart since it was powered on; these values reset everytime power is cut from the Arduino. 
- Outdoor temperature indicator
    The Arduino can display a reading of the outdoor temperature if a TMP35 sensor is installed. 
- Battery monitoring system
    This system monitors the state of charge, health, and voltage of the cart's batteries; it can also inform the user when it is time to replace the cart's batteries. 
- Automatic lights controller
    Assuming a relay for the headlights and a photocell is installed, the Arduino can turn the cart's lights on and off automatically based on brightness of the surroundings; this feature can be overriden with the push of a button. 
- Estimated range indicator
    CartCockpit learns the driving patterns of the driver(s) and updates its range estimations accordingly; more rough driving will lower the estimated range, and smoother driving will increase it. The estimated range indicator also factors in the health of the battery when making its range estimations. 

Unfortunately, time constraints and my departure for school have prevented me from installing this in an actual golf cart, so most of this functionality remains untested; my hope with making this project public is that someone who is more passionate about electrical and/or mechanical engineering takes the initiative to implement, test, and improve upon this project since I am unable to do so at this time. 

<!-- A more in depth explanation of this project can be found [here](https://nathan-strobl.org/cartcockpit/overview). -->

REQUIRED LIBRARIES
====================
To run this software on an Arduino with an ILI9341, please make sure the following libraries are installed:
- Adafruit BusIO
- Adafruit GFX Library
- Adafruit ILI9341

PARTS LIST
====================
The following components are required to build this project, along with links to buy the required parts:
- Arduino Uno/Nano
- [3.2" ILI9341 LCD](https://www.amazon.com/gp/product/B0B1M9S9V6/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1)
- [5V to 3.3V 8-bit Logic Shifter](https://www.amazon.com/gp/product/B09R1QG957/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) (all data lines to LCD must be stepped down to 3.3V)
- [Hall effect sensor](https://www.amazon.com/gp/product/B07QS6PN3B/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
- [TMP36 temperature sensor](https://www.amazon.com/BOJACK-Temperature-Sensors-TMP36GZ-Precision/dp/B08BFY91ZW/ref=sr_1_1_sspa?crid=2FS63S5LD867Q&keywords=tmp35&qid=1707685772&s=industrial&sprefix=tmp35%2Cindustrial%2C122&sr=1-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1)
- [Photocell](https://www.amazon.com/eBoot-Photoresistor-Sensitive-Resistor-Dependent/dp/B01N7V536K/ref=sr_1_3?crid=1CYTGXLT83XGN&keywords=photocell+electronics&qid=1707685813&s=industrial&sprefix=photocell+electronic%2Cindustrial%2C81&sr=1-3) (for automatic light control)
- [Three-position switch](https://www.amazon.com/gp/product/B085L9HFW2/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) (for manual light control)
- [5V relay](https://www.amazon.com/Tolako-Arduino-Indicator-Channel-Official/dp/B00VRUAHLE/ref=sr_1_2_sspa?crid=Q408NAHBK67V&keywords=5v+relay&qid=1707685858&s=industrial&sprefix=5v+relay%2Cindustrial%2C93&sr=1-2-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1) (for Arduino to be able to control 12V lights)
- [Button](https://www.amazon.com/Gikfun-6x6x5mm-Switch-Button-Arduino/dp/B00R17XUFC/ref=sr_1_3?crid=IHS2W5JLVKLN&keywords=arduino+button&qid=1707685880&s=industrial&sprefix=arduino+button%2Cindustrial%2C84&sr=1-3)
- [60V to 5V USB converter](https://www.amazon.com/SSLHONG-Converter-Regulator-Waterproof-Transformer/dp/B0BJQ2PS3K/ref=sr_1_5?keywords=60v%2Bto%2B5v%2Busb&qid=1707685928&sr=8-5&th=1)

I will post a schematic for this project once I find the appropriate software to do so; until then, all connections for this project should be able to be found either within the source code or by Googling common connections for each individual part. 

PHOTOS
====================
CartCockpit can be seen in action by viewing the files in the /Media directory.

<!-- or by viewing the Gallery section [on this page](https://nathan-strobl.org/cartcockpit/overview). -->
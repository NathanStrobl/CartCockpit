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

PHOTOS
====================
<video style="max-height:600px" autoplay loop muted controls>
	<source src="/Photos/cartcockpit.mp4" type="video/mp4" />
</video>
# ESP 32 Clock and Weather station
publiched by Aleksey Evgrashin
## Pictures

Clock
![Weather Clock Board](https://github.com/Evleksey/Weather-Clock-ESP32/blob/main/images/photo1.jpg)

Weather report
![Weather Clock Board](https://github.com/Evleksey/Weather-Clock-ESP32/blob/main/images/photo2.jpg)

**Note:**  
This sketch uses OpenWeatherMap API. In oreder to use you need to set up a free account on https://openweathermap.org/.

In OLED_I2C library folder relpase '<DefaultFonts.c>' file with one in his repository to get custom DSEG_Weather and DSEG_Modern fonts used in the scetch.

## Compiling Source
(**0.** Download and install [Arduino](https://www.arduino.cc/en/Main/Software))  
**1.** Install the Arduino core for the ESP32: https://github.com/espressif/arduino-esp32#installation-instructions  
**2.** Install this ESP8266/ESP32 OLED library: https://github.com/ThingPulse/esp8266-oled-ssd1306
**3.** Install this Dictionary Library: https://github.com/arkhipenko/Dictionary
**4.** Istall Arduino Json Library using Library Manager reference: https://arduinojson.org/
**5.** In OLED_I2C library folder relpase <DefaultFonts.c> file with one in his repository to get custom DSEG_Weather and DSEG_Modern fonts used in the scetch.
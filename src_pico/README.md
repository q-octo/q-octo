## Notes to self

How to compile...

Speed is in RPM

https://www.instructables.com/Using-RP2040-PIO-in-Arduino-IDE-on-Windows/


`git submodule update --init`


## Windows Footguns

* https://arduino-pico.readthedocs.io/en/latest/platformio.html#current-state-of-development
* https://github.com/earlephilhower/arduino-pico?tab=readme-ov-file#windows-specific-notes



## FreeRTOS

https://arduino-pico.readthedocs.io/en/latest/freertos.html

Priority levels range from 0 to 7.

`delay()` and `yield()` free the CPU for other tasks, while delayMicroseconds() does not.


## Web Server

Example code:
* https://github.com/Links2004/arduinoWebSockets/tree/master/examples/esp8266_pico
* https://github.com/earlephilhower/arduino-pico/tree/master/libraries/WebServer/examples

## Configurations

Include `config.h` to use any of the `CFG_<config name>` properties

## Licensing and Credits
#ifndef PLATFORMIOESP8266ENVIRONMENT_LOGGERINTERFACE_H
#define PLATFORMIOESP8266ENVIRONMENT_LOGGERINTERFACE_H

#if defined(Arduino_h)
#include <Arduino.h>
#elif defined(RASPBERRY_PI)
#include <RasPi.h>
#include <iostream>
#elif defined(RH_PLATFORM)
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#else
#error "could not determine target environment"
#endif

#include <stdint.h>


class LoggerInterface {
private:
    uint8_t current_log_lvl = 2;
    uint8_t last_started_log_lvl = UINT8_MAX;

public:

     bool begin();

     void set_log_lvl(uint8_t log_lvl);

     void log(char *msg, uint8_t log_lvl);

     void log(const char *msg, uint8_t log_lvl);

     void start_log(char *msg, uint8_t log_lvl);

     void start_log(const char *msg, uint8_t log_lvl);

     void set_current_log_lvl(uint8_t log_lvl);

     void append_log(char *msg);

     void append_log(uint8_t *msg);

     void append_log(const char *msg);

};


#endif //PLATFORMIOESP8266ENVIRONMENT_LOGGERINTERFACE_H

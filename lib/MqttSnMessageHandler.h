//
// Created by bele on 09.12.16.
//

#ifndef DUMMY_MQTTSNMESSAGEHANDLER_H
#define DUMMY_MQTTSNMESSAGEHANDLER_H

#if defined(Arduino_h)
#include <Arduino.h>
#elif defined(RASPBERRY_PI)
#include <RasPi.h>
#elif defined(RH_PLATFORM)
#include <wiringPi.h>
#else
#error "could not determine target environment"
#endif

#include <stdint.h>
#include "mqttsn_messages.h"
#include "SocketInterface.h"
#include "LoggerInterface.h"

class SocketInterface;


class MqttSnMessageHandler {

public:

    virtual bool begin();

    void setSocket(SocketInterface *socket);

    void setLogger(LoggerInterface *logger);

    virtual bool send(device_address *destination, uint8_t *bytes, uint16_t bytes_len);

    virtual void receiveData(device_address *address, uint8_t *bytes);

    virtual bool loop();

    void printReceived(device_address *address, uint8_t *bytes);

    void printDeviceAddress(device_address* address);

    void printBuffer(uint8_t* bytes);

public:
    SocketInterface* socket = nullptr;
    LoggerInterface *logger = nullptr;

};


#endif //DUMMY_MQTTSNMESSAGEHANDLER_H

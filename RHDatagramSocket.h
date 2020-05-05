/*
* The MIT License (MIT)
*
* Copyright (C) 2018 Gabriel Nikol
* Refactored for use in Gateway 2020 by Joel Kozikowski
*/

#ifndef RADIOHEADSOCKET__RHDATAGRAMSOCKET_H
#define RADIOHEADSOCKET__RHDATAGRAMSOCKET_H

#include <RHDatagram.h>

#include "MqttSnMessageHandler.h"
#include "SocketInterface.h"

/**
 * A socket compatible with RadioHead's RHDatagram manager. Note that unless
 * you have pre-existing code, you should use RHReliableDatagramSocket instead
 * as it is also compatible with normal RHDatagram's on the other end, as it can
 * have "reliable" mode turned on or off.
 */
class RHDatagramSocket : public SocketInterface {

public:
    bool begin() override;

    void setManager(RHDatagram* manager);

    void setLogger(LoggerInterface *logger) override;

    device_address *getBroadcastAddress() override;

    device_address *getAddress() override;

    uint8_t getMaximumMessageLength() override;

    bool send(device_address *destination, uint8_t *bytes, uint16_t bytes_len);

    bool send(device_address *destination, uint8_t *bytes, uint16_t bytes_len, uint8_t signal_strength);

    bool loop() override;

    void setMqttSnMessageHandler(MqttSnMessageHandler *mqttSnMessageHandler) override;

    void setConnectionTimeout(uint16_t timeoutInMillis) { this-> msConnectTimeout = timeoutInMillis; }

protected:
    RHDatagram* manager;

    device_address own_address;
    device_address broadcast_address;

    LoggerInterface *logger = nullptr;

    MqttSnMessageHandler *mqttSnMessageHandler = nullptr;

    uint16_t msConnectTimeout = 30000;

};

#endif // RADIOHEADSOCKET__RHDATAGRAMSOCKET_H

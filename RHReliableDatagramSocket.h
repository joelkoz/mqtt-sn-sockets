/*
* The MIT License (MIT)
*
* Copyright (C) 2018 Gabriel Nikol
* Refactored for use in Gateway 2020 by Joel Kozikowski
*/

#ifndef RADIOHEADSOCKET__RHRELIABLEDATAGRAMSOCKET_H
#define RADIOHEADSOCKET__RHDRELIABLEATAGRAMSOCKET_H

#include <RHReliableDatagram.h>

#include "MqttSnMessageHandler.h"
#include "SocketInterface.h"

/**
 * A socket compatible with RadioHead's RHDatagram and RHReliableDatagram
 * managers on the other end.  An RHReliableDatagram manager is necessary
 * to be assigned to this class via setManager(), Note that the
 * "reliable" protocol used by RHReliableDatagram can be disabled by
 * calling setReliableProtocol().
 */
class RHReliableDatagramSocket : public SocketInterface {

public:
    bool begin() override;

    void setManager(RHReliableDatagram* manager);

    void setLogger(LoggerInterface *logger) override;

    device_address *getBroadcastAddress() override;

    device_address *getAddress() override;

    uint8_t getMaximumMessageLength() override;

    bool send(device_address *destination, uint8_t *bytes, uint16_t bytes_len);

    bool send(device_address *destination, uint8_t *bytes, uint16_t bytes_len, uint8_t signal_strength) override;

    virtual bool receive(device_address *source, uint8_t *bytes, uint16_t bytes_len);

    bool loop() override;

    void setMqttSnMessageHandler(MqttSnMessageHandler *mqttSnMessageHandler) override;

    // Set the timeout this socket will wait for an initial connection
    void setConnectionTimeout(uint16_t timeoutInMillis) { this-> msConnectTimeout = timeoutInMillis; }

    /**
     * Sets whether or not to use RadioHead's "reliable" acknowledged protocol, or its 
     * normal one.  The default is "not reliable"
     */
    void setReliableProtocol(bool isReliable);

    bool isReliable() { return reliable; }

protected:
    RHReliableDatagram* manager;

    device_address own_address;
    device_address broadcast_address;

    LoggerInterface *logger = nullptr;

    MqttSnMessageHandler *mqttSnMessageHandler = nullptr;

    uint16_t msConnectTimeout = 10000;

    bool reliable = true;

};

#endif // RADIOHEADSOCKET__RHDATAGRAMSOCKET_H

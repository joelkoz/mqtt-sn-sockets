#ifndef RADIOHEADSOCKET_LORASOCKET_H
#define RADIOHEADSOCKET_LORASOCKET_H


#include <SocketInterface.h>
#include <RHReliableDatagram.h>
#include <RH_LoRa.h>


class LoRaSocket : public SocketInterface{
public:
    bool begin() override;

    void setManager(RHReliableDatagram *manager);

    void setLogger(LoggerInterface *logger) override;

    void setMqttSnMessageHandler(MqttSnMessageHandler *mqttSnMessageHandler) override;

    device_address *getBroadcastAddress() override;

    device_address *getAddress() override;

    uint8_t getMaximumMessageLength() override;

    bool send(device_address *destination, uint8_t *bytes, uint16_t bytes_len) override;

    bool send(device_address *destination, uint8_t *bytes, uint16_t bytes_len, uint8_t signal_strength) override;

    bool loop() override;

private:
    RHReliableDatagram* manager;

    device_address broadcastAddress;
    device_address ownAddress;

    LoggerInterface *logger;
    MqttSnMessageHandler *mqttSnMessageHandler;
};


#endif

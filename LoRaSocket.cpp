#include "LoRaSocket.h"

bool LoRaSocket::begin() {

    if (this->logger == nullptr) {
        return false;
    }
    if (this->mqttSnMessageHandler == nullptr) {
        return false;
    }

    if (this->manager == nullptr) {
        return false;
    }

    this->broadcastAddress = device_address(RH_BROADCAST_ADDRESS, 0, 0, 0, 0, 0);
    this->ownAddress = device_address(manager->thisAddress(), 0, 0, 0, 0, 0);

    if(!manager->init()){
        return false;
      }
    uint8_t dummy_data[1] = {0};
    if(!manager->sendto((uint8_t*) &dummy_data, sizeof(dummy_data), 0)){
        return false;
      }
    manager->waitPacketSent();
    manager->available(); // put it back to receive mode
#if defined(GATEWAY_TRANSMISSION_PROTOCOL_RASPBERRY_RH_NRF24)
    this->mqttSnMessageHandler->notify_socket_connected();
#endif
#if defined(GATEWAY_TRANSMISSION_PROTOCOL_RASPBERRY_RH_RF95)
    this->mqttSnMessageHandler->notify_socket_connected();
#endif
#if defined(GATEWAY_TRANSMISSION_PROTOCOL_RASPBERRY_RH_LORA)
    this->mqttSnMessageHandler->notify_socket_connected();
#endif
    return true;
}


void LoRaSocket::setManager(RHReliableDatagram *manager) {
    LoRaSocket::manager = manager;
}


void LoRaSocket::setLogger(LoggerInterface *logger) {
    LoRaSocket::logger = logger;
}

void LoRaSocket::setMqttSnMessageHandler(MqttSnMessageHandler *mqttSnMessageHandler) {
    LoRaSocket::mqttSnMessageHandler = mqttSnMessageHandler;
}

device_address *LoRaSocket::getBroadcastAddress() {
    return &this->broadcastAddress;
}

device_address *LoRaSocket::getAddress() {
    return &this->ownAddress;
}

uint8_t LoRaSocket::getMaximumMessageLength() {
    return RH_LORA_MAX_MESSAGE_LEN;
}

bool LoRaSocket::send(device_address *destination, uint8_t *bytes, uint16_t bytes_len) {
    return send(destination, bytes, bytes_len, UINT8_MAX);
}

bool LoRaSocket::send(device_address *destination, uint8_t *bytes, uint16_t bytes_len, uint8_t signal_strength) {
    manager->sendto(bytes, bytes_len, destination->bytes[0]);
    manager->waitPacketSent();
    manager->available(); // put it back to receive mode
    return true;
}

bool LoRaSocket::loop() {
    uint8_t buf_len = RH_LORA_MAX_MESSAGE_LEN;
    uint8_t buf[RH_LORA_MAX_MESSAGE_LEN];
    memset(&buf, 0x0, RH_LORA_MAX_MESSAGE_LEN);
    device_address from;
    memset(&from.bytes[0], 0x0, sizeof(device_address));
    if (manager->available()) {
        if (manager->recvfrom(buf, &buf_len, &from.bytes[0])) {
            mqttSnMessageHandler->receiveData(&from, (uint8_t * ) & buf);
        }
    }
    return true;
}


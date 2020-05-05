#include "RHReliableDatagramSocket.h"


bool RHReliableDatagramSocket::begin() {

    if (this->logger == nullptr) {
        return false;
    }

    if (this->mqttSnMessageHandler == nullptr) {
        logger->log("No mqttSnMessageHandler defined", 0);
        return false;
    }

    if (this->manager == nullptr) {
        logger->log("No RHDatagram manager defined", 0);
        return false;
    }

    own_address = device_address(manager->thisAddress(), 0, 0, 0, 0, 0);
    broadcast_address = device_address(RH_BROADCAST_ADDRESS, 0, 0, 0, 0, 0);

    if (!manager->init()) {
        return false;
    }

    logger->log("MQTT-SN incomming datagram socket waiting for initial connection...", 2);
    while (true) {
        if (manager->waitAvailableTimeout(this->msConnectTimeout)) {
            this->mqttSnMessageHandler->notify_socket_connected();
            return true;
        }
        else {
            logger->log("No client connection - wait timed out. Trying again...", 1);
        }
    }
}


void RHReliableDatagramSocket::setManager(RHReliableDatagram* manager) {
    this->manager = manager;
}


void RHReliableDatagramSocket::setLogger(LoggerInterface *logger) {
    this->logger = logger;
}


void RHReliableDatagramSocket::setMqttSnMessageHandler(MqttSnMessageHandler *mqttSnMessageHandler) {
    this->mqttSnMessageHandler = mqttSnMessageHandler;
}


device_address *RHReliableDatagramSocket::getBroadcastAddress() {
    return &broadcast_address;
}


device_address *RHReliableDatagramSocket::getAddress() {
    return &own_address;
}


uint8_t RHReliableDatagramSocket::getMaximumMessageLength() {
    return RH_MAX_MESSAGE_LEN;
}


bool RHReliableDatagramSocket::send(device_address *destination, uint8_t *bytes, uint16_t bytes_len) {
    return send(destination, bytes, bytes_len, UINT8_MAX);
}


void RHReliableDatagramSocket::setReliableProtocol(bool isReliable) {
    this->reliable = isReliable;
}


bool RHReliableDatagramSocket::send(device_address *destination, uint8_t *bytes, uint16_t bytes_len, uint8_t signal_strength) {
    bool sendOk;

    if (reliable) {
       sendOk = manager->sendtoWait(bytes, bytes_len, destination->bytes[0]);
    }
    else {
       sendOk = manager->sendto(bytes, bytes_len, destination->bytes[0]);
    }
    manager->waitPacketSent();
    manager->available(); // put it back to receive mode
    return sendOk;
}



bool RHReliableDatagramSocket::receive(device_address *source, uint8_t *bytes, uint16_t bytes_len) {
   uint8_t len = bytes_len;
   if (reliable) {
      return manager->recvfromAck(bytes, &len, &(source->bytes[0]));
   }
   else {
      return manager->recvfrom(bytes, &len, &(source->bytes[0]));
   }
}


bool RHReliableDatagramSocket::loop() {

    if (manager->available()) {

        device_address receive_address;
        memset(&receive_address, 0x0, sizeof(device_address));

        uint8_t receive_buffer[RH_MAX_MESSAGE_LEN];
        memset(&receive_buffer, 0x0, RH_MAX_MESSAGE_LEN);

        uint8_t receive_buffer_len = sizeof(receive_buffer);

        if (receive(&receive_address, receive_buffer, receive_buffer_len)) {

            if (mqttSnMessageHandler != nullptr) {
                mqttSnMessageHandler->receiveData(&receive_address, receive_buffer);
            }

            // if (transmissionProtocolUartBridge != nullptr) {
            //     transmissionProtocolUartBridge->receiveData(&receive_address, receive_buffer, receive_buffer_len);
            // }

        }
    }

    return true;
}

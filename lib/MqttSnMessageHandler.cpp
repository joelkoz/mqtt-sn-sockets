#include "MqttSnMessageHandler.h"

#ifndef Arduino_h

#include <string>

#endif

bool MqttSnMessageHandler::begin() {
    if(socket == nullptr){
        return false;
    }
    if(logger == nullptr){
        return false;
    }
    if(!logger->begin()){
        return false;
    }
    return this->socket->begin();
}

void MqttSnMessageHandler::setSocket(SocketInterface *socket) {
    this->socket = socket;
}


void MqttSnMessageHandler::setLogger(LoggerInterface *logger) {
    this->logger = logger;
}


bool MqttSnMessageHandler::send(device_address *destination, uint8_t *bytes, uint16_t bytes_len){
    //delay(2000);
    return socket->send(destination, bytes, bytes_len);
}

void MqttSnMessageHandler::receiveData(device_address *address, uint8_t *bytes) {
    message_header *header = (message_header *) bytes;
    if (header->length < 2) {
        //Serial.print("header->length < 2 | ");
        logger->start_log("header->length < 2 | ", 1);
        printReceived(address, bytes);
        socket->send(address, bytes, bytes[0]);
        return;
    }
    switch (header->type) {
        case MQTTSN_PINGREQ:
            //Serial.print("MQTTSN_PINGREQ");
            break;
        case MQTTSN_SEARCHGW:
            //Serial.print("MQTTSN_SEARCHGW");
            break;
        case MQTTSN_CONNECT:
            //Serial.print("MQTTSN_CONNECT");
            break;
        case MQTTSN_WILLTOPIC:
            //Serial.print("MQTTSN_WILLTOPIC");
            break;
        case MQTTSN_WILLMSG:
            //Serial.print("MQTTSN_WILLMSG");
            break;
        case MQTTSN_REGISTER:
            //Serial.print("MQTTSN_REGISTER");
            break;
        case MQTTSN_REGACK:
            //Serial.print("MQTTSN_REGACK");
            break;
        case MQTTSN_PUBLISH:
            //Serial.print("MQTTSN_PUBLISH");
            break;
        case MQTTSN_PUBACK:
            //Serial.print("MQTTSN_PUBACK");
            break;
        case MQTTSN_SUBSCRIBE:
            //Serial.print("MQTTSN_SUBSCRIBE");
            break;
        case MQTTSN_UNSUBSCRIBE:
            //Serial.print("MQTTSN_UNSUBSCRIBE");
            break;
        case MQTTSN_DISCONNECT:
            //Serial.print("MQTTSN_DISCONNECT");
        case MQTTSN_WILLTOPICUPD:
            //Serial.print("MQTTSN_WILLTOPICUPD");
            break;
        case MQTTSN_WILLMSGUPD:
            //Serial.print("MQTTSN_WILLMSGUPD");
            break;
        case MQTTSN_FORWARD_ENCAPLSULATION:
            //Serial.print("MQTTSN_FORWARD_ENCAPLSULATION");
            break;
        case MQTTSN_ADVERTISE:
            //Serial.print("MQTTSN_ADVERTISE");
            break;
        default:
            //Serial.print("UNKNOWN");
            break;
    }
    //Serial.print(" | ");
    logger->start_log(" | ", 1);
    printReceived(address, bytes);
    send(address, bytes, bytes[0]);
}

void MqttSnMessageHandler::printReceived(device_address *address, uint8_t *bytes) {
    printDeviceAddress(address);
    //Serial.print(" | ");
    //Serial.print(" bytes_len ");
    //Serial.print(" | ");
    logger->append_log(" | ");
    logger->append_log(" bytes_len ");
    logger->append_log(" | ");
#ifndef Arduino_h
    std::string number_str = std::to_string(bytes[0]);
    logger->append_log(number_str.c_str());
    //Serial.print(number_str.c_str());
        if (bytes[1] == MQTTSN_SEARCHGW) {
        logger->append_log(" radius ");
        //Serial.print(" radius: ");
        number_str = std::to_string(bytes[2]);
        logger->append_log(number_str.c_str());	
        //Serial.print(number_str.c_str());
    }
#else
    Serial.print(bytes[0]);
    if (bytes[1] == MQTTSN_SEARCHGW) {
        Serial.print(" radius: ");
        Serial.print(bytes[2]);
    }
#endif
    logger->append_log(" | ");
    //Serial.print(" | ");
    printBuffer(bytes);
    //Serial.println("");
}


void MqttSnMessageHandler::printDeviceAddress(device_address *address) {
    //Serial.print("from { ");
    logger->append_log("from { ");
    for (uint8_t i = 0; i < sizeof(device_address); i++) {
#ifndef Arduino_h
        std::string number_str = std::to_string(address->bytes[i]);
        //Serial.print(number_str.c_str());
        logger->append_log(number_str.c_str());
#else
        //Serial.print(address->bytes[i]);
        logger->append_log(address->bytes[i]);
#endif
        if (i == sizeof(device_address) - 1) {
            //Serial.print(" }");
            logger->append_log(" } ");
        } else {
            //Serial.print(", ");
            logger->append_log(", ");
        }
    }
}

void MqttSnMessageHandler::printBuffer(uint8_t *bytes) {
    //Serial.print("bytes { ");
    logger->append_log("bytes { ");
    for (uint8_t i = 0; i < bytes[0]; i++) {
#ifndef Arduino_h
        if(i==0){
            std::string number_str = std::to_string(bytes[i]);
            //Serial.print(number_str.c_str());
            logger->append_log(number_str.c_str());
        } else {
            //Serial.print(bytes[i]);
            logger->append_log(&bytes[i]);
        }
#else
        Serial.print(bytes[i]);
#endif
        if (i == bytes[0] - 1) {
            //Serial.print(" }");
            logger->append_log(" } ");
        } else {
            //Serial.print(", ");
            logger->append_log(", ");
        }
    }
}

bool MqttSnMessageHandler::loop() {
    return socket->loop();
}

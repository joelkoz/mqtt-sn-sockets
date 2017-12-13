//
// Created by bele on 13.12.17.
//

#include "SimpleMqttSnClientTester.h"

void SimpleMqttSnClientTester::receiveData(device_address *address, uint8_t *bytes) {
    logger->log("SimpleMqttSnClientTester::receiveData", 1);
    message_header *header = (message_header *) bytes;
    if (header->length < 2) {
        return;
    }

    switch (header->type) {

        case MQTTSN_PINGREQ:
            Serial.print("MQTTSN_PINGREQ");
            parse_pingreq(address, bytes);
            break;
        case MQTTSN_PINGRESP:
            Serial.print("MQTTSN_PINGRESP");
            parse_pingresp(address, bytes);
            break;
        case MQTTSN_SEARCHGW:
            Serial.print("MQTTSN_SEARCHGW");
            parse_searchgw(address, bytes);
            break;
        default:
            break;
    }
    Serial.print(" | ");
    printReceived(address, bytes);
}

bool SimpleMqttSnClientTester::send(device_address *destination, uint8_t *bytes, uint16_t bytes_len) {
    logger->log("SimpleMqttSnClientTester::send", 1);
    return socket->send(destination, bytes, bytes_len);
}

void SimpleMqttSnClientTester::parse_pingreq(device_address *address, uint8_t *bytes) {
    msg_pingreq *msg = (msg_pingreq *) bytes;
    if (msg->length == 2 && msg->type == MQTTSN_PINGREQ) {
        handle_pingreq(address);
    }
}

void SimpleMqttSnClientTester::handle_pingreq(device_address *source) {
    send_ping(source);
}

void SimpleMqttSnClientTester::send_ping(device_address *destination) {
    message_header msg;
    msg.to_pingresp();
    socket->send(destination, (uint8_t *) &msg, msg.length);
}

void SimpleMqttSnClientTester::parse_pingresp(device_address *address, uint8_t *bytes) {
    message_header *msg = (message_header *) bytes;
    if (msg->length == 2 && msg->type == MQTTSN_PINGRESP) {
        handle_pingresp(address);
    }
}

void SimpleMqttSnClientTester::handle_pingresp(device_address *source) {
    // nothing to do
}

void SimpleMqttSnClientTester::parse_searchgw(device_address *address, uint8_t *bytes) {
    msg_searchgw *msg = (msg_searchgw *) bytes;
    if (msg->length == 3 && msg->type == MQTTSN_SEARCHGW) {
        handle_searchgw(address, msg->radius);
    }
}

void SimpleMqttSnClientTester::handle_searchgw(device_address *source, uint8_t radius) {
    Serial.print(" radius: ");
    Serial.print(radius);
    send_gwinfo(source, radius);
}

void SimpleMqttSnClientTester::send_gwinfo(device_address *source, uint8_t radius) {
    msg_gwinfo msg(GW_ID, (uint8_t *) socket->getAddress());
    msg.length = 3;
    msg.type = MQTTSN_GWINFO;
    socket->send(source, (uint8_t *) &msg, radius);
}

void SimpleMqttSnClientTester::send_advertise(device_address *source, uint8_t gw_id, uint16_t duration) {
    msg_advertise msg(gw_id, duration, socket->getAddress());
    msg.length = 5;
    msg.type = MQTTSN_GWINFO;
    msg.gw_id = gw_id;
    msg.duration = duration;

    socket->send(socket->getBroadcastAddress(), (uint8_t *) &msg, msg.length);
}

bool SimpleMqttSnClientTester::loop() {
    uint64_t current_millis = (uint64_t) millis();
    if(current_millis - last_advertisment_send_millis > (GW_DURATION - 5) * 1000) {
        last_advertisment_send_millis = (uint64_t) millis();
        send_advertise(socket->getBroadcastAddress(), GW_ID, GW_DURATION);
    }
    return MqttSnMessageHandler::loop();
}

bool SimpleMqttSnClientTester::begin() {
    last_advertisment_send_millis = (uint64_t) millis();
    return MqttSnMessageHandler::begin();
}



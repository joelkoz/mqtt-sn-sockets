//
// Created by bele on 13.12.17.
//

#include "SimpleMqttSnClientTester.h"

void SimpleMqttSnClientTester::receiveData(device_address *address, uint8_t *bytes) {
    logger->log("receiveData | ", 1);
    message_header *header = (message_header *) bytes;
    if (header->length < 2) {
        return;
    }

    switch (header->type) {

        case MQTTSN_PINGREQ:
            Serial.print("PINGREQ ");
            printReceived(address, bytes);
            parse_pingreq(address, bytes);
            break;
        case MQTTSN_PINGRESP:
            Serial.print("PINGRESP ");
            printReceived(address, bytes);
            parse_pingresp(address, bytes);
            break;
        case MQTTSN_SEARCHGW:
            Serial.print("SEARCHGW ");
            printReceived(address, bytes);
            parse_searchgw(address, bytes);
            break;
        case MQTTSN_PUBLISH:
            Serial.print("PUBLISH ");
            printReceived(address, bytes);
            parse_publish(address, bytes);
            break;
        default:
            Serial.print("UNKOWN ");
            printReceived(address, bytes);
            break;
    }

}

bool SimpleMqttSnClientTester::send(device_address *destination, uint8_t *bytes, uint16_t bytes_len) {
    Serial.print("send");
    Serial.print(" | ");
    printDeviceAddress(destination);
    Serial.print(" | ");
    printBuffer(bytes);
    Serial.println();

    return socket->send(destination, bytes, bytes_len);
}

void SimpleMqttSnClientTester::parse_pingreq(device_address *address, uint8_t *bytes) {
    msg_pingreq *msg = (msg_pingreq *) bytes;
    if (msg->length == 2 && msg->type == MQTTSN_PINGREQ) {
        handle_pingreq(address);
    }
}

void SimpleMqttSnClientTester::handle_pingreq(device_address *source) {
    send_pingresp(source);
}

void SimpleMqttSnClientTester::send_pingresp(device_address *destination) {
    message_header msg;
    msg.to_pingresp();
    this->send(destination, (uint8_t *) &msg, msg.length);
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

void SimpleMqttSnClientTester::send_pingreq(device_address *destination) {
    msg_pingreq msg;
    msg.length = 2;
    msg.type = MQTTSN_PINGREQ;
    this->send(destination, (uint8_t *) &msg, msg.length);
}

void SimpleMqttSnClientTester::parse_searchgw(device_address *address, uint8_t *bytes) {
    msg_searchgw *msg = (msg_searchgw *) bytes;
    if (msg->length == 3 && msg->type == MQTTSN_SEARCHGW) {
        handle_searchgw(address, msg->radius);
    }
}

void SimpleMqttSnClientTester::handle_searchgw(device_address *source, uint8_t radius) {
    send_gwinfo(source, radius);
}

void SimpleMqttSnClientTester::send_gwinfo(device_address *source, uint8_t radius) {
    msg_gwinfo msg(GW_ID, (uint8_t *) socket->getAddress());
    msg.length = 3;
    msg.type = MQTTSN_GWINFO;
    msg.gw_id = GW_ID;
    this->send(socket->getBroadcastAddress(), (uint8_t *) &msg, msg.length);

}

void SimpleMqttSnClientTester::parse_publish(device_address *source, uint8_t *bytes) {
    msg_publish *msg = (msg_publish *) bytes;
    if (bytes[0] > 7 && bytes[1] == MQTTSN_PUBLISH) { // 7 bytes header + at least 1 byte data
        bool dup = (msg->flags & FLAG_DUP) != 0;

        int8_t qos = 0;
        if ((msg->flags & FLAG_QOS_M1) == FLAG_QOS_M1) {
            qos = -1;
        } else if ((msg->flags & FLAG_QOS_2) == FLAG_QOS_2) {
            qos = 2;
        } else if ((msg->flags & FLAG_QOS_1) == FLAG_QOS_1) {
            qos = 1;
        } else {
            qos = 0;
        }

        bool retain = (msg->flags & FLAG_RETAIN) != 0;
        bool short_topic = (msg->flags & FLAG_TOPIC_SHORT_NAME) != 0;
        uint16_t data_len = bytes[0] - (uint8_t) 7;
        if (((qos == 0) || (qos == -1)) && msg->message_id != 0x0000) {
            // this can be too strict
            // we can also ignore the message_id for Qos 0 and -1
            return;
        }

        if (!short_topic && !(msg->flags & FLAG_TOPIC_PREDEFINED_ID != 0)) {
            handle_publish(source, msg->data, data_len, msg->message_id, msg->topic_id, short_topic, retain, qos, dup);
        }
        handle_publish(source, msg->data, data_len, msg->message_id, msg->topic_id, short_topic, retain, qos, dup);
    }

}

void SimpleMqttSnClientTester::handle_publish(device_address *source, uint8_t *data, uint16_t data_length,
                                              uint16_t message_id, uint16_t topic_id, bool short_topic, bool retain,
                                              int8_t qos, bool dup) {
    Serial.print("Publish: message_id: ");
    Serial.print(message_id);

    Serial.print(" topic: ");
    if (short_topic) {
        Serial.print("short");
    }else{
        Serial.print("predefined");
    }

    Serial.print(" id: ");
    Serial.print(topic_id);

    Serial.print(" retain: ");
    if(retain){
        Serial.print(dup);
    }else{
        Serial.print(dup);
    }

    Serial.print(" dup: ");
    if(dup){
        Serial.print(dup);
    }else{
        Serial.print(dup);
    }

    Serial.print(" qos: ");
    Serial.print(qos);

    Serial.print(" data: ");
    Serial.print("[ ");
    for (uint8_t i = 0; i < data_length; i++) {
        Serial.print(data[i]);
        if (i + 1 < data_length) {
            Serial.print(", ");
        }else{
            Serial.print(" ] ");
        }
    }
    Serial.println();

}


void SimpleMqttSnClientTester::send_advertise(device_address *source, uint8_t gw_id, uint16_t duration) {
    msg_advertise msg(gw_id, duration, socket->getAddress());
    msg.length = 5;
    msg.type = MQTTSN_ADVERTISE;
    msg.gw_id = gw_id;
    msg.duration = duration;

    this->send(socket->getBroadcastAddress(), (uint8_t *) &msg, msg.length);
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





//
// Created by bele on 13.12.17.
//

#ifndef ARDUINOSOCKETS_SIMPLEMQTTSNCLIENTTESTER_H
#define ARDUINOSOCKETS_SIMPLEMQTTSNCLIENTTESTER_H


#include "MqttSnMessageHandler.h"

#define GW_ID 0x02
#define GW_DURATION 30


class SimpleMqttSnClientTester : public MqttSnMessageHandler {
public:
    bool begin() override;

    bool send(device_address *destination, uint8_t *bytes, uint16_t bytes_len) override;

    void receiveData(device_address *address, uint8_t *bytes) override;

    void parse_pingreq(device_address *address, uint8_t *bytes);

    void send_pingresp(device_address *destination);

    void send_pingreq(device_address *destination);

    void parse_pingresp(device_address *address, uint8_t *bytes);

    void handle_pingresp(device_address *source);

    void handle_pingreq(device_address *source);

    void parse_searchgw(device_address *address, uint8_t *bytes);

    void handle_searchgw(device_address *source, uint8_t radius);

    void send_gwinfo(device_address *source, uint8_t radius);

    void send_advertise(device_address* source,uint8_t gw_id, uint16_t duration);

    bool loop() override;

private:
    uint64_t last_advertisment_send_millis;
};


#endif //ARDUINOSOCKETS_SIMPLEMQTTSNCLIENTTESTER_H

#include "LoggerInterface.h"

bool LoggerInterface::begin() {
    return true;
}

void LoggerInterface::set_log_lvl(uint8_t log_lvl) {
    this->current_log_lvl = log_lvl;
}

void LoggerInterface::log(char *msg, uint8_t log_lvl) {
    log((const char *) msg, log_lvl);
}

void LoggerInterface::log(const char *msg, uint8_t log_lvl) {
    if (log_lvl > current_log_lvl) {
        return;
    }
    char millis_buffer[26];
    sprintf(millis_buffer, "%ld", millis());
#if defined(Arduino_h)
    Serial.print(millis_buffer);
    Serial.print(": ");
    Serial.print(msg);
    Serial.println();
#else
    std::cout << millis_buffer << ": " << msg << std::endl << std::flush;
#endif

}

void LoggerInterface::start_log(char *msg, uint8_t log_lvl) {
    start_log((const char *) msg, log_lvl);
}

void LoggerInterface::start_log(const char *msg, uint8_t log_lvl) {
    last_started_log_lvl = log_lvl;
    if (last_started_log_lvl > current_log_lvl) {
        return;
    }

#if defined(Arduino_h)
    Serial.println();
#else
    std::cout << std::endl;
#endif
    char millis_buffer[26];
    sprintf(millis_buffer, "%ld", millis());
#if defined(Arduino_h)
    Serial.print(millis_buffer);
    Serial.print(": ");
    Serial.print(msg);
#else
    std::cout << millis_buffer << ": " << msg << std::flush;
#endif

}

void LoggerInterface::set_current_log_lvl(uint8_t log_lvl) {
    last_started_log_lvl = log_lvl;
}

void LoggerInterface::append_log(uint8_t *msg) {
    if (last_started_log_lvl > current_log_lvl) {
        return;
    }
#if defined(Arduino_h)
    Serial.print(*msg);
#else
    std::cout << msg << std::flush;
#endif

}

void LoggerInterface::append_log(char *msg) {
    append_log((const char *) msg);
}

void LoggerInterface::append_log(const char *msg) {
    if (last_started_log_lvl > current_log_lvl) {
        return;
    }
#if defined(Arduino_h)
    Serial.print(msg);
#else
    std::cout << msg << std::flush;
#endif
}

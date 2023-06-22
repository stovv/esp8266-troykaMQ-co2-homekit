#include <Arduino.h>
#include <TroykaMQ.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

// имя для пина, к которому подключен датчик
#define PIN_MQ135         A0
// пин кнопки сброса и калибровки
#define BUTTON_PIN 13

#define HOMEKIT_PASS "000-22-279"
#define HOMEKIT_SETUP_ID "3TXB"

// качество воздуха homekit
enum air_quality {
  unknown,   // неизвестно
  excellent, // отлично
  good,      // хорошее
  may_good,  // приемлемое
  low,       // плохое
  bad        // супер плохое
};

// создаём объект для работы с датчиком
// и передаём ему номер пина выходного сигнала и нагревателя
MQ135 mq135(PIN_MQ135);

// кнопка сброса и калибровки
int timeout = 2000;
EasyButton button(BUTTON_PIN);


void calibrate() {
    // выполняем калибровку датчика на чистом воздухе
    mq135.calibrate();
}

void reset_homekit() {
    // сбрасываем информацию о homekit
    homekit_server_reset();
    homekit_storage_reset();
}

void setup() {
    Serial.begin(115200);
    
    button.begin();
    // одно нажатие кнопки - калибровка датчика
    button.onSequence(1, timeout, calibrate);
    // долгое нажатие (5с) - сброс homekit
    button.onPressedFor(5000, reset_homekit);

    wifi_connect();

    // перед запуском нужно прогреть датчик минимум 60с
    LOG_D("Heating...60s");
    delay(60000);
    
    my_homekit_setup();
}

void loop() {
    button.read();
    my_homekit_loop();
    delay(10);
}

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_co2_level;
extern "C" homekit_characteristic_t cha_current_quality_level;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() {
    config.password = HOMEKIT_PASS;
    config.setupId = HOMEKIT_SETUP_ID;
    arduino_homekit_setup(&config);
}

void my_homekit_loop() {
    arduino_homekit_loop();
    const uint32_t t = millis();
    if (t > next_report_millis) {
        // report sensor values every 10 seconds
        next_report_millis = t + 10 * 1000;
        my_homekit_report();
    }

    if (t > next_heap_millis) {
        // show heap info every 5 seconds
        next_heap_millis = t + 5 * 1000;
        LOG_D("Free heap: %d, HomeKit clients: %d", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
    }
}

void my_homekit_report() {
    float dioxide = mq135.readCO2();
    if (dioxide > 100000){
      dioxide = 100000;
    }
    cha_current_co2_level.value.float_value = dioxide;
    LOG_D("Current co2: %.1f", dioxide);
    homekit_characteristic_notify(&cha_current_co2_level, cha_current_co2_level.value);

    int quality = air_quality::unknown;

    if (dioxide <= 600) {
        quality = air_quality::excellent;
    }

    if (dioxide > 600 && dioxide <= 800) {
        quality = air_quality::good;
    }

    if (dioxide > 800 && dioxide <= 1000) {
        quality = air_quality::may_good;
    }

    if (dioxide > 1000 && dioxide <= 1400) {
        quality = air_quality::low;
    }

    if (dioxide > 1400) {
        quality = air_quality::bad;
    }

    cha_current_quality_level.value.uint8_value = quality;
    LOG_D("Current quality: %d", quality);
    homekit_characteristic_notify(&cha_current_quality_level, cha_current_quality_level.value);    
}
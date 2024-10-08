#include <Arduino.h>
#include <esp_log.h>

#include "configuration.h"
#include "util.h"
#include "modules/Modules.h"

unsigned long lastGNSSsend = 0;
unsigned long lastAPRSsend = 0;
static const char* TAG = "main";
APRSClient *aprs;

void setup() {
    // Creates module class instances
    deployModules();

    // TODO: temporary reference to serial for debugging aid
    Serial.begin(115200);

    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "---------- ESP32-LoRa-APRS ----------");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "System starting");

    #if defined(HAS_I2C)
        i2cModule->initialize();
    #endif

    #if defined(HAS_PMU)
        powerModule->initialize();
    #endif

    radioModule->initialize();

    stateModule->initialize();
    
    aprs = new APRSClient(radioModule->getRadioInterface());
    char source[] = "SQ6NTI";
    int aprsState = aprs->begin('>', source, 5);
    ESP_LOGD(TAG, "APRS Client initialization state: %d", aprsState);

    ESP_LOGI(TAG, "Initialization complete");
}

void loop() {
    /*if (millis() - lastGNSSsend > 1000) {
        stateModule->checkPosition();
        lastGNSSsend = millis();
    }*/

    if (millis() - lastAPRSsend > 60000) {
        ESP_LOGD(TAG, "Sending APRS position");
        char destination[] = "GPS";
        char latitude[] = "5105.20N";
        char longitude[] = "01700.90E";
        char message[] = "test tracker";
        char timestamp[] = "093045z";
        int state = aprs->sendPosition(destination, 1, latitude, longitude, message, timestamp);
        ESP_LOGD(TAG, "APRS position sending state: %d", state);
        lastAPRSsend = millis();
    }
}

#include <homekit/homekit.h>
#include <homekit/characteristics.h>


void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

homekit_characteristic_t cha_current_co2_level = HOMEKIT_CHARACTERISTIC_(CARBON_DIOXIDE_LEVEL, 100);

homekit_characteristic_t cha_current_quality_level = HOMEKIT_CHARACTERISTIC_(AIR_QUALITY, 5);

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_sensor, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "CO2 Sensor"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Esp home"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "001122334456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(AIR_QUALITY_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
            &cha_current_quality_level,
            &cha_current_co2_level,
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};


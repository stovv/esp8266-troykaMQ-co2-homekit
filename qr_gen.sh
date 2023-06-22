homekit_accessory_category_other=1
homekit_accessory_category_bridge=2
homekit_accessory_category_fan=3
homekit_accessory_category_garage=4
homekit_accessory_category_lightbulb=5
homekit_accessory_category_door_lock=6
homekit_accessory_category_outlet=7
homekit_accessory_category_switch=8
homekit_accessory_category_thermostat=9
homekit_accessory_category_sensor=10
homekit_accessory_category_security_system=11
homekit_accessory_category_door=12
homekit_accessory_category_window=13
homekit_accessory_category_window_covering=14
homekit_accessory_category_programmable_switch=15
homekit_accessory_category_range_extender=16
homekit_accessory_category_ip_camera=17
homekit_accessory_category_video_door_bell=18
homekit_accessory_category_air_purifier=19
homekit_accessory_category_heater=20
homekit_accessory_category_air_conditioner=21
homekit_accessory_category_humidifier=22
homekit_accessory_category_dehumidifier=23
homekit_accessory_category_apple_tv=24
homekit_accessory_category_speaker=26
homekit_accessory_category_airport=27
homekit_accessory_category_sprinkler=28
homekit_accessory_category_faucet=29
homekit_accessory_category_shower_head=30
homekit_accessory_category_television=31
homekit_accessory_category_target_controller=32

PASS=$(cat CO2.ino | sed -n "s/^#define HOMEKIT_PASS \"\([^ ]*\)\"/\1/p")
SETUP_ID=$(cat CO2.ino | sed -n "s/^#define HOMEKIT_SETUP_ID \"\([^ ]*\)\"/\1/p")
CATEGORY_NAME=$(cat my_accessory.c | sed -n "s/.category=\([^ ]*\),/\1/p" | sed "s/ //g" )
CATEGORY_ID="${!CATEGORY_NAME}"

./tools/gen_qrcode "${CATEGORY_ID}" "${PASS}" "${SETUP_ID}" ./qr.png


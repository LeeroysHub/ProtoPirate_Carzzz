// helpers/protopirate_settings.h
#pragma once

#include <stdint.h>
#include <stdbool.h>

#define PROTOPIRATE_SETTINGS_FILE APP_DATA_PATH("settings.txt")
#define PROTOPIRATE_SETTINGS_DIR  APP_DATA_PATH()

#define PROTOPIRATE_TX_POWER_MAX_INDEX 8U

typedef union ProtoPirateOptionFlags {
    struct {
        uint8_t emulate_feature_enabled : 1;
        uint8_t auto_save               : 1;
        uint8_t sound                   : 1;
        uint8_t hopping_enabled         : 1;
        uint8_t check_saved             : 1;
        uint8_t datetime_filenames      : 1;
        uint8_t reserved                : 2;
    };
    uint8_t data;
} ProtoPirateOptionFlags;

typedef struct {
    uint32_t frequency;
    uint8_t preset_index;
    uint8_t tx_power;
    ProtoPirateOptionFlags option_flags;
    uint16_t car_model_index;
} ProtoPirateSettings;

void protopirate_settings_load(ProtoPirateSettings* settings);
void protopirate_settings_save(ProtoPirateSettings* settings);
void protopirate_settings_set_defaults(ProtoPirateSettings* settings);

// helpers/protopirate_settings.h
#pragma once

#include <stdint.h>
#include <stdbool.h>

#define PROTOPIRATE_SETTINGS_FILE APP_DATA_PATH("settings.txt")
#define PROTOPIRATE_SETTINGS_DIR  APP_DATA_PATH()

#define PROTOPIRATE_TX_POWER_MAX_INDEX 8U

typedef enum ProtoPirateSettingsOptionFlags {
    ProtoPirateSettingsOptionFlagsAutoSave = (1 << 0),
    ProtoPirateSettingsOptionFlagsSound = (1 << 1),
    ProtoPirateSettingsOptionFlagsHoppingEnabled = (1 << 2),
    ProtoPirateSettingsOptionFlagsEmulateFeatureEnabled = (1 << 3),
    ProtoPirateSettingsOptionFlagsCheckSaved = (1 << 4),
    ProtoPirateSettingsOptionFlagsDateTimeFileNames = (1 << 5),
} ProtoPirateSettingsOptionFlags;

#define APP_OPTION_ENABLED(flags, setting) ((flags & setting) == setting)
#define SET_APP_OPTION_ENABLED(flags, setting, enabled) \
    do {                                                \
        if(APP_OPTION_ENABLED(flags, setting)) {        \
            if(!enabled) flags -= setting;              \
        } else {                                        \
            if(enabled) flags += setting;               \
        }                                               \
    } while(0);

typedef struct {
    uint32_t frequency;
    uint8_t preset_index;
    uint8_t tx_power;
    uint8_t option_flags;
    uint16_t car_model_index;
} ProtoPirateSettings;

void protopirate_settings_load(ProtoPirateSettings* settings);
void protopirate_settings_save(ProtoPirateSettings* settings);
void protopirate_settings_set_defaults(ProtoPirateSettings* settings);

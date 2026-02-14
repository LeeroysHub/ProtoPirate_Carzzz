// helpers/protopirate_settings.h
#pragma once

#include <stdint.h>
#include <stdbool.h>

#define PROTOPIRATE_SETTINGS_FILE APP_DATA_PATH("settings.txt")
#define PROTOPIRATE_SETTINGS_DIR  APP_DATA_PATH()

typedef struct {
    uint32_t frequency;
    uint8_t preset_index;
    uint8_t tx_power;
    bool hopping_enabled;
    uint8_t option_flags;
} ProtoPirateSettings;

//AND Flags instead of a million booleans
#define FLAG_AUTO_SAVE          1
#define FLAG_DATETIME_FILENAMES 2

void protopirate_settings_load(ProtoPirateSettings* settings);
void protopirate_settings_save(ProtoPirateSettings* settings);
void protopirate_settings_set_defaults(ProtoPirateSettings* settings);

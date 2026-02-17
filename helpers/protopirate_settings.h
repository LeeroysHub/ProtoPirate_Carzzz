// helpers/protopirate_settings.h
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <protopirate_app_i.h>

#ifdef BUILD_MAIN_APP
#define PROTOPIRATE_SETTINGS_FILE APP_DATA_PATH("settings.txt")
#define PROTOPIRATE_SETTINGS_DIR  APP_DATA_PATH()
#else
#define PROTOPIRATE_SETTINGS_FILE "/ext/apps_data/proto_pirate/settings.txt"
#define PROTOPIRATE_SETTINGS_DIR  "/ext/apps_data/proto_pirate/"
#endif
#define PROTOPIRATE_MODELS_FILE APP_ASSETS_PATH("models.txt")

//AND Flags instead of a million booleans
#define FLAG_AUTO_SAVE          1
#define FLAG_DATETIME_FILENAMES 2

void protopirate_settings_load(ProtoPirateSettings* settings);
void protopirate_settings_save(ProtoPirateSettings* settings);
void protopirate_settings_set_defaults(ProtoPirateSettings* settings);

#ifdef BUILD_MAIN_APP
bool protopirate_model_get_by_index(
    ProtoPirateApp* app,
    ProtoPirateCarModel** car_model,
    uint16_t index);
uint16_t protopirate_model_get_count(void);
#endif

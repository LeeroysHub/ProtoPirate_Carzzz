// helpers/protopirate_settings.c
#include "protopirate_settings.h"
#include <storage/storage.h>
#include <flipper_format/flipper_format.h>
#include <furi.h>

#define TAG "ProtoPirateSettings"

#define SETTINGS_FILE_HEADER  "ProtoPirate Settings"
#define SETTINGS_FILE_VERSION 1
#define MODELS_FILE_HEADER    "ProtoPirate Car Models Database"
#define MODELS_FILE_VERSION   1

//For speed, so we start at a good counter.
#define MODELS_MINIMUN_COUNT 6;

void protopirate_settings_set_defaults(ProtoPirateSettings* settings) {
    settings->frequency = 433920000;
    settings->preset_index = 0;
    settings->tx_power = 0;
    settings->option_flags = 0;
    settings->hopping_enabled = false;
    settings->car_model_index = 0;
}

void protopirate_settings_load(ProtoPirateSettings* settings) {
    // Set defaults first
    protopirate_settings_set_defaults(settings);

    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(storage);

    do {
        if(!flipper_format_file_open_existing(ff, PROTOPIRATE_SETTINGS_FILE)) {
            FURI_LOG_I(TAG, "Settings file not found, using defaults");
            break;
        }

        FuriString* header = furi_string_alloc();
        uint32_t version = 0;

        if(!flipper_format_read_header(ff, header, &version)) {
            FURI_LOG_W(TAG, "Failed to read settings header");
            furi_string_free(header);
            break;
        }

        if(version != SETTINGS_FILE_VERSION) {
            FURI_LOG_W(TAG, "Unsupported settings version %lu", (unsigned long)version);
            furi_string_free(header);
            break;
        }

        if(furi_string_cmp_str(header, SETTINGS_FILE_HEADER) != 0) {
            FURI_LOG_W(TAG, "Invalid settings file header");
            furi_string_free(header);
            break;
        }

        furi_string_free(header);

        // Read frequency
        if(!flipper_format_read_uint32(ff, "Frequency", &settings->frequency, 1)) {
            FURI_LOG_W(TAG, "Failed to read frequency, using default");
            settings->frequency = 433920000;
        }

        // Read preset index
        uint32_t preset_temp = 0;
        if(!flipper_format_read_uint32(ff, "PresetIndex", &preset_temp, 1)) {
            FURI_LOG_W(TAG, "Failed to read preset index, using default");
            preset_temp = 0;
        }
        settings->preset_index = (uint8_t)preset_temp;

        // Read options flag (auto-save and date time filenames)
        uint32_t option_flags_temp = 0;
        if(!flipper_format_read_uint32(ff, "OptionFlags", &option_flags_temp, 1)) {
            FURI_LOG_W(TAG, "Failed to read Option Flags, using default");
            option_flags_temp = 0;
        }
        settings->option_flags = option_flags_temp;

        // Read tx-power
        uint32_t tx_power_temp = 0;
        if(!flipper_format_read_uint32(ff, "TXPower", &tx_power_temp, 1)) {
            FURI_LOG_W(TAG, "Failed to read TXPower, using default");
            tx_power_temp = 0;
        }
        settings->tx_power = (uint8_t)tx_power_temp;

        // Read hopping
        uint32_t hopping_temp = 0;
        if(!flipper_format_read_uint32(ff, "Hopping", &hopping_temp, 1)) {
            FURI_LOG_W(TAG, "Failed to read hopping, using default");
            hopping_temp = 0;
        }
        settings->hopping_enabled = (hopping_temp == 1);

#ifdef BUILD_MAIN_APP
        // Read Selected Car Model
        uint32_t car_model_index_temp = 0;
        if(!flipper_format_read_uint32(ff, "CarModelIndex", &car_model_index_temp, 1)) {
            car_model_index_temp = 0;
        }
        settings->car_model_index = car_model_index_temp;
#endif

        FURI_LOG_I(
            TAG,
            "Settings loaded: freq=%lu, preset=%u, auto_save=%d, hopping=%d, car_model=%u",
            settings->frequency,
            settings->preset_index,
            ((settings->option_flags & FLAG_AUTO_SAVE) == FLAG_AUTO_SAVE),
            settings->hopping_enabled,
            settings->car_modeL_index);
    } while(false);

    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

void protopirate_settings_save(ProtoPirateSettings* settings) {
    Storage* storage = furi_record_open(RECORD_STORAGE);

    // Ensure directory exists
    storage_simply_mkdir(storage, PROTOPIRATE_SETTINGS_DIR);

    FlipperFormat* ff = flipper_format_file_alloc(storage);

    do {
        if(!flipper_format_file_open_always(ff, PROTOPIRATE_SETTINGS_FILE)) {
            FURI_LOG_E(TAG, "Failed to open settings file for writing");
            break;
        }

        if(!flipper_format_write_header_cstr(ff, SETTINGS_FILE_HEADER, SETTINGS_FILE_VERSION)) {
            FURI_LOG_E(TAG, "Failed to write settings header");
            break;
        }

        if(!flipper_format_write_uint32(ff, "Frequency", &settings->frequency, 1)) {
            FURI_LOG_E(TAG, "Failed to write frequency");
            break;
        }

        uint32_t preset_temp = settings->preset_index;
        if(!flipper_format_write_uint32(ff, "PresetIndex", &preset_temp, 1)) {
            FURI_LOG_E(TAG, "Failed to write preset index");
            break;
        }

        uint32_t option_flags_temp = settings->option_flags;
        if(!flipper_format_write_uint32(ff, "OptionFlags", &option_flags_temp, 1)) {
            FURI_LOG_E(TAG, "Failed to write auto-save");
            break;
        }

        uint32_t tx_power_temp = settings->tx_power;
        if(!flipper_format_write_uint32(ff, "TXPower", &tx_power_temp, 1)) {
            FURI_LOG_E(TAG, "Failed to write TX Power");
            break;
        }

        uint32_t hopping_temp = settings->hopping_enabled ? 1 : 0;
        if(!flipper_format_write_uint32(ff, "Hopping", &hopping_temp, 1)) {
            FURI_LOG_E(TAG, "Failed to write hopping");
            break;
        }

#ifdef BUILD_MAIN_APP
        uint32_t car_model_index_temp = settings->car_model_index;
        if(!flipper_format_write_uint32(ff, "CarModelIndex", &car_model_index_temp, 1)) {
            FURI_LOG_E(TAG, "Failed to write car model");
            break;
        }
#endif

        FURI_LOG_I(
            TAG,
            "Settings saved: freq=%lu, preset=%u, auto_save=%d, hopping=%d",
            settings->frequency,
            settings->preset_index,
            ((settings->option_flags & FLAG_AUTO_SAVE) == FLAG_AUTO_SAVE),
            settings->hopping_enabled);

    } while(false);

    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

#ifdef BUILD_MAIN_APP
bool protopirate_model_get_by_index(
    ProtoPirateApp* app,
    ProtoPirateCarModel** car_model,
    uint16_t index) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(storage);
    FuriString* preset_name = furi_string_alloc();
    FuriString* model_name = furi_string_alloc();
    uint32_t frequency;
    uint8_t* preset_data = NULL;
    uint32_t preset_data_size = 0;

    bool error = true;
    bool custom_preset = false;

    //Allocate RAM for the Car Model into.
    if(!*car_model) {
        *car_model = malloc(sizeof(ProtoPirateCarModel));
        (*car_model)->name = furi_string_alloc();
        (*car_model)->preset = NULL; // important initialization
        (*car_model)->index = 0; // optional but clean
    }

    //Index 0 is None.
    switch(index) {
    case 0:
        //Leave as same as error (none)
        break;
    default: {
        if(!flipper_format_file_open_existing(ff, PROTOPIRATE_MODELS_FILE)) {
            FURI_LOG_I(TAG, "Models file not found");
            break;
        }

        FuriString* header = furi_string_alloc();
        uint32_t version = 0;

        if(!flipper_format_read_header(ff, header, &version)) {
            FURI_LOG_W(TAG, "Failed to read settings header");
            furi_string_free(header);
            break;
        }

        if(version != MODELS_FILE_VERSION) {
            FURI_LOG_W(TAG, "Unsupported models.txt version %lu", (unsigned long)version);
            furi_string_free(header);
            break;
        }

        if(furi_string_cmp_str(header, MODELS_FILE_HEADER) != 0) {
            FURI_LOG_W(TAG, "Invalid settings file header");
            furi_string_free(header);
            break;
        }

        furi_string_free(header);

        char model_name_index[16]; //Frequency65535 + NULL
        char frequency_index[16]; //Frequency65535 + NULL
        char preset_name_index[18]; //PresetName65535 + NULL
        char preset_data_index[23]; //PresetData65535 + NULL

        //Build the values we need to grab.
        snprintf(model_name_index, sizeof(model_name_index), "ModelName%u", index);
        snprintf(frequency_index, sizeof(frequency_index), "Frequency%u", index);
        snprintf(preset_name_index, sizeof(preset_name_index), "PresetName%u", index);
        snprintf(preset_data_index, sizeof(preset_data_index), "CustomPresetData%u", index);

        //Read the Model Name.
        if(!flipper_format_read_string(ff, model_name_index, model_name)) {
            FURI_LOG_E("ProtoPirate", "Failed to read %s", model_name_index)
            break;
        }

        // Read frequency
        if(!flipper_format_read_uint32(ff, frequency_index, &frequency, 1)) {
            FURI_LOG_W(TAG, "Failed to read frequency.");
            frequency = 0;
            break;
        }

        if(!flipper_format_read_string(ff, preset_name_index, preset_name)) {
            //Set the Custom preset name.
            furi_string_set_str(preset_name, "Custom");
            custom_preset = true;
        } else {
            //Get the built in preset if it exists
            const char* preset_name_long = furi_string_get_cstr(preset_name);
            const char* preset_name_short = preset_name_long;

            //Which Preset have we selected?
            if(!strcmp(preset_name_long, "FuriHalSubGhzPresetOok270Async")) {
                preset_name_short = "AM270";
            } else if(!strcmp(preset_name_long, "FuriHalSubGhzPresetOok650Async")) {
                preset_name_short = "AM650";
            } else if(!strcmp(preset_name_long, "FuriHalSubGhzPreset2FSKDev238Async")) {
                preset_name_short = "FM238";
            } else if(!strcmp(preset_name_long, "FuriHalSubGhzPreset2FSKDev12KAsync")) {
                preset_name_short = "FM12K";
            } else if(!strcmp(preset_name_long, "FuriHalSubGhzPreset2FSKDev476Async")) {
                preset_name_short = "FM476";
            } else if(!strcmp(preset_name_long, "FuriHalSubGhzPresetCustom")) {
                preset_name_short = "Custom";
                custom_preset = true;
            }

            //Set the preset_name
            furi_string_set_str(preset_name, preset_name_short);

            if(!custom_preset) {
                size_t preset_index = subghz_setting_get_preset_count(app->setting);
                for(size_t i = 0; i < subghz_setting_get_preset_count(app->setting); i++) {
                    if(!strcmp(
                           subghz_setting_get_preset_name(app->setting, i), preset_name_short)) {
                        preset_index = i;
                        break;
                    }
                }
                if(preset_index >= subghz_setting_get_preset_count(app->setting)) {
                    preset_name_short = "AM650";
                    for(size_t i = 0; i < subghz_setting_get_preset_count(app->setting); i++) {
                        if(!strcmp(
                               subghz_setting_get_preset_name(app->setting, i),
                               preset_name_short)) {
                            preset_index = i;
                            break;
                        }
                    }
                    if(preset_index >= subghz_setting_get_preset_count(app->setting)) {
                        FURI_LOG_E(TAG, "Failed to get preset index!");
                        break;
                    }
                }

                //Get the preset data into a buffer to copy it.
                uint8_t* temp_preset_data = NULL;
                uint32_t temp_preset_data_size = 0;
                temp_preset_data = subghz_setting_get_preset_data(app->setting, preset_index);
                temp_preset_data_size =
                    subghz_setting_get_preset_data_size(app->setting, preset_index);

                //Error fallback
                if(temp_preset_data == NULL) {
                    FURI_LOG_E(TAG, "Failed to get preset data!");
                    break;
                } else {
                    //Copy the preset data, so we can initialize it.
                    preset_data = malloc(temp_preset_data_size);
                    memcpy(preset_data, temp_preset_data, temp_preset_data_size);
                    preset_data_size = temp_preset_data_size;

                    //Yay, we made it...
                    error = false;
                }
            }
        }

        /* Custom_preset_data (only if present) */
        if(custom_preset &&
           flipper_format_get_value_count(ff, preset_data_index, &preset_data_size) &&
           preset_data_size > 0) {
            if(preset_data_size >= 1024) {
                FURI_LOG_E(
                    "ProtoPirate",
                    "%s too large: %lu",
                    preset_data_index,
                    (unsigned long)preset_data_size);
                break;
            }

            preset_data = malloc(preset_data_size);
            if(!preset_data) {
                FURI_LOG_E(
                    "ProtoPirate",
                    "Malloc failed: %s (%lu bytes)",
                    preset_data_index,
                    (unsigned long)preset_data_size);
                break;
            }

            flipper_format_rewind(ff);
            if(!flipper_format_read_hex(ff, preset_data_index, preset_data, preset_data_size)) {
                break;
            }

            //Yay, we made it...
            error = false;
        }
    }
    }

    if(!error) {
        //Set the Car Model name and index.
        furi_string_set((*car_model)->name, model_name);
        (*car_model)->index = index;

        //Alloc a preset if none, or recycle.
        if(!(*car_model)->preset) {
            (*car_model)->preset = malloc(sizeof(SubGhzRadioPreset));
            (*car_model)->preset->name = furi_string_alloc();
        } else {
            //Free old data..
            if((*car_model)->preset->data) {
                free((*car_model)->preset->data);
                (*car_model)->preset->data = NULL;
            }
        }
        furi_string_set((*car_model)->preset->name, preset_name);
        (*car_model)->preset->data = preset_data;
        (*car_model)->preset->data_size = preset_data_size;
        (*car_model)->preset->frequency = frequency;
    } else {
        (*car_model)->index = 0;
        furi_string_set_str(
            (*car_model)->name,
            (app->car_models_count) ? "< Select a Car Model >" : "No Models in Database");

        if((*car_model)->preset) {
            if((*car_model)->preset->data) {
                free((*car_model)->preset->data);
                (*car_model)->preset->data = NULL;
            }

            (*car_model)->preset->data_size = 0;

            if((*car_model)->preset->name) {
                furi_string_free((*car_model)->preset->name);
                (*car_model)->preset->name = NULL;
            }

            free((*car_model)->preset);
            (*car_model)->preset = NULL;
        }

        // If we allocated preset_data but never assigned it, free it
        if(custom_preset && preset_data_size) {
            free(preset_data);
        }
    }

    //Close up and return.
    flipper_format_free(ff);
    furi_string_free(model_name);
    furi_string_free(preset_name);
    furi_record_close(RECORD_STORAGE);
    return (!error);
}

uint16_t protopirate_model_get_count(void) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(storage);
    FuriString* header = furi_string_alloc();
    FuriString* tmp = furi_string_alloc();
    uint32_t version = 0;
    uint16_t count = 0;
    bool ok = true;

    if(!flipper_format_file_open_existing(ff, PROTOPIRATE_MODELS_FILE)) {
        goto cleanup;
    }

    if(!flipper_format_read_header(ff, header, &version)) goto cleanup;
    if(version != MODELS_FILE_VERSION || furi_string_cmp_str(header, MODELS_FILE_HEADER) != 0)
        goto cleanup;

    //Start at 1 if we have to search.
    uint16_t low = 1;
    uint16_t high = MODELS_MINIMUN_COUNT;
    while(ok) {
        char key[32];
        snprintf(key, sizeof(key), "ModelName%u", high);

        ok = flipper_format_read_string(ff, key, tmp);
        if(!ok) {
            if(low == 1) {
                //We found NO models in the database at our MODELS_MINIMUN_COUNT value.
                low = high = 1;
            } else {
                //1st Missing item ABOVE the counter. Now Search backwards for the true count.
                break;
            }
        }

        //IF we are on the 1st iteration, and we found the item at MODELS_MINIMUN_COUNT, check the 1 above.
        //We may have a shortcut to counting them all.
        if(low == 1) {
            snprintf(key, sizeof(key), "ModelName%u", high + 1);
            ok = flipper_format_read_string(ff, key, tmp);
            if(!ok) {
                ok = true;
                low = high;
                break;
            }
        }

        //Double the Size of the partition
        low = high;
        high *= 2;

        //Quit halfway point.
        if(high > 32767) {
            high = 65535;
            break;
        }
    }

    //If we dont have the count yet, Binary Searcg the count now..
    uint16_t left = low;
    uint16_t right = high;
    if(!ok) {
        while(left <= right) {
            uint16_t mid = left + ((right - left) / 2);

            char key[32];
            snprintf(key, sizeof(key), "ModelName%u", mid);
            flipper_format_rewind(ff);
            ok = flipper_format_read_string(ff, key, tmp);

            if(ok) {
                left = mid + 1; //We found an Item, go higher.
            } else {
                right = mid - 1; //No Item found, go lower.
            }
        }
    }
    count = right;

cleanup:
    furi_string_free(tmp);
    furi_string_free(header);
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
    return count;
}
#endif //BUILD_MAIN_APP

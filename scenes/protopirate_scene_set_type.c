//#include "../subghz_i.h"
//#include "../helpers/subghz_txrx_create_protocol_key.h"
//#include "../helpers/subghz_gen_info.h"
//#include "protopirate_scene.h"
//#include <lib/subghz/blocks/math.h>
#include "../protopirate_app_i.h"
#include "../helpers/protopirate_storage.h"
#define TAG "SubGhzSetType"

void protopirate_scene_set_type_submenu_callback(void* context, uint32_t index) {
    ProtoPirateApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

static const char* submenu_names[ProtoPirateSetTypeMAX] = {
    [ProtoPirateSetTypeFord_v0] = "Ford V0",
};

void protopirate_scene_set_type_on_enter(void* context) {
    ProtoPirateApp* app = context;

    for(ProtoPirateSetType i = 0; i < ProtoPirateSetTypeMAX; i++) {
        submenu_add_item(
            app->submenu, submenu_names[i], i, protopirate_scene_set_type_submenu_callback, app);
    }

    submenu_set_selected_item(
        app->submenu, scene_manager_get_scene_state(app->scene_manager, ProtoPirateSceneSetType));

    view_dispatcher_switch_to_view(app->view_dispatcher, ProtoPirateViewSubmenu);
}

//bool subghz_scene_set_type_generate_protocol_from_infos(ProtoPirateApp* app) {
/*GenInfo gen_info = *subghz->gen_info;
    bool generated_protocol = false;
    switch(gen_info.type) {
    case GenData:
        if(gen_info.data.te) {
            generated_protocol = subghz_txrx_gen_data_protocol_and_te(
                subghz->txrx,
                gen_info.mod,
                gen_info.freq,
                gen_info.data.name,
                gen_info.data.key,
                gen_info.data.bits,
                gen_info.data.te);
        } else {
            generated_protocol = subghz_txrx_gen_data_protocol(
                subghz->txrx,
                gen_info.mod,
                gen_info.freq,
                gen_info.data.name,
                gen_info.data.key,
                gen_info.data.bits);
        }
        break;
    case GenFaacSLH:
        generated_protocol = subghz_txrx_gen_faac_slh_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.faac_slh.serial,
            gen_info.faac_slh.btn,
            gen_info.faac_slh.cnt,
            gen_info.faac_slh.seed,
            gen_info.faac_slh.manuf);
        break;
    case GenKeeloq:
        generated_protocol = subghz_txrx_gen_keeloq_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.keeloq.serial,
            gen_info.keeloq.btn,
            gen_info.keeloq.cnt,
            gen_info.keeloq.manuf);
        break;
    case GenCameAtomo:
        generated_protocol = subghz_txrx_gen_came_atomo_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.came_atomo.serial,
            gen_info.came_atomo.cnt);
        break;
    case GenKeeloqBFT:
        generated_protocol = subghz_txrx_gen_keeloq_bft_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.keeloq_bft.serial,
            gen_info.keeloq_bft.btn,
            gen_info.keeloq_bft.cnt,
            gen_info.keeloq_bft.seed,
            gen_info.keeloq_bft.manuf);
        break;
    case GenAlutechAt4n:
        generated_protocol = subghz_txrx_gen_alutech_at_4n_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.alutech_at_4n.serial,
            gen_info.alutech_at_4n.btn,
            gen_info.alutech_at_4n.cnt);
        break;
    case GenSomfyTelis:
        generated_protocol = subghz_txrx_gen_somfy_telis_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.somfy_telis.serial,
            gen_info.somfy_telis.btn,
            gen_info.somfy_telis.cnt);
        break;
    case GenKingGatesStylo4k:
        generated_protocol = subghz_txrx_gen_kinggates_stylo_4k_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.kinggates_stylo_4k.serial,
            gen_info.kinggates_stylo_4k.btn,
            gen_info.kinggates_stylo_4k.cnt);
        break;
    case GenBenincaARC:
        generated_protocol = subghz_txrx_gen_beninca_arc_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.beninca_arc.serial,
            gen_info.beninca_arc.btn,
            gen_info.beninca_arc.cnt);
        break;
    case GenNiceFlorS:
        generated_protocol = subghz_txrx_gen_nice_flor_s_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.nice_flor_s.serial,
            gen_info.nice_flor_s.btn,
            gen_info.nice_flor_s.cnt,
            gen_info.nice_flor_s.nice_one);
        break;
    case GenSecPlus1:
        generated_protocol =
            subghz_txrx_gen_secplus_v1_protocol(subghz->txrx, gen_info.mod, gen_info.freq);
        break;
    case GenSecPlus2:
        generated_protocol = subghz_txrx_gen_secplus_v2_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.sec_plus_2.serial,
            gen_info.sec_plus_2.btn,
            gen_info.sec_plus_2.cnt);
        break;
    case GenPhoenixV2:
        generated_protocol = subghz_txrx_gen_phoenix_v2_protocol(
            subghz->txrx,
            gen_info.mod,
            gen_info.freq,
            gen_info.phoenix_v2.serial,
            gen_info.phoenix_v2.cnt);
        break;
    default:
        furi_crash("Not implemented");
        break;
    }

    if(generated_protocol) {
        subghz_file_name_clear(subghz);
        scene_manager_set_scene_state(
            subghz->scene_manager, SubGhzSceneSetType, SubGhzCustomEventManagerSet);
        scene_manager_next_scene(subghz->scene_manager, SubGhzSceneSaveName);
    } else {
        furi_string_set(subghz->error_str, "Function requires\nan SD card with\nfresh databases.");
        scene_manager_next_scene(subghz->scene_manager, SubGhzSceneShowError);
    }
    return generated_protocol; */
//}

bool protopirate_scene_set_type_on_event(void* context, SceneManagerEvent event) {
    ProtoPirateApp* app = context;
    bool generated_protocol = false;

    //TEMPORARY!
    UNUSED(app);

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case ProtoPirateSetTypeFord_v0:
            // Find the protocol in the registry
            /* const SubGhzProtocol* protocol = NULL;
            for(size_t i = 0; i < protopirate_protocol_registry.size; i++) {
                if(strcmp(protopirate_protocol_registry.items[i]->name, proto_name) == 0) {
                    protocol = protopirate_protocol_registry.items[i];
                    FURI_LOG_I(TAG, "Found protocol %s in registry at index %zu", proto_name, i);
                    break;
                }
            }*/
            return true;
        default:
            return false;
        }
        /* scene_manager_set_scene_state(subghz->scene_manager, SubGhzSceneSetType, event.event);

        subghz_gen_info_reset(subghz->gen_info);
        subghz_scene_set_type_fill_generation_infos(subghz->gen_info, event.event);

        if(scene_manager_get_scene_state(subghz->scene_manager, SubGhzSceneStart) ==
           SubmenuIndexAddManually) {
            generated_protocol = subghz_scene_set_type_generate_protocol_from_infos(subghz);
        } else if(
            scene_manager_get_scene_state(subghz->scene_manager, SubGhzSceneStart) ==
            SubmenuIndexAddManuallyAdvanced) {
            switch(subghz->gen_info->type) {
            case GenData: // Key (u64)
                scene_manager_next_scene(subghz->scene_manager, SubGhzSceneSetKey);
                break;
            case GenSecPlus1: // None
                return subghz_scene_set_type_generate_protocol_from_infos(subghz);
            case GenFaacSLH: // Serial (u32), Button (u8), Counter (u32), Seed (u32)
            case GenKeeloq: // Serial (u32), Button (u8), Counter (u16)
            case GenCameAtomo: // Serial (u32), Counter (u16)
            case GenKeeloqBFT: // Serial (u32), Button (u8), Counter (u16), Seed (u32)
            case GenAlutechAt4n: // Serial (u32), Button (u8), Counter (u16)
            case GenSomfyTelis: // Serial (u32), Button (u8), Counter (u16)
            case GenKingGatesStylo4k: // Serial (u32), Button (u8), Counter (u16)
            case GenBenincaARC: // Serial (u32), Button (u8), Counter (u32)
            case GenNiceFlorS: // Serial (u32), Button (u8), Counter (u16)
            case GenSecPlus2: // Serial (u32), Button (u8), Counter (u32)
            case GenPhoenixV2: // Serial (u32), Counter (u16)
                scene_manager_next_scene(subghz->scene_manager, SubGhzSceneSetSerial);
                break;
            } */
    }
    // }
    //scene_manager_get_scene_state(
    //    const SceneManager* appscene_manager, uint32_t scene_id)
    return generated_protocol;
}

void protopirate_scene_set_type_on_exit(void* context) {
    ProtoPirateApp* app = context;
    submenu_reset(app->submenu);
}

/* bool protopirate_create_remote(
    FlipperFormat* flipper_format,
    const char* protocol_name,
    FuriString* out_path) {
    if(!protopirate_storage_init()) {
        FURI_LOG_E(TAG, "Failed to create app folder");
        return false;
    }

    FuriString* file_path = furi_string_alloc();

    if(!protopirate_storage_get_next_filename(protocol_name, file_path)) {
        FURI_LOG_E(TAG, "Failed to get next filename");
        furi_string_free(file_path);
        return false;
    }

    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* save_file = flipper_format_file_alloc(storage);
    bool result = false;

    do {
        // Create new file
        if(!flipper_format_file_open_new(save_file, furi_string_get_cstr(file_path))) {
            FURI_LOG_E(TAG, "Failed to create file");
            break;
        }

        // Write standard SubGhz file header
        if(!flipper_format_write_header_cstr(save_file, "Flipper SubGhz Key File", 1)) {
            FURI_LOG_E(TAG, "Failed to write header");
            break;
        }

        protopirate_storage_write_capture_data(save_file, flipper_format);

        if(out_path) furi_string_set(out_path, file_path);

        // Invalidate cache
        g_file_list_valid = false;

        result = true;
        FURI_LOG_I(TAG, "Saved capture to %s", furi_string_get_cstr(file_path));

    } while(false);

    flipper_format_free(save_file);
    furi_string_free(file_path);
    furi_record_close(RECORD_STORAGE);
    return result;
} */

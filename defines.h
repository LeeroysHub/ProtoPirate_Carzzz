#pragma once
#include <furi/core/log.h>

//*** You cannot Build the EXTERNAL features  INTERNAL features IN SAME FAP ***
//*** The following items are MUTUALLY EXCLUSIVE ***
// ENABLE_EMULATE_FEATURE / ENABLE_EMULATE_EXTERNAL
// ENABLE_SUB_DECODE_SCENE / ENABLE_SUB_DECODE_EXTERNAL
// ENABLE_TIMING_TUNER_SCENE / ENABLE_TIMING_TUNER_EXTERNAL

#ifdef BUILD_EMULATE_APP
#define ENABLE_EMULATE_FEATURE
#define ENABLE_SAVED_SCENE
#else
#ifdef BUILD_RECEIVER_APP
#define ENABLE_RECEIVER_SCENE
#define ENABLE_SAVED_SCENE
//#define ENABLE_SET_TYPE_SCENE
#define ENABLE_SUB_DECODE_EXTERNAL
#define ENABLE_TIMING_TUNER_EXTERNAL
#define ENABLE_EMULATE_EXTERNAL
#else
#define ENABLE_SUB_DECODE_SCENE
#define ENABLE_TIMING_TUNER_SCENE
#endif
#endif

//The in-built variable_item_list has the Left/Right event mod
//We can remove the module if FW ever getes the changes...
//#define USE_BUILT_IN_VARIABLE_ITEM_LIST

#define REMOVE_LOGS

#ifdef REMOVE_LOGS
// Undefine existing macros
#undef FURI_LOG_E
#undef FURI_LOG_W
#undef FURI_LOG_I
#undef FURI_LOG_D
#undef FURI_LOG_T
// Define empty macros
#define FURI_LOG_E(tag, format, ...)
#define FURI_LOG_W(tag, format, ...)
#define FURI_LOG_I(tag, format, ...)
#define FURI_LOG_D(tag, format, ...)
#define FURI_LOG_T(tag, format, ...)

#endif // REMOVE_LOGS

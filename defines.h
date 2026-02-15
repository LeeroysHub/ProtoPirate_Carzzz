#pragma once
#include <furi/core/log.h>

//This is done by the FAP now.
//#define BUILD_MAIN_APP

#ifdef BUILD_MAIN_APP
#define ENABLE_EMULATE_FEATURE
#define ENABLE_RECEIVER_SCENE
#define ENABLE_SAVED_SCENE
//#define ENABLE_SET_TYPE_SCENE
#else
#define ENABLE_TIMING_TUNER_SCENE
#define ENABLE_SUB_DECODE_SCENE
#endif

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

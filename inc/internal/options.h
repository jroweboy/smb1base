
#ifndef __OPTIONS_H
#define __OPTIONS_H



#ifndef MAPPER_MMC3
  #define MAPPER_MMC3 0
#endif
#ifndef MAPPER_MMC5
  #define MAPPER_MMC5 0
#endif
#ifndef USE_MMC5_AUDIO
  #define USE_MMC5_AUDIO 0
#endif
#ifndef USE_VRC7_AUDIO
  #define USE_VRC7_AUDIO 0
#endif
#ifndef USE_VANILLA_MUSIC
  #define USE_VANILLA_MUSIC 0
#endif
#ifndef USE_FAMISTUDIO_MUSIC
  #define USE_FAMISTUDIO_MUSIC 0
#endif
#ifndef DEBUG_WORLD_SELECT
  #define DEBUG_WORLD_SELECT 0
#endif
#ifndef PRINT_METASPRITE_IDS
  #define PRINT_METASPRITE_IDS 0
#endif
#ifndef MULTIPLE_POWERUPS_ON_SCREEN
  #define MULTIPLE_POWERUPS_ON_SCREEN 0
#endif

#ifndef MOUSE_CONFIG_CONTROLLER_SIZE
  #define MOUSE_CONFIG_CONTROLLER_SIZE 0
#endif
#ifndef MOUSE_CONFIG_SENSITIVITY
  #define MOUSE_CONFIG_SENSITIVITY 0
#endif
#ifndef MOUSE_X_MINIMUM
  #define MOUSE_X_MINIMUM 0
#endif
#ifndef MOUSE_X_MAXIMUM
  #define MOUSE_X_MAXIMUM 0
#endif
#ifndef MOUSE_Y_MINIMUM
  #define MOUSE_Y_MINIMUM 0
#endif
#ifndef MOUSE_Y_MAXIMUM
  #define MOUSE_Y_MAXIMUM 0
#endif

// This is just here to make things look nicer for code editors
#ifdef __NES__

#ifndef ENABLE_C_CODE
  #define ENABLE_C_CODE 0
#endif
#ifndef USE_CUSTOM_TITLESCREEN
  #define USE_CUSTOM_TITLESCREEN 0
#endif
#ifndef USE_MOUSE_SUPPORT
  #define USE_MOUSE_SUPPORT 0
#endif

#else

#define ENABLE_C_CODE 1
#define USE_CUSTOM_TITLESCREEN 1
#define USE_MOUSE_SUPPORT 1

#endif

#endif

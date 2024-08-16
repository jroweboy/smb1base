
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
#ifndef USE_MMC5_FOR_VANILLA_SFX
  #define USE_MMC5_FOR_VANILLA_SFX 0
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
#ifndef WORLD_HAX
  #define WORLD_HAX 0
#endif
#ifndef PRINT_METASPRITE_IDS
  #define PRINT_METASPRITE_IDS 0
#endif

#ifdef __NES__

#ifndef ENABLE_C_CALLBACKS
  #define ENABLE_C_CALLBACKS 0
#endif

#else // Enable C callbacks for code editors

#define ENABLE_C_CALLBACKS 1

#endif

#endif

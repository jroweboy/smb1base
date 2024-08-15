

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Easy edit options for controlling the basic feature set available


;;;;;;;;;;;;;;;;;;;;;;
;; GENERAL

; Mapper MMC5 is a more powerful mapper, but may have some compatibility issues with older emulators
; and powerpaks that aren't using the latest mapper set.
; It still has very high compatibility, and with the extra features and audio channels, its the recommended option
MAPPER_MMC5 = 1

; Mapper MMC3 is the most compatible while still being reasonable feature rich target. Enabling this will reduce
; the amount of CHR available, so its only included for scenarios like making a repro cart and can work around the
; missing features.
; MAPPER_MMC3 = 1


;;;;;;;;;;;;;;;;;;;;;;
;; AUDIO

; ---- MUSIC ----
; Enable one of the follow music engines by uncommenting the one you want to use

; Use the original game audio engine for music
USE_VANILLA_MUSIC = 1

; Use the Famistudio engine for music.
; USE_FAMISTUDIO_MUSIC = 1


; ---- SFX ----
; Use the original SFX audio engine.
USE_VANILLA_SFX = 1


; EXTRA
; If using famistudio or famitone engines and the MMC5 mapper, you can use the extra audio channels on the MMC5
; USE_MMC5_AUDIO = 1

; If using famistudio or famitone engines and the VRC7 mapper, you can use the extra audio channels on the MMC5
; USE_VRC7_AUDIO = 1

; If using MMC5 mapper and USE_VANILLA_SFX, this will use the additional audio channels to play the sound effects
; which keeps sound effects from interrupting the music.
; USE_MMC5_FOR_VANILLA_SFX = 1


;;;;;;;;;;;;;;;;;;;;;;
;; DEBUG FLAGS

; Enable WORLD_HAX to turn on world select. When in the level loading screen, press b to increase the level number and
; press a to increase the world number.
WORLD_HAX = 1

; Enable to print out all metasprite IDs when compiling. This can be useful to help when debugging
; PRINT_METASPRITE_IDS = 1






;;;;;;;;;;;;;;;;;;;
; YOU DO NOT NEED TO EDIT BELOW THIS LINE
;
; Internal validation for the options. If you see one of these error messages, you need to follow the instructions and
; update the options above.


; Verify mapper selection

.macro bool_option name
  .if .not .defined(.ident(.string(name)))
    .ident(.string(name)) .set 0
  .elseif .not ((.ident(.string(name)) = 1) .or (.ident(.string(name)) = 0))
    ; if you get this error then you've set a boolean config option to something other than 0 or 1
    .error .sprintf("Boolean Option `%s` was set to a value other than 0 or 1", .string(name))
  .endif
.endmacro

bool_option MAPPER_MMC3
bool_option MAPPER_MMC5

.if MAPPER_MMC3 + MAPPER_MMC5 > 1
  ; Check that `MAPPER_MMC3` and `MAPPER_MMC5` aren't both uncommented and that only one of them are set to 1
  .error "Multiple mappers selected at the same time! Only one mapper can be chosen at once"
.endif

.if MAPPER_MMC3 + MAPPER_MMC5 = 0
  ; Check that exactly one mapper option is uncommented and that it is set to 1
  .error "Must select one of MAPPER_MMC3 or MAPPER_MMC5"
.endif

; verify audio options

bool_option USE_MMC5_AUDIO
bool_option USE_MMC5_FOR_VANILLA_SFX
bool_option USE_VRC7_AUDIO
bool_option USE_VANILLA_MUSIC
bool_option USE_FAMISTUDIO_MUSIC

.if (.not MAPPER_MMC5) .and USE_MMC5_AUDIO
  ; Either disable USE_MMC5_AUDIO or switch to using MAPPER_MMC5 for this feature to work
  .error "Cannot use MMC5 Audio channels if the mapper isn't MMC5"
.endif

.if (.not MAPPER_MMC5) .and USE_MMC5_FOR_VANILLA_SFX
  ; Either disable USE_MMC5_FOR_VANILLA_SFX or switch to using MAPPER_MMC5 for this feature to work
  .error "Cannot use MMC5 Audio channels for SFX if the mapper isn't MMC5"
.endif

; verify debug options

bool_option WORLD_HAX
bool_option PRINT_METASPRITE_IDS



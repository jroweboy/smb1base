
;;;;;;;;;;;;;;;;;;;
; Internal validation for the options. If you see one of these error messages, you need to follow the instructions and
; update the options above.

.macro bool_option name
  .if .not .defined(.ident(.string(name)))
    .ident(.string(name)) .set 0
  .elseif .not ((.ident(.string(name))=1) .or (.ident(.string(name))=0))
    ; if you get this error then you've set a boolean config option to something other than 0 or 1
    .error .sprintf("Boolean Option `%s` was set to a value other than 0 or 1", .string(name))
    .fatal "Invalid Options selected"
  .endif
.endmacro



bool_option MAPPER_MMC3
bool_option MAPPER_MMC5
bool_option ENABLE_C_CALLBACKS

bool_option USE_MMC5_AUDIO
bool_option USE_MMC5_FOR_VANILLA_SFX
bool_option USE_VRC7_AUDIO
bool_option USE_VANILLA_MUSIC
bool_option USE_FAMISTUDIO_MUSIC

bool_option USE_VANILLA_SFX
bool_option USE_CUSTOM_ENGINE_SFX

bool_option WORLD_HAX
bool_option PRINT_METASPRITE_IDS


; Verify mapper selection

.if MAPPER_MMC3 + MAPPER_MMC5 > 1
  ; Check that `MAPPER_MMC3` and `MAPPER_MMC5` aren't both uncommented and that only one of them are set to 1
  .error "Multiple mappers selected at the same time! Only one mapper can be chosen at once"
  .fatal "Invalid Options selected"
.endif

.if MAPPER_MMC3 + MAPPER_MMC5=0
  ; Check that exactly one mapper option is uncommented and that it is set to 1
  .error "Must select one of MAPPER_MMC3 or MAPPER_MMC5"
  .fatal "Invalid Options selected"
.endif

; verify audio options

.if (.not MAPPER_MMC5) .and USE_MMC5_AUDIO
  ; Either disable USE_MMC5_AUDIO or switch to using MAPPER_MMC5 for this feature to work
  .error "Cannot use MMC5 Audio channels if the mapper isn't MMC5"
  .fatal "Invalid Options selected"
.endif

.if (.not MAPPER_MMC5) .and USE_MMC5_FOR_VANILLA_SFX
  ; Either disable USE_MMC5_FOR_VANILLA_SFX or switch to using MAPPER_MMC5 for this feature to work
  .error "Cannot use MMC5 Audio channels for SFX if the mapper isn't MMC5"
  .fatal "Invalid Options selected"
.endif

; verify debug options


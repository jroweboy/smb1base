

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

; MUSIC
; Use the original game audio engine for music
USE_VANILLA_MUSIC = 1
; Use the Famistudio engine for 
; USE_FAMISTUDIO_MUSIC = 1

; SFX
; Use the original SFX audio engine.
USE_VANILLA_SFX = 1

; EXTRA
; If using famistudio or famitone engines and the MMC5 mapper, you can use the extra audio channels on the MMC5
; for additional 
USE_MMC5_AUDIO = 0

; If using MMC5 mapper and USE_VANILLA_SFX, this will use the additional audio channels to play the sound effects
; which keeps them from interrupting the music.
USE_MMC5_FOR_VANILLA_SFX = 1


;;;;;;;;;;;;;;;;;;;;;;
;; DEBUG FLAGS

; Enable WORLD_HAX to turn on world select. When in the level loading screen, press b to increase the level number and
; press a to increase the world number.
WORLD_HAX = 1


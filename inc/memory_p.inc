.ifndef __MEMORY_P_INC
__MEMORY_P_INC = 1

.macro RESERVE label, size
.global .ident(.string(label))
.ifdef _MEMORY_DEFINE_MEMORY
.if _MEMORY_DEFINE_MEMORY = 1
.ident(.string(label)): .res size
.endif
.endif
.endmacro

.macro RESERVEZP label, size
.globalzp .ident(.string(label))
.ifdef _MEMORY_DEFINE_MEMORY
.if _MEMORY_DEFINE_MEMORY = 1
.ident(.string(label)): .res size
.endif
.endif
.endmacro

.endif ; __MEMORY_P_INC

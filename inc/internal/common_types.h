
#ifndef __COMMON_TYPES_H
#define __COMMON_TYPES_H

// #include <stdbool.h>
#include <stdint.h>

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;


// Common useful macros

void farcall_trampoline(void);

#define _MACRO_STRINGIFY(a) #a

#define PUSHSEG(a) \
  _Pragma(_MACRO_STRINGIFY(code-name (push, #a );)); \
  _Pragma(_MACRO_STRINGIFY(data-name (push, #a );)); \
  _Pragma(_MACRO_STRINGIFY(rodata-name (push, #a );));


#define POPSEG() \
  _Pragma(_MACRO_STRINGIFY(code-name (pop);)); \
  _Pragma(_MACRO_STRINGIFY(data-name (pop);)); \
  _Pragma(_MACRO_STRINGIFY(rodata-name (pop);));

#define WRAPPED(func_decl) \
  _Pragma(_MACRO_STRINGIFY(wrapped-call (push, farcall_trampoline, bank);));\
  func_decl; \
  _Pragma(_MACRO_STRINGIFY(wrapped-call (pop);));\


#define DISABLE_NMI() NmiDisable = 0xff;
#define ENABLE_NMI() NmiDisable = 0x00;

#define _MACRO_ZP_1(a)      _Pragma (_MACRO_STRINGIFY(zpsym ( #a );));
#define _MACRO_ZP_2(a,b)    _MACRO_ZP_1(a) _MACRO_ZP_1 (b)
#define _MACRO_ZP_3(a,...)  _MACRO_ZP_1(a) _MACRO_ZP_2 (__VA_ARGS__)
#define _MACRO_ZP_4(a,...)  _MACRO_ZP_1(a) _MACRO_ZP_3 (__VA_ARGS__)
#define _MACRO_ZP_5(a,...)  _MACRO_ZP_1(a) _MACRO_ZP_4 (__VA_ARGS__)
#define _MACRO_ZP_6(a,...)  _MACRO_ZP_1(a) _MACRO_ZP_5 (__VA_ARGS__)
#define _MACRO_ZP_7(a,...)  _MACRO_ZP_1(a) _MACRO_ZP_6 (__VA_ARGS__)
#define _MACRO_ZP_8(a,...)  _MACRO_ZP_1(a) _MACRO_ZP_7 (__VA_ARGS__)
#define _MACRO_ZP_9(a,...)  _MACRO_ZP_1(a) _MACRO_ZP_8 (__VA_ARGS__)
#define _MACRO_ZP_10(a,...) _MACRO_ZP_1(a) _MACRO_ZP_9 (__VA_ARGS__)
#define _MACRO_ZP_11(a,...) _MACRO_ZP_1(a) _MACRO_ZP_10(__VA_ARGS__)
#define _MACRO_ZP_12(a,...) _MACRO_ZP_1(a) _MACRO_ZP_11(__VA_ARGS__)
#define _MACRO_ZP_13(a,...) _MACRO_ZP_1(a) _MACRO_ZP_12(__VA_ARGS__)
#define _MACRO_ZP_14(a,...) _MACRO_ZP_1(a) _MACRO_ZP_13(__VA_ARGS__)
#define _MACRO_ZP_15(a,...) _MACRO_ZP_1(a) _MACRO_ZP_14(__VA_ARGS__)
#define _MACRO_ZP_16(a,...) _MACRO_ZP_1(a) _MACRO_ZP_15(__VA_ARGS__)

// NUM_ARGS(...) evaluates to the literal number of the passed-in arguments.
#define _MACRO_NUM_ARGS2(X,X16,X15,X14,X13,X12,X11,X10,X9,X8,X7,X6,X5,X4,X3,X2,X1,N,...) N
#define _MACRO_NUM_ARGS(...) _MACRO_NUM_ARGS2(0, __VA_ARGS__ ,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define _MACRO_ZP_COUNT_PASS2(N, ...) _MACRO_ZP_ ## N(__VA_ARGS__)
#define _MACRO_ZP_COUNT_PASS1(N, ...) _MACRO_ZP_COUNT_PASS2(N, __VA_ARGS__)

/**
 * @brief Define up to 16 symbols as a Zeropage variable
 * 
 * Example:
 * ZP(frame_count, game_state);
 */
#define ZP(...) _MACRO_ZP_COUNT_PASS1(_MACRO_NUM_ARGS(__VA_ARGS__), __VA_ARGS__)


#define __LIB_CALLSPEC __fastcall__

#endif // __COMMON_TYPES_H
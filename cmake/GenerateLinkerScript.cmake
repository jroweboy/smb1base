include_guard()

function(generate_linker_script)
  cmake_policy(SET CMP0007 NEW)

  set(TEMPLATE [=[
# THIS FILE IS GENERATED DO NOT EDIT
# Check GenerateLinkerScript.cmake for more details
MEMORY {
  # INES header "start" doesn't matter as no code will reference it
  HEADER:     start =    $0, size =   $10, type = ro, file = %O\;

  # CPU space (nes)
  ZEROPAGE:   start = $0000, size =  $100, type = rw, file = ""\;
  
  # small bit of RAM from $100 to $200
  SMALLRAM:   start = $0100, size =  $100, type = rw, file = ""\;
  # OAM DMA wants to start at a multiple of $200, splitting "SMALLRAM" and "RAM"
  OAM:        start = $0200, size =  $100, type = rw, file = ""\;
  RAM:        start = $0300, size =  $400, type = rw, file = ""\;
  # RAMMIR:   start = $0800, size = $1800, type = rw, file = ""\;

  PPU:        start = $2000, size =    $8, type = rw, file = ""\;
  # PPUMIR:   start = $2008, size = $1FF8, type = rw, file = ""\;

  CPU:        start = $4000, size =   $18, type = rw, file = ""\;

  # CPU space (cartridge)
  # UNUSED:   start = $4020, size = $1FE8, type = rw, file = ""\;
  # PRGRAM:   start = $6000, size = $2000, type = rw, file = ""\;

  @PRG_MEMORY@

  # CHR space
  CHR:        start = $0000, size = @CHR_SIZE@, type = ro, file = %O, fill = yes, fillval = $FF\;
}

# SEGMENTS DEFINITION
# model the mapping of code segments to memory areas
SEGMENTS {
  HEADER:     load = HEADER,      type = ro\;
  
  ZEROPAGE:   load = ZEROPAGE,    type = rw,  optional = yes\;
  SMALLRAM:   load = SMALLRAM,    type = rw,  optional = yes\;
  OAM:        load = OAM,         type = rw,  optional = yes\;
  
  RAM:        load = RAM,         type = rw,  optional = yes\;
  BSS:        load = RAM,         type = rw,  define = yes\; # C support

  PPU:        load = PPU,         type = rw,  optional = yes\;
  CPU:        load = CPU,         type = rw,  optional = yes\;

  @PRG_SEGMENT@

  VECTORS:    load = @PRG_LAST_MEMORY@,   type = ro,  start = $FFFA\;

  CHR:        load = CHR,         type = ro,  optional = yes\;
}

SYMBOLS {
  # # These values should really just be set by nes2header
  # INES_MAPPER:    type = weak, value = @INES_MAPPER@\;
  # INES_MIRROR:    type = weak, value = @INES_MIRROR@\; # 'h'|'H'|'v'|'V'|'4'|218
  # INES_SRAM:      type = weak, value = @INES_SRAM@\;   # 1 = battery backed SRAM at $6000-7FFF
  # INES_PRG_BANKS: type = weak, value = @inesPrgBanks@\;   # number of 16kb banks of PRG ROM (for rom size, not mapper banks)
  # INES_CHR_BANKS: type = weak, value = @inesChrBanks@\;   # number of 8kb banks of CHR ROM (for rom size, not mapper banks)

  @SYMBOLS_C_SUPPORT@
}
]=])

  set(options)
  set(oneValueArgs SRC DEST)
  set(multiValueArgs)
  cmake_parse_arguments(PARSE_ARGV 0 "LS" "${options}" "${oneValueArgs}" "${multiValueArgs}")

  message(STATUS "file: ${LS_SRC}")

  file(STRINGS ${LS_SRC} LS_CONTENTS)

  set(nes2prgSize 0)
  set(lineNum 0)
  set(bankLineNum)
  set(bankAddr)
  set(bankSize)
  set(segmentLineNum)
  set(segmentName)
  set(segmentAddr)
  set(segmentBank)
  set(segmentStart)

  # matches any number of allowed whitespace characters
  set(R_SP "[ \t]+")
  # captures one hex or decimal number
  set(R_HEX "(\\$?[0-9a-fA-F]+)")
  # matches a hex number and captures the hex portion
  set(R_MHX "\\$([0-9a-fA-F]+)")
  # captures one identifier
  set(R_ID "([a-zA-Z][a-zA-Z0-9]*)")

  # Parse the LS_CONTENTS looking for mapper and bank/segment information
  while (LS_CONTENTS)
    list(POP_FRONT LS_CONTENTS LINE)
    math(EXPR LINE_NUM "${LINE_NUM}+1")
    # This all works, its just pointless since this is all information exported in the source anyway.
    # elseif (LINE MATCHES "[^;]*nes2mapper +([0-9]+)")
    #   set(INES_MAPPER ${CMAKE_MATCH_1})
    # elseif (LINE MATCHES "[^;]*nes2mirror +('h'|'H'|'v'|'V'|'4'|218)")
    #   set(INES_MIRROR ${CMAKE_MATCH_1})
    # Get the PRG total size to verify that the bank sizes add up
    if (LINE MATCHES "[^;]*nes2prg${R_SP}${R_HEX}")
      set(prgValue ${CMAKE_MATCH_1})
      if(prgValue MATCHES "${R_MHX}")
        # hex2dec
        math(EXPR prgValue "0x${CMAKE_MATCH_1}" OUTPUT_FORMAT DECIMAL)
      endif()
      math(EXPR inesPrgBanks "${prgValue} / 0x4000" OUTPUT_FORMAT DECIMAL)
    elseif (LINE MATCHES "[^;]*nes2chr${R_SP}${R_HEX}")
      set(chrValue ${CMAKE_MATCH_1})
      if(chrValue MATCHES "${R_MHX}")
        # hex2dec
        math(EXPR chrValue "0x${CMAKE_MATCH_1}" OUTPUT_FORMAT DECIMAL)
      endif()
      math(EXPR inesChrBanks "${chrValue} / 0x2000" OUTPUT_FORMAT DECIMAL)
    elseif (LINE MATCHES "[^;]*CreateBanks[ \t]+(\\$?[0-9a-fA-F]+)[ \t]*,[ \t]*(\\$?[0-9a-fA-F]+),")

    elseif (LINE MATCHES "[^;]*CreateSegment +([a-zA-Z][a-zA-Z0-9]*)[ ]*,[ ]*([0-9\\|]+),[ ]*(\\$?[0-9a-fA-F]*)[ ]*([ ]*,[ ]*\\$?[0-9a-fA-F]+)?")
    list(APPEND segmentName ${CMAKE_MATCH_1})
      list(APPEND segmentName ${CMAKE_MATCH_1})
      set(segmentAddrSize ${CMAKE_MATCH_2})
      list(APPEND segmentBank ${CMAKE_MATCH_3})
      set(segmentStartVal ${CMAKE_MATCH_4})

      if(segmentAddrSize MATCHES "\\$([0-9]+)")
        # hex2dec
        math(EXPR segmentAddrSize "0x${CMAKE_MATCH_1}" OUTPUT_FORMAT DECIMAL)
      endif()
      list(APPEND segmentAddr ${segmentAddrSize})

      if(${segmentStartVal} MATCHES "\\$([0-9]+)")
        math(EXPR segmentStartVal "0x${CMAKE_MATCH_1}" OUTPUT_FORMAT DECIMAL)
      elseif(${segmentStartVal} MATCHES "([0-9]+)")
        set(segmentStartVal ${CMAKE_MATCH_1})
      else()
        set(segmentStartVal 0)
      endif()
      list(APPEND segmentStart ${segmentStartVal})
    endif()
  endwhile()
  
  message(STATUS "segmentName ${segmentName}")
  message(STATUS "segmentBank ${segmentBank}")
  message(STATUS "segmentStart ${segmentStart}")

  set(CHR_SIZE "${chrValue}")
  set(PRG_MEMORY_TEMPLATE [=[
PRG@prgBankNum@: start = $@prgAddrHex@, size = @prgBankSize@, type = ro, file = %O, fill = yes, fillval = @prgBankNum@, align = @PRG_ALIGN@\\;
]=])
  set(PRG_SEGMENT_TEMPLATE [=[
@prgSegmentName@: load = @prgMemoryName@, type = ro\\;
]=])

  set(PRG_MEMORY)
  list(LENGTH segmentBank LIST_LEN)
  math(EXPR LIST_LEN "${LIST_LEN} - 1" OUTPUT_FORMAT DECIMAL)
  foreach(index RANGE ${LIST_LEN})
    list(GET segmentBank ${index} thisBankList)
    list(LENGTH segmentBank WHATEVER)
    string(REPLACE "|" ";" bankList ${thisBankList})
    list(GET bankList 0 prgBankNum)
    message(STATUS "blah i ${index} w ${WHATEVER} s ${segmentBank} l ${thisBankList} b ${bankList} p ${prgBankNum}")
    list(GET bankList 0 prgBankNum)
    
    # math(EXPR prgValue "" OUTPUT_FORMAT HEXADECIMAL)
    
    string(CONFIGURE "${PRG_MEMORY_TEMPLATE}" THIS_PRG)
    list(APPEND PRG_MEMORY ${THIS_PRG})
  endforeach()
  
  string(CONFIGURE "${TEMPLATE}" TEMPLATEOUT)

  file(WRITE ${LS_DEST} ${TEMPLATEOUT})

endfunction()

function(generate_ca65_linker_cmd)

  set(options)
  set(oneValueArgs TARGET HEADER CONFIG)
  set(multiValueArgs)
  cmake_parse_arguments(PARSE_ARGV 0 "CFG" "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if (CFG_HEADER)
    set(CFG_CONFIG ${CMAKE_BINARY_DIR}/nes.cfg)
    cmake_path(ABSOLUTE_PATH CFG_HEADER OUTPUT_VARIABLE ABS_CFG_HEADER)
    message(STATUS "target: ${CFG_TARGET} file: ${CFG_HEADER} abs file: ${ABS_CFG_HEADER}")
    add_custom_command(
      OUTPUT ${CFG_CONFIG}
      COMMAND ${CMAKE_COMMAND} -DINFILE=${ABS_CFG_HEADER} -DOUTFILE=${CFG_CONFIG} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/GenerateLinkerScript.cmake
      DEPENDS ${CFG_HEADER} ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/GenerateLinkerScript.cmake
      COMMENT "Generated nes.cfg from ${CFG_HEADER}"
    )
  endif()

  cmake_path(ABSOLUTE_PATH CFG_CONFIG OUTPUT_VARIABLE ABS_CFG_CONFIG)
  target_sources(${CFG_TARGET} PRIVATE ${ABS_CFG_CONFIG})

  # no longer needed with mesen 2
  # include(DeduplicateDebugSymbols)
  # deduplicate_debug_symbols(TARGET ${CFG_TARGET} FILE $<TARGET_FILE_DIR:${CFG_TARGET}>/${CFG_TARGET}.dbg)

  target_link_options(${CFG_TARGET} PRIVATE
    -C ${ABS_CFG_CONFIG}
    -m $<TARGET_FILE_DIR:${CFG_TARGET}>/map.txt
    --ld-args --dbgfile,$<TARGET_FILE_DIR:${CFG_TARGET}>/${CFG_TARGET}.dbg
  )
endfunction()

if(CMAKE_SCRIPT_MODE_FILE)
  message(STATUS "generate linker script file: ${INFILE}")
  generate_linker_script(SRC ${INFILE} DEST ${OUTFILE})
endif()

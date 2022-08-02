cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME smb1base)

project(${PROJECT_NAME} ASM C)

add_executable(${PROJECT_NAME}
  inc/charmap.inc
  inc/common.inc
  inc/nes2header.inc
  inc/variables.inc

  src/chr.s
  src/main.s
  src/nesheader.s
  src/smb.s

  src/nes.cfg
)

target_include_directories(${PROJECT_NAME} PUBLIC inc/)

target_link_options(${PROJECT_NAME} PRIVATE
  -C ${CMAKE_CURRENT_SOURCE_DIR}/src/nes.cfg
  -m $<TARGET_FILE_DIR:${PROJECT_NAME}>/map.txt
  --ld-args --dbgfile,$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.dbg
)
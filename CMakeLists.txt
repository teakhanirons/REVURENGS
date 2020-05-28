cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{DOLCESDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{DOLCESDK}/share/dolce.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define DOLCESDK to point to your SDK path!")
  endif()
endif()

include("$ENV{DOLCESDK}/share/dolce.cmake" REQUIRED)

project(REVURENGS LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3 -std=c99")

set(ELF "${PROJECT_NAME}.elf")
set(SELF "${PROJECT_NAME}.self")
set(VPK "${PROJECT_NAME}.vpk")

set(TITLE_NAME "REVURENGS")
set(TITLE_ID "REVURENGS")
set(TITLE_VER "01.00")

add_executable("${ELF}"
  main.c
)

target_link_libraries("${ELF}"
  fnblit
  SceCtrl_stub
  SceDisplay_stub
  SceLibKernel_stub
)

dolce_create_self("${SELF}"
  "${ELF}"
)

dolce_create_vpk("${VPK}" "${TITLE_ID}" "${SELF}"
  VERSION "${TITLE_VER}"
  NAME "${TITLE_NAME}"
  FILE icon0.png sce_sys/icon0.png
  FILE font.sfn font.sfn
)

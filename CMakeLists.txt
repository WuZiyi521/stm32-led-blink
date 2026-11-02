cmake_minimum_required(VERSION 3.20)

# 强制 ARM 交叉编译（CLion 默认会挑 MinGW，编不了 Cortex-M）
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER   "D:/Clion/arm/bin/arm-none-eabi-gcc.exe" CACHE FILEPATH "" FORCE)
set(CMAKE_ASM_COMPILER "D:/Clion/arm/bin/arm-none-eabi-gcc.exe" CACHE FILEPATH "" FORCE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(led3 C ASM)

set(CMAKE_C_STANDARD 11)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)

add_executable(led3.elf Core/Src/main.c startup.c)
target_include_directories(led3.elf PRIVATE Core/Inc)
target_compile_options(led3.elf PRIVATE -mcpu=cortex-m3 -mthumb -Os -Wall -ffunction-sections -fdata-sections)
target_link_options(led3.elf PRIVATE -mcpu=cortex-m3 -mthumb -nostdlib -nostartfiles -Wl,--gc-sections -T ${LINKER_SCRIPT})

add_custom_command(TARGET led3.elf POST_BUILD
    COMMAND D:/Clion/arm/bin/arm-none-eabi-objcopy.exe -O binary ${CMAKE_CURRENT_BINARY_DIR}/led3.elf ${CMAKE_CURRENT_BINARY_DIR}/led3.bin
    COMMENT "objcopy -> led3.bin")

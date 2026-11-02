set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_DIR "D:/Clion/arm/bin")
set(CMAKE_C_COMPILER   "${TOOLCHAIN_DIR}/arm-none-eabi-gcc.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_DIR}/arm-none-eabi-gcc.exe")
set(CMAKE_OBJCOPY      "${TOOLCHAIN_DIR}/arm-none-eabi-objcopy.exe")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

add_executable(PhoneHush)

target_sources(PhoneHush PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
  ${CMAKE_CURRENT_LIST_DIR}/GC9A01A.cpp
  ${CMAKE_CURRENT_LIST_DIR}/CST816S.cpp
)

 # print memory usage, enable all warnings
target_link_options(PhoneHush PRIVATE -Xlinker --print-memory-usage)
target_compile_options(PhoneHush PRIVATE -Wall -Wextra)
# Make sure TinyUSB can find tusb_config.h
target_include_directories(PhoneHush PUBLIC
  ${CMAKE_CURRENT_LIST_DIR})

pico_enable_stdio_usb(PhoneHush 0)
pico_enable_stdio_uart(PhoneHush 0)
# Import pico_stdlib which imports commonly used features
target_link_libraries(PhoneHush PUBLIC pico_stdlib pico_unique_id tinyusb_device
        tinyusb_board pico_multicore hardware_pio hardware_spi hardware_i2c hardware_dma hardware_pwm)

# Create map/bin/hex file etc.
pico_add_extra_outputs(PhoneHush)

# Have a command to flash
add_custom_target(flash
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/reset.py /dev/tty.usbmodem1102
    COMMAND sleep 1
    COMMAND picotool load PhoneHush.elf
    COMMAND picotool reboot
    DEPENDS ${MAIN_BIN_FILE}
)

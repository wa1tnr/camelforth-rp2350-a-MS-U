# add_compile_options(-DNOT_WOKWI -v -Werror -Wall -Wextra -Wnull-dereference -fmax-errors=8)
# add_compile_options(-DNOT_WOKWI -v -Wall -Wextra -Wnull-dereference -fmax-errors=8)
# commented 15 Nov 2024 add_compile_options(-DNOT_WOKWI    -Wall -Wextra -Wnull-dereference -fmax-errors=8)

# immediate candidate for restore 15 Nov 20:14z # add_compile_options(-DNOT_WOKWI -Wall -Wextra -Wnull-dereference -fmax-errors=3)
add_executable(camelforth-b
        camelforth-b.c
        )

# Sat Feb 27 23:45:07 UTC 2021

target_compile_definitions(camelforth-b PRIVATE
    # enable this flag only for no_flash compile:
    # NO_FLASH_CMAKE=-1
    # NO_FLASH_CMAKE=-1
)

# 15 Nov 2024
target_link_libraries(camelforth-b PRIVATE forth pico-LED asmword ws2812 hardware_flash pico_stdlib)

# pico_set_binary_type(camelforth-b copy_to_ram)
pico_set_binary_type(camelforth-b copy_to_ram)
# pico_set_binary_type(camelforth-b no_flash)
# pico_set_binary_type(camelforth-b no_flash)

pico_enable_stdio_usb(camelforth-b 1)
pico_enable_stdio_uart(camelforth-b 1) # was zero

pico_add_extra_outputs(camelforth-b)

example_auto_set_url(camelforth-b)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # add_compile_options(-Wno-maybe-uninitialized -DNOT_WOKWI -v -Wall -Wextra -Wnull-dereference -fmax-errors=4)
    add_compile_options(-Wno-maybe-uninitialized -DNOT_WOKWI -Wall -Wextra -Wnull-dereference -fmax-errors=4)
    # add_compile_options(-Wno-maybe-uninitialized)
    # -Werror
endif()

add_subdirectory(forth)
add_subdirectory(pico-hw)
add_subdirectory(pio)
add_subdirectory(asm)

file(GLOB_RECURSE YKUSH_SOURCES
    "*.cpp"
)

add_executable(ykushcmd ${YKUSH_SOURCES})

# --------------------------------------
# Add all source directories as include paths
# --------------------------------------
target_include_directories(ykushcmd PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}             # src/
    ${CMAKE_CURRENT_SOURCE_DIR}/ykush       # src/ykush/
    ${CMAKE_CURRENT_SOURCE_DIR}/ykush2      # src/ykush/
    ${CMAKE_CURRENT_SOURCE_DIR}/ykush3      # src/ykush/
    ${CMAKE_CURRENT_SOURCE_DIR}/ykushxs		# src/ykush/
    ${CMAKE_CURRENT_SOURCE_DIR}/help        # src/help/
    ${CMAKE_CURRENT_SOURCE_DIR}/utils       # src/utils/
)

# --------------------------------------
# Link HIDAPI (official CMake target from subtree)
# --------------------------------------
target_link_libraries(ykushcmd PRIVATE hidapi::hidapi)

# --------------------------------------
# libusb support (if needed)
# --------------------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

target_include_directories(ykushcmd PRIVATE
    ${LIBUSB_INCLUDE_DIRS}
)

target_link_libraries(ykushcmd PRIVATE
    ${LIBUSB_LIBRARIES}
)

# --------------------------------------
# Warnings
# --------------------------------------
if(MSVC)
    target_compile_options(ykushcmd PRIVATE /W3)
else()
    target_compile_options(ykushcmd PRIVATE -Wall -Wextra -Wpedantic)
endif()


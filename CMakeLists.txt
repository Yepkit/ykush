cmake_minimum_required(VERSION 3.15)

project(ykushcmd
    VERSION 1.0.0
    LANGUAGES C CXX
)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------
# HIDAPI configuration
# ---------------------------
set(HIDAPI_BUILD_SHARED ON  CACHE BOOL "" FORCE)
set(HIDAPI_BUILD_STATIC OFF CACHE BOOL "" FORCE)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(HIDAPI_WITH_HIDRAW ON  CACHE BOOL "" FORCE)
    set(HIDAPI_WITH_LIBUSB OFF CACHE BOOL "" FORCE)
endif()

add_subdirectory(hidapi)
add_subdirectory(src)

# ---------------------------
# Uninstall target
# ---------------------------
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY
)

add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# ---------------------------
# CPack packaging
# ---------------------------
set(CPACK_PACKAGE_NAME "ykushcmd")
set(CPACK_PACKAGE_VENDOR "Yepkit")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "support@yepkit.com")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "YKUSH Command Utility")
    set(CPACK_NSIS_PACKAGE_NAME "ykushcmd")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
else()
    set(CPACK_GENERATOR "TGZ;DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Yepkit")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libhidapi-hidraw0")
endif()

include(CPack)


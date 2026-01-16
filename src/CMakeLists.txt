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

target_include_directories(ykushcmd PRIVATE
    ${PROJECT_SOURCE_DIR}/hidapi/hidapi
)

# --------------------------------------
# Link HIDAPI (official CMake target from subtree)
# --------------------------------------
target_link_libraries(ykushcmd PRIVATE hidapi::hidapi)


# ---------------------------
# Install rules
# ---------------------------
install(TARGETS ykushcmd
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)




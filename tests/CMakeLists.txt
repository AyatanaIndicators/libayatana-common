find_package(GMock)

# gschemas.compiled

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "gschemas.compiled")
set_source_files_properties("gschemas.compiled" GENERATED)
execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} gio-2.0 --variable glib_compile_schemas OUTPUT_VARIABLE COMPILE_SCHEMA_EXECUTABLE OUTPUT_STRIP_TRAILING_WHITESPACE)

add_custom_command(
    OUTPUT "gschemas.compiled"
    DEPENDS "${CMAKE_BINARY_DIR}/data/org.ayatana.common.gschema.xml"
    COMMAND cp -f "${CMAKE_BINARY_DIR}/data/org.ayatana.common.gschema.xml" "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND ${COMPILE_SCHEMA_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target("gschemas-compiled" ALL DEPENDS "gschemas.compiled")

# tst_utils

add_executable(tst_utils tst_utils.cpp)
target_compile_definitions(tst_utils PUBLIC SCHEMA_DIR="${CMAKE_CURRENT_BINARY_DIR}")
target_link_options(tst_utils PRIVATE -no-pie)
add_dependencies(tst_utils "gschemas-compiled")
target_include_directories(tst_utils PUBLIC "${CMAKE_SOURCE_DIR}/src")

target_link_libraries(tst_utils
    ayatana-common
    ${GLIB_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_LIBRARIES}
)

add_test(TstUtils tst_utils)

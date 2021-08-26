
add_library(ayatana-common SHARED
    utils.c
)

target_link_libraries(ayatana-common
    ${GLIB_LIBRARIES}
    ${URLDISPATCHER_LIBRARIES}
)

if(URLDISPATCHER_FOUND)
    add_definitions( -DHAS_URLDISPATCHER )
endif()

set(PUBLIC_HEADERS
    utils.h
)

set_target_properties(ayatana-common
    PROPERTIES
    VERSION ${API_VERSION}.0.0
    SOVERSION ${ABI_VERSION}
)

configure_file(libayatana-common.pc.in ${CMAKE_CURRENT_BINARY_DIR}/libayatana-common.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libayatana-common.pc DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig)
install(FILES ${PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/ayatana/common)
install(TARGETS ayatana-common DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

# AyatanaCommon.gir

find_package(GObjectIntrospection REQUIRED QUIET)

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.gir"
    DEPENDS "ayatana-common"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND
    ${INTROSPECTION_SCANNER}
    utils.c utils.h
    --namespace=AyatanaCommon
    --c-include=ayatana/common/utils.h
    --quiet
    --warn-all
    --include=GLib-2.0
    --library="ayatana-common"
    --library-path=${CMAKE_CURRENT_BINARY_DIR}
    --output "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.gir"
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.gir" DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/gir-1.0")

# AyatanaCommon.vapi

find_package(Vala REQUIRED QUIET)

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.vapi"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.gir"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND
    ${VAPI_GEN}
    --library=AyatanaCommon
    AyatanaCommon.gir
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.vapi" DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/vala/vapi")

add_custom_target("src" ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.vapi")

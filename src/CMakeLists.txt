
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

if (INTROSPECTION_FOUND)
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon-0.0.gir"
        DEPENDS "ayatana-common"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND
        ${INTROSPECTION_SCANNER}
        utils.c utils.h
        --no-libtool
        --namespace=AyatanaCommon
        --nsversion=0.0
        --c-include=ayatana/common/utils.h
        --quiet
        --warn-all
        --include=GLib-2.0
        --library="ayatana-common"
        --library-path=${CMAKE_CURRENT_BINARY_DIR}
        --output "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon-0.0.gir"
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon-0.0.gir" DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/gir-1.0")

# AyatanaCommon.typelib

    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon-0.0.typelib"
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon-0.0.gir"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND
        ${INTROSPECTION_COMPILER}
        --includedir=${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon-0.0.gir
        -o "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon-0.0.typelib"
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon-0.0.typelib" DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/girepository-1.0")

# AyatanaCommon.vapi

    if (VALA_COMPILER)
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.vapi"
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon-0.0.typelib"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMAND
            ${VAPI_GEN}
            --library=AyatanaCommon
            AyatanaCommon-0.0.gir
        )

        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.vapi" DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/vala/vapi")

        add_custom_target("src" ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/AyatanaCommon.vapi")

    endif()

endif()

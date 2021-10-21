# org.ayatana.common.gschema.xml

find_package(Intltool REQUIRED)
set(ENV{LC_ALL} "C")
intltool_merge_translations("${CMAKE_CURRENT_SOURCE_DIR}/org.ayatana.common.gschema.xml.in" "${CMAKE_CURRENT_BINARY_DIR}/org.ayatana.common.gschema.xml" ALL UTF8 STYLE "xml" NO_TRANSLATIONS)
find_package(GSettings REQUIRED)
set(GSETTINGS_COMPILE ON)
add_schema("org.ayatana.common.gschema.xml")

# ayatana-indicators.target

pkg_check_modules(SYSTEMD systemd)

if (${SYSTEMD_FOUND})
    if (${SYSTEMD_VERSION} VERSION_LESS 247)

        # This bit now becomes really hacky-whacky...

        # As systemd is a build-requirement, there should be the
        # systemd unit directory available on the build host, so
        # let's look for that...

        if (EXISTS "/usr/lib/systemd/user")
            # Let's try /usr/lib/ ...
            set(SYSTEMD_USER_UNIT_DIR "/usr/lib/systemd/user")
        elseif (EXISTS "/usr/lib64/systemd/user")
            # Let's try /usr/lib64/ ...
            set(SYSTEMD_USER_UNIT_DIR "/usr/lib64/systemd/user")
        else()
            # Fallback to something that fails on Debian and Ubuntu
            # but might work on other distributions...
            set(SYSTEMD_USER_UNIT_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/systemd/user")
        endif()

    else()
        pkg_get_variable(SYSTEMD_USER_UNIT_DIR systemd systemd_user_unit_dir)
    endif()
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/ayatana-indicators.target" DESTINATION "${SYSTEMD_USER_UNIT_DIR}")
endif()

cmake_minimum_required (VERSION 3.13)
project (libayatana-common VERSION "0.9.11" LANGUAGES C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(GETTEXT_PACKAGE "ayatana-common")
set(API_VERSION 0)
set(ABI_VERSION 0)

# Options
option(ENABLE_TESTS "Enable all tests and checks" OFF)
option(ENABLE_COVERAGE "Enable coverage reports (includes enabling all tests and checks)" OFF)
option(ENABLE_WERROR "Treat all build warnings as errors" OFF)
option(ENABLE_LOMIRI_FEATURES "Build with Lomiri-specific libraries, schemas and media" OFF)

if(ENABLE_COVERAGE)
    set(ENABLE_TESTS ON)
    set(CMAKE_BUILD_TYPE "Coverage")
else()
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(ENABLE_WERROR)
    add_definitions("-Werror")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions("-Weverything")
else()
    add_definitions("-Wall")
endif()

# GNU standard installation directories
include (GNUInstallDirs)

# Check for prerequisites
find_package (PkgConfig REQUIRED)
include (FindPkgConfig)

set (
    DEPS
    glib-2.0>=2.36
    gio-2.0>=2.36
)

if (ENABLE_LOMIRI_FEATURES)
    list (
        APPEND
        DEPS
        lomiri-url-dispatcher>=0
    )
endif ()

pkg_check_modules (DEPS REQUIRED ${DEPS})
include_directories (${DEPS_INCLUDE_DIRS})

# for GIR and Vala bindings
find_package (GObjectIntrospection QUIET)
find_package (Vala QUIET)

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(po)

if (ENABLE_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
    if (ENABLE_COVERAGE)
        find_package(CoverageReport)
        ENABLE_COVERAGE_REPORT(
            TARGETS "ayatana-common"
            TESTS "tst_utils" "tst_utils_vala"
            FILTER /usr/include ${CMAKE_BINARY_DIR}/*
        )
    endif()
endif()

# Display config info

message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Unit tests: ${ENABLE_TESTS}")
message(STATUS "Build with -Werror: ${ENABLE_WERROR}")

if (INTROSPECTION_FOUND)
    message(STATUS "Build GObjectIntrospection: YES")
    if (VALA_COMPILER)
        message(STATUS "Build Vala bindings: YES")
    else()
        message(STATUS "Build Vala bindings: NO")
    endif()
else()
    message(STATUS "Build GObjectIntrospection: NO")
endif()

message(STATUS "Build with Lomiri features: ${ENABLE_LOMIRI_FEATURES}")

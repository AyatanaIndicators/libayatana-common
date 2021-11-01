project (libayatana-common C CXX)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(PROJECT_VERSION "0.9.5")
set(PACKAGE ${CMAKE_PROJECT_NAME})
set(GETTEXT_PACKAGE "ayatana-common")
set(API_VERSION 0)
set(ABI_VERSION 0)

# Options
option(ENABLE_TESTS "Enable all tests and checks" OFF)
option(ENABLE_COVERAGE "Enable coverage reports (includes enabling all tests and checks)" OFF)
option(ENABLE_WERROR "Treat all build warnings as errors" OFF)

if(ENABLE_COVERAGE)
    set(ENABLE_TESTS ON)
    set(CMAKE_BUILD_TYPE "Coverage")
else()
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(ENABLE_WERROR)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

# GNU standard installation directories
include (GNUInstallDirs)
set (CMAKE_INSTALL_PKGLIBEXECDIR "${CMAKE_INSTALL_LIBEXECDIR}/${CMAKE_PROJECT_NAME}")
set (CMAKE_INSTALL_FULL_PKGLIBEXECDIR "${CMAKE_INSTALL_FULL_LIBEXECDIR}/${CMAKE_PROJECT_NAME}")

# Check for prerequisites
find_package (PkgConfig REQUIRED)
include (FindPkgConfig)

pkg_check_modules(GLIB REQUIRED
    glib-2.0>=2.36
    gio-2.0>=2.36
)
include_directories (${GLIB_INCLUDE_DIRS})

pkg_check_modules(
  URLDISPATCHER
  lomiri-url-dispatcher>=0
)
include_directories(${URLDISPATCHER_INCLUDE_DIRS})

# for GIR and Vala bindings
find_package (GObjectIntrospection QUIET)
find_package (Vala QUIET)

set(CC_WARNING_ARGS " -Wall -pedantic -Wextra -Wno-missing-field-initializers")

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
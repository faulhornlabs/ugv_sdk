cmake_minimum_required(VERSION 3.5.1)
project(ugv_sdk VERSION 0.1.5)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Found ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

option(BUILD_APPS "Build applications" ON)

## Generate symbols for IDE indexer
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Set compiler to use c++ 11 features
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Chosse build type
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE ${default_build_type})
endif()

if (NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX _d)
endif()
if (NOT CMAKE_RELWITHDEBINFO_POSTFIX)
    set(CMAKE_RELWITHDEBINFO_POSTFIX _rd)
endif()


## Use GNUInstallDirs to install libraries into correct locations on all platforms.
include(GNUInstallDirs)
message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX} with 'make install'")

# Build libraries
find_package(Threads REQUIRED)
find_package(wrp_io REQUIRED CONFIG)

add_library(${PROJECT_NAME} 
    src/mobile_base.cpp
    src/hunter_base.cpp
    src/hunter_can_parser.c
    src/scout_base.cpp
    src/scout_can_parser.c
    src/scout_uart_parser.c
    src/tracer_base.cpp
    src/tracer_can_parser.c
    src/tracer_uart_parser.c
    src/bunker_base.cpp
    src/bunker_can_parser.c
)
add_library(westonrobot::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} westonrobot::wrp_io Threads::Threads)

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
    PRIVATE src)
    
# Build apps
# Check wether to build monitor tool
set(BUILD_MONITOR ON)
find_package(Curses QUIET)
if(BUILD_MONITOR AND NOT CURSES_FOUND)
    set(BUILD_MONITOR OFF)
    message(STATUS "Monitor app will not be built due to missing ncurses library, try: sudo apt install libncurses5-dev")
endif()

# add app source directory
if (BUILD_APPS)
    add_subdirectory(apps)
endif()

# targets to install
install(TARGETS ${PROJECT_NAME}
    EXPORT ugv_sdkTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)

install(DIRECTORY include/ugv_sdk
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# export target configuration 
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ugv_sdkConfigVersion.cmake
    VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
    COMPATIBILITY SameMajorVersion)

install(EXPORT ugv_sdkTargets
    FILE ugv_sdkTargets.cmake
    NAMESPACE westonrobot::
    DESTINATION lib/cmake/ugv_sdk)

configure_file(cmake/ugv_sdkConfig.cmake.in ugv_sdkConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ugv_sdkConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/ugv_sdkConfigVersion.cmake"
        DESTINATION lib/cmake/ugv_sdk)

# Packaging support
set(CPACK_PACKAGE_VENDOR "Weston Robot")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Weston Robot Platform - SDK")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/weston_robot")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
# set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruixiang Du (ruixiang.du@westonrobot.com)") 
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "libasio-dev")
set(CPACK_SOURCE_IGNORE_FILES .git dist .*build.* /\\\\.DS_Store)
include(CPack)

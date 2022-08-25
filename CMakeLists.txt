# Copyright (C) 2015 Florian Franzen
project(neurosuite CXX)
cmake_minimum_required(VERSION 2.8.12)

set(PROJECT_VERSION_MAJOR 2)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

# Options
option(WITH_QT4
       "Enable to build against Qt4 (default: Qt5)"
       OFF)

option(WITH_TEST
       "Build scrollarea test binary"
       OFF)

##############################
# Configure build environment #
###############################
# Enable exports for library build
add_definitions(-DNEUROSUITE_BUILD_NEUROSUITE_LIB)

if(APPLE)
   # Enable RPATH on OS X (default in CMake >= 3.0)
   set(CMAKE_MACOSX_RPATH ON)
endif(APPLE)

# Configure CMAKE
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#####################
# Find dependencies #
#####################
# Configure Qt functionality
set(CMAKE_AUTOMOC ON)
add_definitions(-DQT_USE_QSTRINGBUILDER)

# Find correct Qt library
if(WITH_QT4)
    find_package(Qt4 4.8.0 REQUIRED QtGui QtWebKit)

    if(APPLE)
        # The QStandardPaths port needs these on OS X
        find_library(CF_LIBRARIES NAMES CoreFoundation)
        find_library(CS_LIBRARIES NAMES CoreServices)
    endif()
else()
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5PrintSupport REQUIRED)
    find_package(Qt5WebKitWidgets REQUIRED)
endif()

if (MSVC)
   # Qt disables the native wchar_t type, do it too to avoid linking issues
   #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zc:wchar_t-" )
endif()

########################
# Include subdirectory #
########################
# Build main library
add_subdirectory(src)

# Build test binary
if(WITH_TEST)
    add_subdirectory(test)
endif()

##############################
# Create CMake package files #
##############################
set(CMAKE_PACKAGE_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/cmake")
set(CMAKE_MODULE_BUILD_DIR "${CMAKE_PACKAGE_BUILD_DIR}/modules")

# Create targets file
export(TARGETS ${PROJECT_NAME}
       FILE "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}Targets.cmake"
)

# Copy config file
configure_file(cmake/PackageConfig.cmake
    "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}Config.cmake"
    @ONLY
)

# Create version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Copy module files
foreach(CMAKE_FILE  DeployNeurosuite DeployQt5 PackNeurosuite)
    configure_file(cmake/modules/${CMAKE_FILE}.cmake
                   "${CMAKE_MODULE_BUILD_DIR}/${CMAKE_FILE}.cmake"
                   COPYONLY)
endforeach()

###############################
# Install CMake package files #
###############################
if(WIN32)
    set(CMAKE_PACKAGE_INSTALL_DIR "CMake")
else()
    set(CMAKE_PACKAGE_INSTALL_DIR "lib/cmake/${PROJECT_NAME}")
endif()
set(CMAKE_MODULE_INSTALL_DIR "${CMAKE_PACKAGE_INSTALL_DIR}/modules")

# Install config and version
install(FILES "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}Config.cmake"
              "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_PACKAGE_INSTALL_DIR}
)

# Install target
install(EXPORT LibraryTargets
        DESTINATION ${CMAKE_PACKAGE_INSTALL_DIR}
        FILE "${PROJECT_NAME}Targets.cmake")

# Install module files
file(GLOB MODULE_FILES "${CMAKE_MODULE_BUILD_DIR}/*.cmake")
install(FILES ${MODULE_FILES}
        DESTINATION ${CMAKE_MODULE_INSTALL_DIR})

######################
# CPack Configuriton #
######################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}/cmake/modules")
include(PackNeurosuite)
# Init CPack variables
neurosuite_cpack_init("lib${PROJECT_NAME}"
                      ${PROJECT_VERSION}
                      "Florian Franzen <FlorianFranzen@gmail.com"
                      "Library for shared functionality of Klusters, Neuroscope and NDManager"
                      OFF)

# Use deb and rpm on linux
if(WITH_QT4)
    neurosuite_cpack_ubuntu(
        "libqtgui4, libqt4-webkit4"
        OFF
    )
    neurosuite_cpack_suse(
        "libqt4, libqt4-x11, libQtWebKit4"
        OFF
    )
    neurosuite_cpack_fedora(
        "qt, qt-mobility, qtwebkit, qt-x11"
        OFF
    )
    neurosuite_cpack_scientific(
        "libQtCore, libQtGui, libQtNetwork, libQtWebKit"
        OFF
    )
else()
    neurosuite_cpack_ubuntu(
        "libqt5widgets5, libqt5printsupport5, libqt5webkit5"
        OFF
    )
endif()

include(CPack)

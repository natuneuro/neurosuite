# Find packages of dependencies
include(CMakeFindDependencyMacro)

set(@PROJECT_NAME@_WITH_QT4 @WITH_QT4@)
if(@PROJECT_NAME@_WITH_QT4)
    find_dependency(Qt4 4.8.0)
else()
    find_dependency(Qt5Widgets)
    find_dependency(Qt5PrintSupport)
    find_dependency(Qt5WebKitWidgets)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

# Include target definition (includes and definition)
include("${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@Targets.cmake")

###################
# Build library #
###################
include_directories(gui/page)

set(SOURCE_FILES
    gui/qrecentfileaction.cpp
    gui/qcolorbutton.cpp
    gui/page/qpagedialog.cpp
    gui/page/qpagemodel.cpp
    gui/page/qpageview.cpp
    gui/page/qpageview_p.cpp
    gui/page/qpagewidget.cpp
    gui/page/qpagewidgetmodel.cpp
    gui/page/qextenddialog.cpp
    gui/page/klusterseparator.cpp
    gui/dockarea.cpp
    gui/qextendtabwidget.cpp
    gui/qhelpviewer.cpp
    gui/klusterrubberband.cpp
    shared/channelcolors.cpp
    shared/itemcolors.cpp
    shared/zoomwindow.cpp
    shared/utilities.cpp
)

# Add qstandardpaths and it os specific interface if qt5 is not available
if(WITH_QT4)
    list(APPEND SOURCE_FILES libinqt5/qstandardpaths.cpp)
    include_directories(libinqt5)

    if (UNIX)
        if(APPLE)
            list(APPEND SOURCE_FILES
                 libinqt5/qstandardpaths_mac.cpp)
        else(APPLE)
            list(APPEND SOURCE_FILES
                 libinqt5/qstandardpaths_unix.cpp)
        endif()
    endif()

    if (WIN32)
        list(APPEND SOURCE_FILES
             libinqt5/qstandardpaths_win.cpp)
    endif()
endif()

# Add ressource files
set(RESSOURCE_FILES
    gui/libneurosuite-icons.qrc
    gui/libneurosuite-cursors.qrc
)

if(WITH_QT4)
    qt4_add_resources(SOURCE_FILES ${RESSOURCE_FILES})
else()
    qt5_add_resources(SOURCE_FILES ${RESSOURCE_FILES})
endif()

# Run compiler
add_library (${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME}
    PROPERTIES CXX_VISIBILITY_PRESET hidden
)

################
# Link library #
################
if(WITH_QT4)
    target_link_libraries(${PROJECT_NAME} Qt4::QtGui Qt4::QtWebKit)

    if(APPLE)
        # Or QStandardPaths port needs these on OS X
        target_link_libraries(${PROJECT_NAME} ${CF_LIBRARIES} ${CS_LIBRARIES})
    endif()
else()
    target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::PrintSupport Qt5::WebKitWidgets)
endif()

###################
# Install library #
###################
# Group headers in subfolder
set(INCLUDE_FILE_DESTINATION "include/${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME}
        EXPORT LibraryTargets
        DESTINATION lib
        INCLUDES DESTINATION ${INCLUDE_FILE_DESTINATION})

set (HEADER_FILES
     libneurosuite_export.h
     gui/qrecentfileaction.h
     gui/qcolorbutton.h
     gui/page/qpagedialog.h
     gui/page/qextenddialog.h
     gui/page/qpagewidget.h
     gui/page/qpagemodel.h
     gui/page/qpageview.h
     gui/page/qpagewidgetmodel.h
     gui/dockarea.h
     gui/qextendtabwidget.h
     gui/qhelpviewer.h
     gui/klusterrubberband.h
     shared/channelcolors.h
     shared/itemcolors.h
     shared/types.h
     shared/array.h
     shared/utilities.h
     shared/zoomwindow.h
)

install(FILES ${HEADER_FILES}
        DESTINATION ${INCLUDE_FILE_DESTINATION})

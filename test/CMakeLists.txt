set(TEST_SOURCE_FILES
    main.cpp
    listview.cpp
    mainview.cpp
    scrollarea.cpp
    itemgroupview.cpp
)

add_executable(test_scrollview ${TEST_SOURCE_FILES})

if(WITH_QT4)
    target_link_libraries(test_scrollview Qt4::QtGui)
else()
    target_link_libraries(test_scrollview Qt5::Widgets)
endif()

install(TARGETS test_scrollview
        DESTINATION bin
)

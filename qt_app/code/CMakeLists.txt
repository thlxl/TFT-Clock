cmake_minimum_required(VERSION 3.5)

project(tft_clock VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加mqtt包含目录
include_directories(C:/mqtt/paho-cpp/include C:/mqtt/paho-c/include)

# 添加matt静态库路径
link_directories(C:/mqtt/paho-cpp/lib C:/mqtt/paho-c/lib)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        mqttclient.cpp
        mqttclient.h
        mqttworker.h
        mqttworker.cpp
        settingsdialog.h
        settingsdialog.cpp
        lcd_number.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(tft_clock
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc
        mylabel.h mylabel.cpp
        colorgradientslider.h colorgradientslider.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET tft_clock APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(tft_clock SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(tft_clock
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(tft_clock PRIVATE Qt${QT_VERSION_MAJOR}::Widgets 
                        paho-mqtt3a-static 
                        paho-mqtt3c-static 
                        paho-mqttpp3-static
                        Ws2_32)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.tft_clock)
endif()
set_target_properties(tft_clock PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS tft_clock
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(tft_clock)
endif()

if(WIN32)
    set_target_properties(tft_clock PROPERTIES
                          WIN32_EXECUTABLE OFF)
endif()

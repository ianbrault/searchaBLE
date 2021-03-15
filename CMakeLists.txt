cmake_minimum_required(VERSION 3.5)

project(searchaBLE LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the OS X app icon
# TODO: do this for other platforms as well
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE logo.icon.icns)
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/logo.icon.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Bluetooth REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Bluetooth REQUIRED)

set(PROJECT_SOURCES
        src/bt_utils.cpp
        src/bt_utils.h
        src/main.cpp
        src/main_window.cpp
        src/main_window.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(searchaBLE
        ${PROJECT_SOURCES}
        ${APP_ICON_MACOSX}
    )
else()
    if(ANDROID)
        add_library(searchaBLE SHARED
            ${PROJECT_SOURCES}
        )
    elseif(APPLE)
        add_executable(searchaBLE
            MACOSX_BUNDLE
            ${PROJECT_SOURCES}
            ${APP_ICON_MACOSX}
        )
    else()
        add_executable(searchaBLE
            ${PROJECT_SOURCES}
        )
    endif()
endif()

add_subdirectory(fmt)
target_link_libraries(searchaBLE PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Bluetooth fmt::fmt)

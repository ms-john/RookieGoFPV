cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME RookieGoFPV)
set(APP_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(PROJECT_VERSION 1.0)
set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Quick Widgets Gui)

# pkgconfig
find_package(PkgConfig REQUIRED)
# gstreamer
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-video-1.0 gstreamer-app-1.0)
# libusb
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
# libsodium
pkg_check_modules(LIBSODIUM REQUIRED IMPORTED_TARGET libsodium)

include_directories(
    ${GSTREAMER_INCLUDE_DIRS}
    ${LIBUSB_INCLUDE_DIRS}
    ${LIBPCAP_HEADERS}
    rtl8812/src
    rtl8812/hal
)

file(GLOB_RECURSE WFB_NG_SOURCES
    src/wfb-ng/*.h
    src/wfb-ng/*.c
    src/wfb-ng/*.cpp
)

include(monitor.cmake)

set(PROJECT_SOURCES
    resource.qrc
    src/main.cpp
    src/player/GstPlayerWidget.h
    src/player/GstPlayerWidget.cpp
    ${WFB_NG_SOURCES}
    src/wfb_ng_middleware.cpp
    src/wfb_ng_middleware.h
    src/wfb_api.h
    src/wfb_api.cpp
    src/wfb-ng/wifibroadcast.h
)

if (6 GREATER_EQUAL 6)
  qt_add_executable(${APP_NAME}
      ${PROJECT_SOURCES}
  )
  # Define target properties for Android with Qt 6 as:
  # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
  if (ANDROID)
    add_library(${APP_NAME} SHARED
        ${PROJECT_SOURCES}
    )
  else ()
    add_executable(${APP_NAME}
        ${PROJECT_SOURCES}
    )
  endif ()
endif ()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RookieGoFPV
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_directories(
    ${APP_NAME}
    PRIVATE
    ${GSTREAMER_LIBRARY_DIRS}
    ${LIBUSB_LIBRARY_DIRS}
    ${LIBSOIDUM_LIBRARY_DIRS}
)

target_link_libraries(${APP_NAME}
    PRIVATE Qt6::Quick
    Qt6::Widgets
    Qt6::Gui
    ${GSTREAMER_LIBRARIES}
    ${LIBUSB_LIBRARIES}
    Rtl8812auNet
    PkgConfig::LIBSODIUM
)

include(GNUInstallDirs)
install(TARGETS ${APP_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

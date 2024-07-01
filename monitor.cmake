cmake_minimum_required(VERSION 3.15)

project(Rtl8812auNet)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBUSB REQUIRED IMPORTED_TARGET libusb-1.0)
set(LIBPCAP_HEADERS /opt/homebrew/opt/libpcap/include)
set(LIBPCAP_LIBS /opt/homebrew/opt/libpcap/lib)

set(LIB_ROOT_PATH ./rtl8812)

add_library(Rtl8812auNet
    SHARED
    ${LIB_ROOT_PATH}/src/logger.h
#    ${LIB_ROOT_PATH}/src/main.cpp

    ${LIB_ROOT_PATH}/hal/Hal8812PhyReg.h
    ${LIB_ROOT_PATH}/hal/Hal8812PwrSeq.c
    ${LIB_ROOT_PATH}/hal/Hal8812PwrSeq.h
    ${LIB_ROOT_PATH}/hal/basic_types.h
    ${LIB_ROOT_PATH}/hal/hal8812a_fw.c
    ${LIB_ROOT_PATH}/hal/hal8812a_fw.h
    ${LIB_ROOT_PATH}/hal/hal_com_reg.h
    ${LIB_ROOT_PATH}/hal/rtl8812a_hal.h
    ${LIB_ROOT_PATH}/hal/rtl8812a_recv.h
    ${LIB_ROOT_PATH}/hal/rtl8812a_spec.h

    ${LIB_ROOT_PATH}/src/EepromManager.cpp
    ${LIB_ROOT_PATH}/src/EepromManager.h
    ${LIB_ROOT_PATH}/src/Firmware.h
    ${LIB_ROOT_PATH}/src/FirmwareManager.cpp
    ${LIB_ROOT_PATH}/src/FirmwareManager.h
    ${LIB_ROOT_PATH}/src/FrameParser.cpp
    ${LIB_ROOT_PATH}/src/FrameParser.h
    ${LIB_ROOT_PATH}/src/HalModule.cpp
    ${LIB_ROOT_PATH}/src/HalModule.h
    ${LIB_ROOT_PATH}/src/ParsedRadioPacket.cpp
    ${LIB_ROOT_PATH}/src/RadioManagementModule.cpp
    ${LIB_ROOT_PATH}/src/RadioManagementModule.h
    ${LIB_ROOT_PATH}/src/Rtl8812aDevice.cpp
    ${LIB_ROOT_PATH}/src/Rtl8812aDevice.h
    ${LIB_ROOT_PATH}/src/RtlUsbAdapter.cpp
    ${LIB_ROOT_PATH}/src/RtlUsbAdapter.h
    ${LIB_ROOT_PATH}/src/SelectedChannel.h
    ${LIB_ROOT_PATH}/src/WiFiDriver.cpp
    ${LIB_ROOT_PATH}/src/WiFiDriver.h
    ${LIB_ROOT_PATH}/src/registry_priv.h
)

target_link_libraries(Rtl8812auNet PRIVATE
    PkgConfig::LIBUSB
    ${LIBPCAP_LIBS}/libpcap.a
)

set_property(TARGET Rtl8812auNet PROPERTY CXX_STANDARD 20)
target_include_directories(Rtl8812auNet PRIVATE  ${LIB_ROOT_PATH}/hal
    ${LIBPCAP_HEADERS}
)

/*
 * WfbngLink.cpp
 * copied from WfbngLink.hpp:https://github.com/gehee/FPVue_android.git
 * thanks to geehe
 */

#include "WfbngLink.hpp"
#include <string>

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <list>
#include <span>

#include "RxFrame.h"
#include "libusb.h"
#include "rtl8812/src/WiFiDriver.h"
#include "rtl8812/src/logger.h"
#include "wfb-ng/src/wifibroadcast.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

// #define TAG "com.geehe.fpvue"

std::string uint8_to_hex_string(const uint8_t *v, const size_t s) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (int i = 0; i < s; i++) {
    ss << std::hex << std::setw(2) << static_cast<int>(v[i]);
  }
  return ss.str();
}

WfbngLink::WfbngLink() {
//  initAgg();
  // 初始化 spdlog logger
  logger = spdlog::stdout_color_mt("console");
  Logger_t log = std::shared_ptr<spdlog::logger>();
  wifi_driver = std::make_unique<WiFiDriver>(log);

}

void WfbngLink::initAgg() {
  spdlog::info("initAgg");
  uint64_t epoch = 0;

  uint32_t video_channel_id_f = (link_id << 8) + video_radio_port;
  video_channel_id_be = htobe32(video_channel_id_f);
  uint32_t mavlink_channel_id_f = (link_id << 8) + mavlink_radio_port;
  mavlink_channel_id_be = htobe32(mavlink_channel_id_f);

  video_aggregator = std::make_unique<Aggregator>(
      client_addr, video_client_port, keyPath, epoch, video_channel_id_f);
  mavlink_aggregator = std::make_unique<Aggregator>(
      client_addr, mavlink_client_port, keyPath, epoch, mavlink_channel_id_f);
}

int WfbngLink::run(int wifiChannel, std::string pid, std::string vid) {
  int r;
  libusb_context *ctx = NULL;

//  r = libusb_set_option(
//      NULL, LIBUSB_OPTION_NO_DEVICE_DISCOVERY); // disable device discovery

  r = libusb_init(&ctx);
  if (r < 0) {
    //    __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to init libusb.");
    logger->error("Failed to init libusb.");

    return r;
  }

  // Open adapters
  struct libusb_device_handle *dev_handle;
  uint16_t u_vid = std::stoi(vid, nullptr, 16);
  uint16_t u_pid = std::stoi(pid, nullptr, 16);
//  logger->debug("vid: {}, pid: {}", u_vid, u_pid);
  dev_handle = libusb_open_device_with_vid_pid(ctx, u_vid, u_pid);
  if (dev_handle == nullptr) {
    libusb_exit(ctx);
    return r;
  }

  /*Check if kernel driver attached*/
  if (libusb_kernel_driver_active(dev_handle, 0)) {
    r = libusb_detach_kernel_driver(dev_handle, 0); // detach driver
    //    __android_log_print(ANDROID_LOG_DEBUG, TAG,
    //                        "libusb_detach_kernel_driver: %d", r);
    logger->debug("libusb_detach_kernel_driver: {}", r);
  }
  r = libusb_claim_interface(dev_handle, 0);
  //  __android_log_print(ANDROID_LOG_DEBUG, TAG,
  //                      "Creating driver and device for fd=%d", fd);
//  logger->debug("Creating driver and device for fd={}", pid);

  rtl_device = wifi_driver->CreateRtlDevice(dev_handle);
  if (!rtl_device) {
    //    __android_log_print(ANDROID_LOG_ERROR, TAG, "CreateRtlDevice error");
    logger->error("CreateRtlDevice error");
    return -1;
  }

  uint8_t *video_channel_id_be8 =
      reinterpret_cast<uint8_t *>(&video_channel_id_be);
  uint8_t *mavlink_channel_id_be8 =
      reinterpret_cast<uint8_t *>(&mavlink_channel_id_be);

  try {
    auto packetProcessor = [this, video_channel_id_be8,
                            mavlink_channel_id_be8](const Packet &packet) {
      RxFrame frame(packet.Data);
      if (!frame.IsValidWfbFrame()) {
        return;
      }
      // TODO(geehe) Get data from libusb?
      int8_t rssi[4] = {1, 1, 1, 1};
      uint32_t freq = 0;
      int8_t noise[4] = {1, 1, 1, 1};
      uint8_t antenna[4] = {1, 1, 1, 1};

      std::lock_guard<std::mutex> lock(agg_mutex);
      if (frame.MatchesChannelID(video_channel_id_be8)) {
        video_aggregator->process_packet(
            packet.Data.data() + sizeof(ieee80211_header),
            packet.Data.size() - sizeof(ieee80211_header) - 4, 0, antenna, rssi,
            noise, freq, 0, 0, NULL);
      } else if (frame.MatchesChannelID(mavlink_channel_id_be8)) {
        mavlink_aggregator->process_packet(
            packet.Data.data() + sizeof(ieee80211_header),
            packet.Data.size() - sizeof(ieee80211_header) - 4, 0, antenna, rssi,
            noise, freq, 0, 0, NULL);
      }
    };
    rtl_device->Init(packetProcessor,
                     SelectedChannel{
                         .Channel = static_cast<uint8_t>(wifiChannel),
                         .ChannelOffset = 0,
                         .ChannelWidth = channelWidth,
                     });
  } catch (const std::runtime_error &error) {
    //    __android_log_print(ANDROID_LOG_ERROR, TAG, "runtime_error: %s",
    //                        error.what());
    logger->error("runtime_error: {}", error.what());
    return -1;
  }

  //  __android_log_print(ANDROID_LOG_DEBUG, TAG, "Init done, releasing...");
  logger->debug("Init done, releasing...");

  r = libusb_release_interface(dev_handle, 0);
  //  __android_log_print(ANDROID_LOG_DEBUG, TAG, "libusb_release_interface:
  //  %d",
  //                      r);
  logger->debug("libusb_release_interface: {}", r);

  libusb_exit(ctx);
  return 0;
}

void WfbngLink::stop() { rtl_device->should_stop = true; }

void WfbngLink::setRtlChannel(uint8_t channel) {
  SelectedChannel selectedChannel{
      .Channel = channel, .ChannelOffset = 0, .ChannelWidth = CHANNEL_WIDTH_20};
  if (rtl_device != nullptr)
    rtl_device->SetMonitorChannel(selectedChannel);
}
// Copyright (c), 2022-2024, Mist Studio.
/*
 * WfbngLink.hpp
 * copied from WfbngLink.hpp:https://github.com/gehee/FPVue_android.git
 * thanks to geehe
 */

#ifndef FPV_VR_WFBNG_LINK_H
#define FPV_VR_WFBNG_LINK_H

#include "rtl8812/src/WiFiDriver.h"
#include "spdlog/spdlog.h"
#include "wfb-ng/src/rx.hpp"
#include <list>
#include <map>

class WfbngLink {
public:
  WfbngLink();
  int run(int wifiChannel, std::string pid, std::string vid);
  void initAgg();
  void stop();

  std::mutex agg_mutex;
  std::unique_ptr<Aggregator> video_aggregator;
  std::unique_ptr<Aggregator> mavlink_aggregator;

  void setVideoClientPort(int port) { video_client_port = port; }
  void setMavlinkClientPort(int port) { mavlink_client_port = port; }
  void setClientAddr(const std::string &addr) { client_addr = addr; }
  void setLinkId(uint32_t id) { link_id = id; }
  void setVideoRadioPort(uint8_t port) { video_radio_port = port; }
  void setMavlinkRadioPort(uint8_t port) { mavlink_radio_port = port; }
  void setKeyPath(const std::string &path) { keyPath = path; }

  void setRtlChannel(uint8_t channel);
  void setRtlChannelWidth(ChannelWidth_t width) { channelWidth = width; }

private:
  std::string keyPath = "/etc/wfb/gs.key";
  std::unique_ptr<WiFiDriver> wifi_driver;
  uint32_t video_channel_id_be;
  uint32_t mavlink_channel_id_be;
  //  std::map<int, std::unique_ptr<Rtl8812aDevice>> rtl_devices;
  std::unique_ptr<Rtl8812aDevice> rtl_device;

  std::shared_ptr<spdlog::logger> logger;

  // TODO(geehe) Get that from the android UI.
  int video_client_port = 5600;
  int mavlink_client_port = 14550;
  std::string client_addr = "127.0.0.1";
  uint32_t link_id = 7669206; // sha1 hash of link_domain="default"
  uint8_t video_radio_port = 0;
  uint8_t mavlink_radio_port = 0x10;

  ChannelWidth_t channelWidth = ChannelWidth_t::CHANNEL_WIDTH_20;
};

#endif // FPV_VR_WFBNG_LINK_H

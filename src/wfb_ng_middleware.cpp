// Copyright (c), 2022-2024, Mist Studio.

//
// Created by John on 24-6-26.
//

#include "wfb_ng_middleware.h"
#include <QDebug>

wfb_ng_middleware::wfb_ng_middleware(QObject *parent) : QObject(parent) {
  m_wfbng_thread = new wfb_ng_thread(this);
  m_wfbng_channel = 0;
  m_wfbng_frame_count = 0;
  m_wfbng_rtp_pkt_count = 0;
  m_wfbng_wifi_frame_count = 0;
  connect(this, &wfb_ng_middleware::startWFBReceiver, this,
          &wfb_ng_middleware::onStartWFBReceiver);
}
wfb_ng_middleware::~wfb_ng_middleware() {}

bool wfb_ng_middleware::start(const QString &vidPid, uint8_t channel,
                              int channelWidth, const QString &keyPath) {

  int bandwidth = ChannelWidth_t::CHANNEL_WIDTH_20;
  switch (channelWidth) {
  case 20:
    bandwidth = ChannelWidth_t::CHANNEL_WIDTH_20;
    break;
  case 40:
    bandwidth = ChannelWidth_t::CHANNEL_WIDTH_40;
    break;
  case 80:
    bandwidth = ChannelWidth_t::CHANNEL_WIDTH_80;
    break;
  case 160:
    bandwidth = ChannelWidth_t::CHANNEL_WIDTH_160;
    break;
  default:
    break;
  }
  qDebug() << "startWFBReceiver: " << vidPid << " " << channel << " "
           << channelWidth << " " << keyPath;
  emit startWFBReceiver(vidPid, channel, bandwidth, keyPath);
}
bool wfb_ng_middleware::stop() {
  //  auto ret = m_wfb_receiver->stop();
  //  return ret;
}
void wfb_ng_middleware::onStartWFBReceiver(const QString &vidPid,
                                           uint8_t channel, int channelWidth,
                                           const QString &keyPath) {
  m_wfbng_thread->restart(vidPid, channel, channelWidth, keyPath);
}

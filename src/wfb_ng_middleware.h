// Copyright (c), 2022-2024, Mist Studio.

//
// Created by John on 24-6-26.
//

#ifndef WFB_NG_MIDDLEWARE_H
#define WFB_NG_MIDDLEWARE_H

#include "src/gehee/WfbngLink.hpp"
#include <QDebug>
#include <QObject>
#include <QThread>

class wfb_ng_thread : public QThread {
public:
  wfb_ng_thread(QObject *parent = nullptr) {}
  ~wfb_ng_thread() = default;
  void run() override {
    m_wfb_receiver = std::make_unique<WfbngLink>();
    m_wfb_receiver->setKeyPath(this->keyPath);
    m_wfb_receiver->setClientAddr(this->client_addr);
    m_wfb_receiver->setLinkId(this->link_id);
    m_wfb_receiver->setVideoClientPort(this->video_client_port);
    m_wfb_receiver->setVideoRadioPort(this->video_radio_port);
    m_wfb_receiver->setMavlinkClientPort(this->mavlink_client_port);
    m_wfb_receiver->setMavlinkRadioPort(this->mavlink_radio_port);

    m_wfb_receiver->run(m_wfbng_channel, m_wfb_rtl_pid, m_wfb_rtl_vid);
    m_wfb_receiver->initAgg();
  }
  void restart(const QString &vidPid, uint8_t channel, int channelWidth,
               const QString &keyPath) {
    if (!isRunning()) {
      // pid:vid,split below,use std no qt
      std::string pid_vid = vidPid.toStdString();
      m_wfb_rtl_vid = pid_vid.substr(0, pid_vid.find(":"));
      m_wfb_rtl_pid = pid_vid.substr(pid_vid.find(":") + 1);
      m_wfbng_channel = channel;
      if (m_wfb_receiver != nullptr) {
        m_wfb_receiver->setRtlChannel(channel);
        m_wfb_receiver->setRtlChannelWidth(channel_width);
      }
      this->keyPath = keyPath.toStdString();
      qDebug() << "wfb_ng_thread restart";
      start();
    } else {
      qDebug() << "wfb_ng_thread is already running";
      // 停止然后重启
      m_wfb_receiver->stop();
      wait();
      restart(vidPid, channel, channelWidth, keyPath);
    }
  };

  void setVideoClientPort(int port) {}
  void setMavlinkClientPort(int port) { mavlink_client_port = port; }
  void setClientAddr(const std::string &addr) { client_addr = addr; }
  void setLinkId(uint32_t id) { link_id = id; }
  void setVideoRadioPort(uint8_t port) { video_radio_port = port; }
  void setMavlinkRadioPort(uint8_t port) { mavlink_radio_port = port; }
  void setKeyPath(const std::string &path) { keyPath = path; }

  void setRtlChannel(uint8_t channel) { // 这个可能在运行中也可以设置,待测试
    if (m_wfb_receiver != nullptr) {
      m_wfb_receiver->setRtlChannel(channel);
    }
  }
  void setRtlChannelWidth(ChannelWidth_t w) { channel_width = w; }

private:
  std::unique_ptr<WfbngLink> m_wfb_receiver;

  int m_wfbng_channel;
  std::string m_wfb_rtl_pid;
  std::string m_wfb_rtl_vid;

  std::string keyPath = "/etc/wfb/gs.key";
  int video_client_port = 5600;
  int mavlink_client_port = 14550;
  std::string client_addr = "127.0.0.1";
  uint32_t link_id = 7669206;
  uint8_t video_radio_port = 0;
  uint8_t mavlink_radio_port = 0x10;
  ChannelWidth_t channel_width = ChannelWidth_t::CHANNEL_WIDTH_20;
};

class wfb_ng_middleware : public QObject {
  Q_OBJECT
public:
  explicit wfb_ng_middleware(QObject *parent = nullptr);
  ~wfb_ng_middleware();

  Q_INVOKABLE bool start(const QString &vidPid, uint8_t channel,
                         int channelWidth, const QString &keyPath);
  Q_INVOKABLE bool stop();

private:
  qulonglong m_wfbng_frame_count;
  qulonglong m_wfbng_rtp_pkt_count;
  qulonglong m_wfbng_wifi_frame_count;
  int m_wfbng_channel;
  wfb_ng_thread *m_wfbng_thread;

signals:
  void startWFBReceiver(const QString &vidPid, uint8_t channel,
                        int channelWidth, const QString &keyPath);

public slots:
  void onStartWFBReceiver(const QString &vidPid, uint8_t channel,
                          int channelWidth, const QString &keyPath);
};

#endif // WFB_NG_MIDDLEWARE_H

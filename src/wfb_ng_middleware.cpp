// Copyright (c), 2022-2024, Mist Studio.

//
// Created by John on 24-6-26.
//

#include "wfb_ng_middleware.h"
wfb_ng_middleware::wfb_ng_middleware(QObject *parent)
	: QObject(parent)
{
	m_wfb_receiver = new WFBReceiver();
	m_wfbng_channel = 0;
	m_wfbng_frame_count = 0;
	m_wfbng_rtp_pkt_count = 0;
	m_wfbng_wifi_frame_count = 0;
	connect(this, &wfb_ng_middleware::startWFBReceiver, this, &wfb_ng_middleware::onStartWFBReceiver);
}
wfb_ng_middleware::~wfb_ng_middleware()
{

}
bool wfb_ng_middleware::Start(const QString &vidPid, uint8_t channel, int channelWidth, const QString &keyPath)
{
//	auto ret = WFBReceiver::Instance().Start(vidPid, channel, channelWidth, keyPath);
//	return ret;
	emit startWFBReceiver(vidPid, channel, channelWidth, keyPath);
}
bool wfb_ng_middleware::Stop()
{
	auto ret = WFBReceiver::Instance().Stop();
	return ret;
}
void wfb_ng_middleware::onStartWFBReceiver(const QString &vidPid,
										   uint8_t channel,
										   int channelWidth,
										   const QString &keyPath)
{

	WFBReceiver::Instance().Start(vidPid.toStdString(), channel, channelWidth, keyPath.toStdString());
}

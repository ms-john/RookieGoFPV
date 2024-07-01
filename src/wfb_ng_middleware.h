// Copyright (c), 2022-2024, Mist Studio.

//
// Created by John on 24-6-26.
//

#ifndef WFB_NG_MIDDLEWARE_H
#define WFB_NG_MIDDLEWARE_H

#include <QObject>
#include "wfb-ng/WFBReceiver.h"

class wfb_ng_middleware: public QObject
{
Q_OBJECT
public:
	explicit wfb_ng_middleware(QObject *parent = nullptr);
	~wfb_ng_middleware();

	Q_INVOKABLE bool Start(const QString &vidPid, uint8_t channel, int channelWidth, const QString &keyPath);
	Q_INVOKABLE bool Stop();

private:
	qulonglong m_wfbng_frame_count;
	qulonglong m_wfbng_rtp_pkt_count;
	qulonglong m_wfbng_wifi_frame_count;

	int m_wfbng_channel;
	WFBReceiver *m_wfb_receiver;

signals:
	void startWFBReceiver(const QString &vidPid, uint8_t channel, int channelWidth, const QString &keyPath);

public slots:
	void onStartWFBReceiver(const QString &vidPid, uint8_t channel, int channelWidth, const QString &keyPath);
};


#endif //WFB_NG_MIDDLEWARE_H

// Copyright (c), 2022-2024, Mist Studio.

//
// Created by John on 24-6-26.
//

#ifndef WFB_API_H
#define WFB_API_H

#include <QObject>
#include <QDebug>

class wfb_api:public QObject
{
	Q_OBJECT
public:
	static wfb_api &Instance()
	{
		static wfb_api api;
		return api;
	}

public:
	wfb_api(){

	}
	~wfb_api(){

	}

	void PutLog(const std::string &level, const std::string &msg)
	{
		emit onLog(QString(level.c_str()), QString(msg.c_str()));
		qDebug() << level.c_str() << msg.c_str();
	}

	int playerPort = 5600;
	QString playerCodec;

	qulonglong wfbFrameCount_ = 0;
	qulonglong wifiFrameCount_ = 0;
	qulonglong rtpPktCount_ = 0;

	void UpdateCount()
	{
		emit onWifiFrameCount(wifiFrameCount_);
		emit onWfbFrameCount(wfbFrameCount_);
		emit onRtpPktCount(rtpPktCount_);
	}
	void NotifyWifiStop()
	{
		emit onWifiStop();
	}


signals:
	// onlog
	void onLog(QString level, QString msg);
	void onWifiStop();
	void onWifiFrameCount(qulonglong count);
	void onWfbFrameCount(qulonglong count);
	void onRtpPktCount(qulonglong count);
	void onRtpStream(QString sdp);
};

#endif //WFB_API_H

// Copyright (c), 2022-2024, Mist Studio.

//
// Created by lemon on 24-11-11.
//
#include <QObject>
#include <QWindow>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

typedef struct gst_param {
	GError* error = nullptr;
	GstElement* pipeline = nullptr;
	GstElement* source = nullptr;
	bool is_linked = false;
	union
	{
		GstElement* rtph265depay;
		GstElement* rtph264depay;
	};
	union
	{
		GstElement* avdec_h265;
		GstElement* avdec_h264;
		GstElement* decodebin3;
	};
	GstElement* sink = nullptr;
	bool is_playing = false;
} gst_param_t;

class VideoStream : public QObject
{
	Q_OBJECT
	Q_PROPERTY(WId playerId READ playerId WRITE setPlayerId NOTIFY playerIdChanged)
public:
	VideoStream(QObject* parent = nullptr);
	~VideoStream();

	Q_INVOKABLE void start();

	Q_INVOKABLE void startH265();
	Q_INVOKABLE void startH264();

	WId playerId() const;
	void setPlayerId(WId playerId);

	gst_param_t gst_param;

signals:
	void playerIdChanged(WId playerId);

private slots:
	void onPlayerIdChanged(WId playerId);

private:
	WId video_player_id;
};

class WindowHelper : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE WId getWinId(QWindow* window)
	{
		return window->winId();
	}
};
// Copyright (c), 2022-2024, Mist Studio.

//
// Created by lemon on 24-11-11.
//
#include "VideoStream.h"
#include "gst/gstelement.h"
#include <QDebug>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
VideoStream::VideoStream(QObject* parent)
	: QObject(parent)
{
}

VideoStream::~VideoStream()
{
}

void VideoStream::start()
{
	if (gst_param.is_playing)
	{
		return;
	}
	qDebug() << "start";
	gst_param.pipeline = gst_pipeline_new("rtp-h265-pipeline");
	gst_param.source = gst_element_factory_make("udpsrc", "source");
	g_object_set(gst_param.source, "port", 5600, nullptr);
	g_object_set(gst_param.source, "caps", gst_caps_from_string("application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H265"), nullptr);
	gst_param.rtph265depay = gst_element_factory_make("rtph265depay", "rtph265depay");
	gst_param.avdec_h265 = gst_element_factory_make("avdec_h265", "avdec_h265");

	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.source);
	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.rtph265depay);
	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.avdec_h265);
	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.sink);

	gst_element_link(gst_param.source, gst_param.rtph265depay);
	gst_element_link(gst_param.rtph265depay, gst_param.avdec_h265);
	gst_element_link(gst_param.avdec_h265, gst_param.sink);

	gst_element_set_state(gst_param.pipeline, GST_STATE_PLAYING);
	gst_param.is_playing = true;
}

void VideoStream::startH265()
{
	if (gst_param.pipeline)
	{
		gst_element_set_state(gst_param.pipeline, GST_STATE_NULL);
		gst_object_unref(gst_param.pipeline);
		gst_param.pipeline = nullptr;
		gst_param.sink = gst_element_factory_make("glimagesink", "sink");
		gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(gst_param.sink), video_player_id);

		gst_param.is_playing = false;
	}
	gst_param.pipeline = gst_pipeline_new("rtp-h264-pipeline");
	gst_param.source = gst_element_factory_make("udpsrc", "source");
	g_object_set(gst_param.source, "port", 5600, nullptr);
	g_object_set(gst_param.source, "caps", gst_caps_from_string("application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H265"), nullptr);
	gst_param.rtph265depay = gst_element_factory_make("rtph265depay", "rtph265depay");
	gst_param.avdec_h265 = gst_element_factory_make("avdec_h265", "avdec_h265");

	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.source);
	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.rtph265depay);
	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.avdec_h265);
	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.sink);

	gst_element_link(gst_param.source, gst_param.rtph265depay);
	gst_element_link(gst_param.rtph265depay, gst_param.avdec_h265);
	gst_element_link(gst_param.avdec_h265, gst_param.sink);

	gst_element_set_state(gst_param.pipeline, GST_STATE_PLAYING);
	gst_param.is_playing = true;
}

void VideoStream::startH264()
{
	if (gst_param.pipeline)
	{
		gst_element_set_state(gst_param.pipeline, GST_STATE_NULL);
		gst_object_unref(gst_param.pipeline);
		gst_param.pipeline = nullptr;
		gst_param.is_playing = false;
		gst_param.sink = gst_element_factory_make("glimagesink", "sink");
		gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(gst_param.sink), video_player_id);
	}

	gst_param.pipeline = gst_pipeline_new("rtp-pipeline");
	gst_param.source = gst_element_factory_make("udpsrc", "source");
	g_object_set(gst_param.source, "port", 5600, nullptr);
	g_object_set(gst_param.source, "caps", gst_caps_from_string("application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264"), nullptr);
	gst_param.rtph264depay = gst_element_factory_make("rtph264depay", "rtph264depay");
	gst_param.avdec_h264 = gst_element_factory_make("avdec_h264", "avdec_h264");

	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.source);
	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.rtph264depay);
	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.avdec_h264);
	gst_bin_add(GST_BIN(gst_param.pipeline), gst_param.sink);

	gst_element_link(gst_param.source, gst_param.rtph264depay);
	gst_element_link(gst_param.rtph264depay, gst_param.avdec_h264);
	gst_element_link(gst_param.avdec_h264, gst_param.sink);

	gst_element_set_state(gst_param.pipeline, GST_STATE_PLAYING);
	gst_param.is_playing = true;
}

void VideoStream::onPlayerIdChanged(WId playerId)
{
	video_player_id = playerId;
}

WId VideoStream::playerId() const
{
	return video_player_id;
}

void VideoStream::setPlayerId(WId playerId)
{
	video_player_id = playerId;
	emit playerIdChanged(playerId);
	qDebug() << "playerId" << playerId;
	if (!gst_param.sink)
	{
		gst_param.sink = gst_element_factory_make("glimagesink", "sink");
	}
	gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(gst_param.sink), video_player_id);
}

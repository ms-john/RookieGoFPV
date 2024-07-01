// Copyright (c), 2022-2024, Mist Studio.

//
// Created by John on 24-6-26.
//

#include "GstPlayerWidget.h"
#include <gst/video/videooverlay.h>
namespace ms
{
namespace app
{
namespace player
{

bool GstPlayerWidget::play()
{
	Q_ASSERT(m_pipeline);
	auto ret = gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
	return ret;
}
bool GstPlayerWidget::pause()
{
	Q_ASSERT(m_pipeline);
	auto ret = gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
	if (ret == GST_STATE_CHANGE_FAILURE)
		qDebug() << "Unable to set the pipeline to the paused state.";
	return ret;
}
bool GstPlayerWidget::stop()
{
	Q_ASSERT(m_pipeline);
	auto ret = gst_element_set_state(m_pipeline, GST_STATE_NULL);
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	return ret;
}

bool GstPlayerWidget::setPlayUri(const QString &uri)
{
	m_play_uri = uri;
	QString autovideosink = " ! glimagesink name=ms-videosink";
	m_play_uri += autovideosink;
	std::string url = m_play_uri.toStdString();
	qDebug() << "url: " << url.c_str();

	if (m_pipeline) {
		stop();
	}

	GError *error = nullptr;
	if (m_pipeline == nullptr) {
		qDebug() << "pipeline is null";
	}
	m_pipeline = gst_parse_launch(url.c_str(), &error);
	// 打印 error
	if (error) {
		qDebug() << "Failed to create pipeline: " << error->message;
		g_error_free(error);
		return false;
	}
	if (!m_pipeline) {
		qDebug() << "Failed to create pipeline";
		return false;
	}
	GstElement *udp_src = gst_bin_get_by_name(GST_BIN(m_pipeline), "ms-udp");
	if (udp_src) {
		printf("udp_src is not null\n");
		g_object_set(udp_src,
					 "caps",
					 gst_caps_from_string("application/x-rtp, media=video, clock-rate=90000, encoding-name=H265"),
					 nullptr);
	}
	m_sink = gst_bin_get_by_name(GST_BIN(m_pipeline), "ms-videosink");
	g_object_set(m_sink, "sync", false, nullptr);
	gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(m_sink), windowId);
	auto ret = gst_element_set_state(m_pipeline, GST_STATE_READY);
	return ret;
}
GstPlayerWidget::GstPlayerWidget(QWidget *parent)
{
	windowId = winId();
}
GstPlayerWidget::~GstPlayerWidget()
{
	gst_object_unref(m_pipeline);
}

}
}
}
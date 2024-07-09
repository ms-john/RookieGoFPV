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
GstPlayerThread::GstPlayerThread(QObject *parent, WId windowId)
	: QThread(parent)
{
	this->windowId = windowId;
}

GstPlayerThread::~GstPlayerThread()
{
	if (m_pipeline) {
		gst_object_unref(m_pipeline);
	}
}

void GstPlayerThread::run()
{ exec(); }

bool GstPlayerThread::setPlayUri(const QString &uri)
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
	m_pipeline = gst_parse_launch(url.c_str(), &error);
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
		g_object_set(udp_src, "caps",
					 gst_caps_from_string("application/x-rtp, media=video, "
										  "clock-rate=90000, encoding-name=H265"),
					 nullptr);
	}
	m_sink = gst_bin_get_by_name(GST_BIN(m_pipeline), "ms-videosink");
	g_object_set(m_sink, "sync", false, nullptr);
	gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(m_sink), windowId);
	auto ret = gst_element_set_state(m_pipeline, GST_STATE_READY);
	return ret;
}

bool GstPlayerThread::play()
{
	Q_ASSERT(m_pipeline);
	auto ret = gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
	return ret;
}

bool GstPlayerThread::pause()
{
	//  Q_ASSERT(m_pipeline);
	auto ret = gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
	if (ret == GST_STATE_CHANGE_FAILURE)
		qDebug() << "Unable to set the pipeline to the paused state.";
	return ret;
}

bool GstPlayerThread::stop()
{
	Q_ASSERT(m_pipeline);
	auto ret = gst_element_set_state(m_pipeline, GST_STATE_NULL);
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	return ret;
}

GstPlayerWidget::GstPlayerWidget(QWidget *parent)
	: QWidget(parent)
{
	m_thread = new GstPlayerThread(this, this->winId());
	m_thread->start();
}

GstPlayerWidget::~GstPlayerWidget()
{
	m_thread->quit();
	m_thread->wait();
	delete m_thread;
}

bool GstPlayerWidget::setPlayUri(const QString &uri)
{
	return m_thread->setPlayUri(uri);
}

bool GstPlayerWidget::play()
{
	return m_thread->play();
}

bool GstPlayerWidget::pause()
{
	return m_thread->pause();
}

bool GstPlayerWidget::stop()
{
	return m_thread->stop();
}

} // namespace player
} // namespace app
} // namespace ms
// Copyright (c), 2022-2024, Mist Studio.

//
// Created by John on 24-6-26.
//

#ifndef GSTPLAYERWIDGET_H
#define GSTPLAYERWIDGET_H

#include <QThread>
#include <QWidget>
#include <gst/gst.h>

namespace ms
{
namespace app
{
namespace player
{

class GstPlayerThread: public QThread
{
Q_OBJECT

public:
	GstPlayerThread(QObject *parent = nullptr, WId windowId = 0);
	~GstPlayerThread();
	void run() override;
	bool setPlayUri(const QString &uri);
	bool play();
	bool pause();
	bool stop();

signals:
	void errorOccurred(const QString &error);

private:
	GstElement *m_pipeline = nullptr;
	GstElement *m_sink = nullptr;
	QString m_play_uri;
	WId windowId;
};

class GstPlayerWidget: public QWidget
{
Q_OBJECT

public:
	explicit GstPlayerWidget(QWidget *parent = nullptr);
	~GstPlayerWidget();

	Q_INVOKABLE bool setPlayUri(const QString &uri);
	Q_INVOKABLE bool play();
	Q_INVOKABLE bool pause();
	Q_INVOKABLE bool stop();

private:
	GstPlayerThread *m_thread = nullptr;
};
} // namespace player
} // namespace app
} // namespace ms

#endif // GSTPLAYERWIDGET_H

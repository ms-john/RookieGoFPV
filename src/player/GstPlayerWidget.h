// Copyright (c), 2022-2024, Mist Studio.

//
// Created by John on 24-6-26.
//

#ifndef GSTPLAYERWIDGET_H
#define GSTPLAYERWIDGET_H

#include <QWidget>
#include <gst/gst.h>
namespace ms
{
namespace app
{
namespace player
{

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

signals:

public slots:

private:
	GstElement *m_pipeline;
	GstElement *m_sink;
	QString m_play_uri;
	WId windowId;
};
}
}
}

#endif //GSTPLAYERWIDGET_H

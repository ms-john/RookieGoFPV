// Copyright (c), 2022-2024, Mist Studio.

// created by John on 24-7-1.
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QWidget>
#include <QQmlContext>
#include <QQuickItem>
#include <QHBoxLayout>
#include <QQuickView>

#include <gst/gst.h>
#include <QMainWindow>
#include "player/GstPlayerWidget.h"

//0bda:881a
#include "wfb_ng_middleware.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

        app.setOrganizationName("NULL");
        app.setOrganizationDomain("NULL");
        app.setApplicationName("RookieGOFPV_OPENIPC");

	gst_init(&argc, &argv);

	auto main_width = 800+200;
	auto main_height = 600;

	auto player_width = 800;
	auto player_height = 600;

	auto qml_width = 200;
	auto qml_height = 600;

	QMainWindow mainwindow;
	mainwindow.resize(main_width, main_height);
	mainwindow.setObjectName("rootWidget");
	mainwindow.setWindowTitle("RookieGoFPV for WFB-NG/OpenIPC-FPV (macOS)");

	QWidget *centralWidget = new QWidget(&mainwindow);
	QHBoxLayout *layout = new QHBoxLayout(centralWidget);
	mainwindow.setCentralWidget(centralWidget);
	centralWidget->resize(qml_width, qml_height);
	centralWidget->setLayout(layout);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	ms::app::player::GstPlayerWidget playerWidget;
	playerWidget.setObjectName("playerWidget");
	playerWidget.resize(player_width, player_height);
	playerWidget.setMinimumWidth(player_width);
	playerWidget.setMinimumHeight(player_height);
	QString sampleUri = "videotestsrc pattern=ball ! video/x-raw,format=NV12,width=1920,height=1080,framerate=60/1";
	playerWidget.setPlayUri(sampleUri);

	layout->addWidget(&playerWidget);

	wfb_ng_middleware wfbNgMiddleware;

	QQuickView qmlView;
	QWidget *container = QWidget::createWindowContainer(&qmlView);
	container->resize(qml_width, qml_height);
	qmlView.resize(qml_width, qml_height);
	qmlView.setResizeMode(QQuickView::SizeViewToRootObject);
	layout->addWidget(container);
	qmlView.setSource(QUrl("qrc:/qml/main.qml"));
	qmlView.rootContext()->setContextProperty("player", &playerWidget);
	qmlView.rootContext()->setContextProperty("wfbNG", &wfbNgMiddleware);
	layout->addWidget(container);
	mainwindow.show();

	return app.exec();
}

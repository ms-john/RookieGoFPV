// Copyright (c), 2022-2024, Mist Studio.

// Modified by lemon on 24-11-11.
#include "wfb_ng_middleware.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <gst/gst.h>

#include "VideoStream.h"


int main(int argc, char* argv[])
{
	QGuiApplication app(argc, argv);
	gst_init(&argc, &argv);
	QString app_path = QCoreApplication::applicationDirPath();

	app.setOrganizationName("POCKETFPV");
	app.setOrganizationDomain("");
	app.setApplicationName("RookieGOFPV");

	QString qt_version = qVersion();

	VideoStream videoStream;

	qmlRegisterType<WindowHelper>("com.pocketfpv.windowHelper", 1, 0, "WindowHelper");

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("qt_version", qt_version);
	engine.rootContext()->setContextProperty("videoStream", &videoStream);

	engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

	wfb_ng_middleware wfbNgMiddleware;

	engine.rootContext()->setContextProperty("wfbNG", &wfbNgMiddleware);

	return app.exec();
}

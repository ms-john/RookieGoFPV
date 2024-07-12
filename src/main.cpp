// Copyright (c), 2022-2024, Mist Studio.

// created by John on 24-7-1.
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include "wfb_ng_middleware.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setOrganizationName("NULL");
	app.setOrganizationDomain("NULL");
	app.setApplicationName("RookieGOFPV_OPENIPC");

	//("RookieGoFPV for WFB-NG/OpenIPC-FPV (macOS)");

        QQmlApplicationEngine engine;
        const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
        QObject::connect(
            &engine,
            &QQmlApplicationEngine::objectCreationFailed,
            &app,
            []() { QCoreApplication::exit(-1); },
            Qt::QueuedConnection);
        engine.load(url);

//
//	QQuickView qmlView;
//	QWidget *container = QWidget::createWindowContainer(&qmlView);
//	container->resize(qml_width, qml_height);
//	qmlView.resize(qml_width, qml_height);
//	qmlView.setResizeMode(QQuickView::SizeViewToRootObject);
//	layout->addWidget(container);
//	qmlView.setSource(QUrl("qrc:/qml/main.qml"));
//	qmlView.rootContext()->setContextProperty("player", &playerWidget);
//	qmlView.rootContext()->setContextProperty("wfbNG", &wfbNgMiddleware);
//	layout->addWidget(container);
//	mainwindow.show();

	return app.exec();
}

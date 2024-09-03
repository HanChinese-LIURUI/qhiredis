#pragma once
#include "ui_tcpclient.h"
#include <QTcpSocket>
#include <QtWidgets/QMainWindow>
#include <QElapsedTimer>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class tcpClientClass; };
QT_END_NAMESPACE

class tcpClient : public QMainWindow {
	Q_OBJECT

public:
	tcpClient(QWidget* parent = nullptr);
	~tcpClient();

private:
	QTcpSocket* socket;
	QTimer* timer;
	QElapsedTimer* elapsedTimer;
	Ui::tcpClientClass* ui;
};

#include "tcpclient.h"
#include <QDebug>


tcpClient::tcpClient(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::tcpClientClass()) {
	ui->setupUi(this);

	socket = new QTcpSocket(this);
	timer = new QTimer(this);
	elapsedTimer = new QElapsedTimer();

	timer->start(50);

	socket->connectToHost("192.168.3.11", 8080);
	//socket->connectToHost("192.168.3.10", 8080);
	connect(socket, &QTcpSocket::connected, [=]() {
		QByteArray stx = QByteArray::fromHex("02");
		QByteArray etx = QByteArray::fromHex("03");
		socket->write(stx + "11111" + etx);
		});

	connect(socket, &QTcpSocket::readyRead, [=]() {
		QByteArray data = socket->readAll();

		if (data.contains("OK") && elapsedTimer->elapsed() > 3) {
			qDebug() << u8"耗时" << elapsedTimer->elapsed() << data;
		}
		else {
			//qDebug() << data;
		}

		});

	connect(timer, &QTimer::timeout, this, [=]() {
		QByteArray stx = QByteArray::fromHex("02");
		QByteArray etx = QByteArray::fromHex("03");
		QByteArray send = stx + "(jsfjl的撒范德萨范德萨分！#@￥*）（……&……@）fjlksadjflkdsajfl;kdsajfl;kdsajfl;ksdajfl;kdsajfl;kdsajfl;ksadjflk;dsajflk;dsajfl;kdsajfl;kjlkjsadflk;dsajf6354f654646w5e4r65ew范德萨发萨达5法二维5过是大发但是法65萨德方5撒撒对方54额94四5冯工，lk（#@！）（#*）（……%……@#@#？》《)" +etx;
		socket->write(send);
		elapsedTimer->start();

		});
}

tcpClient::~tcpClient() {
	delete ui;
}

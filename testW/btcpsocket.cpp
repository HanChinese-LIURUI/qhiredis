#include "btcpsocket.h"
#include <QTextCodec>

BTcpSocket::BTcpSocket(qintptr descriptor, QObject* parent)
	: bDescriptor(descriptor),
	QTcpSocket(parent),
	thread(new QThread()),
	indexAcc(0),
	frameHeader(true) {

	thread->moveToThread(thread);
	moveToThread(thread);

	connect(thread, &QThread::started, this, &BTcpSocket::initSocket);
	connect(thread, &QThread::finished, this, &BTcpSocket::deleteLater);
	connect(this, &QTcpSocket::readyRead, this, &BTcpSocket::dataProcessing);

	thread->start();

}

BTcpSocket::~BTcpSocket() {
	thread->quit();
	thread->wait();
	thread->deleteLater();

}

void BTcpSocket::initSocket() {
	setSocketDescriptor(bDescriptor);

}

void BTcpSocket::dataProcessing() {
	QByteArray lineData, lineHexData;

	QByteArray readData = readAll().toHex();
	buffer.append(readData);

	if (buffer.size() < 2)
		return;

	while (!buffer.isEmpty()) {

		if (buffer.mid(0, 2) == "02") {
			frameHeader = false;
			buffer.remove(0, 2);
		}
		else {
			if (frameHeader) {
				buffer.clear();
				frameHeader = true;
				qWarning() << u8"帧头不符合要求";
				return;
			}
		}

		int index = buffer.indexOf("03", indexAcc);
		if (index == 0) {
			buffer.remove(0, index + 2);
			frameHeader = true;
			qWarning() << u8"帧头到帧未没有数据";
			return;
		}
		if (index < 0) {
			continue;
		}

		if (index % 2 == 0) {
			lineHexData = buffer.mid(0, index);
			
			lineData = QByteArray::fromHex(lineHexData);
			QTextCodec* tc = QTextCodec::codecForName("GBK");
			emit readyData(tc->toUnicode(lineData).toUtf8());

			buffer.remove(0, index + 2);
			indexAcc = 0;
			frameHeader = true;
		}
		else {
			indexAcc += 2;
		}
	}
}
#include "btcpserver.h"
#include "testw.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QtConcurrent>
#include <QThread>
#include <QDateTime>


TestW::TestW(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::TestWClass()) {
	ui->setupUi(this);

	QHiRedis redis;
	QString error;
	bool state = redis.connectServer(&error,"","blsm02386886360+");
	QVariant s = redis.getHashField(1200, "HTTP://S.JNC.CN/NA/pKF6JoLqhyU7TYfbB5a");
	QVariantHash h = redis.getHashAll(1200);
	getHashfield();
	hashInsert();

}

TestW::~TestW() {
	delete ui;
}

void TestW::hashInsert() {
	QHiRedis redis;
	QString error;
	bool state = redis.connectServer(&error);

	QFile file(R"(\\IUFO\Server\测试数据\nei.txt)");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << u8"文件打开失败";
		return;
	}

	QVariantHash hash;
	while (!file.atEnd()) {
		QString line = file.readLine().trimmed();
		if (line.isEmpty()) continue;
		hash.insert(line, 0);
		if (hash.size() == 1000 || file.atEnd()) {
			QElapsedTimer timer;
			timer.start();
			state = redis.setHash("tes1t", hash);
			qInfo() << u8"插入数据库耗时" << timer.elapsed() << state;
			hash.clear();
		}

	}

}

void TestW::getHashfield() {

	QtConcurrent::run([]() {
		QHiRedis redis;
		QString error;
		bool state = redis.connectServer(&error);
		QElapsedTimer timer;

		while (1) {
			QThread::msleep(100);
			timer.start();
			QVariant s = redis.getHashField(1200, "HTTP://S.JNC.CN/NA/pKF6JoLqhyU7TYfbB5a");
			if (timer.elapsed() > 3)
				qInfo() << u8"查询数据" << timer.elapsed() << state << s;
		}
		}
	);

}

#include "testw.h"

TestW::TestW(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::TestWClass()) {
	ui->setupUi(this);

	QHiRedis redis;
	QString error;
	bool state = redis.connectServer(&error);
	state = redis.setString(321, 321);
	state = redis.expire(321,15);

}

TestW::~TestW() {
	delete ui;
}

#ifndef TESTW_H__
#define TESTW_H__

#include "ui_testw.h"
#include <QtWidgets/QMainWindow>
#include "../qhiredis/qhiredis.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TestWClass; };
QT_END_NAMESPACE

class TestW : public QMainWindow {
	Q_OBJECT

public:
	TestW(QWidget* parent = nullptr);
	~TestW();

private:
	void hashInsert();
	void getHashfield();
private:
	Ui::TestWClass* ui;
};

#endif // TESTW_H__

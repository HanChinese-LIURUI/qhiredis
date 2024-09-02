#include "testw.h"
#include <QDebug>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	QByteArray lin1eData = QByteArray::fromHex("023031402003");


	TestW w;
	//w.show();
	QByteArray buffer, lineHexData, lineData;
	int bufferSize = 0;

	QString test1 = "01@ ";
	QString test2 = "01@ ";
	QByteArray hex = "02" + test1.toUtf8().toHex().toUpper() + "03"
		"02" + test2.toUtf8().toHex().toUpper() + "03";

	buffer.append(hex);
	int indexAcc = 0;
	while (!buffer.isEmpty()) {

		//如果包含stx
		if (buffer.mid(0, 2) == "02") {
			buffer.remove(0, 2);
		}

		//如果包含了etx
		
		int index = buffer.indexOf("03", indexAcc);
		if (index <= 0) {
			buffer.remove(0, 2);
			continue;
		}

		if (index % 2 == 0) {
			lineHexData = buffer.mid(0, index);
			lineData = QByteArray::fromHex(lineHexData);
			buffer.remove(0, index + 2);
			indexAcc = 0;
			qDebug() << lineData;
		}
		else {
			indexAcc += 2;
		}
	}

	return a.exec();



}

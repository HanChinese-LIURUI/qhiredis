//!自定义的TcpSocket应用层协议
//! <STX>帧头
//! <ETX>帧尾
//! 

#ifndef BTCPSOCKET_H__
#define BTCPSOCKET_H__

#include <QObject>
#include <QTcpSocket>
#include <QThread>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

namespace ByteAgreement {
	constexpr auto STX = "02";
	constexpr auto ETX = "03";
}


class BTcpSocket : public QTcpSocket {
	Q_OBJECT

public:
	BTcpSocket(qintptr descriptor, QObject* parent = nullptr);
	~BTcpSocket();

signals:
	//! @brief 经过处理后的完整单条信息
	void readyData(const QByteArray& data);
protected slots:
	//! @brief 数据处理
	virtual void dataProcessing();

	virtual void initSocket();
private:
	qintptr	bDescriptor;
	QThread* thread;
	int indexAcc;
	QByteArray buffer;
	bool  frameHeader;
};

#endif // BTCPSOCKET_H__

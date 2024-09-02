#ifndef BTCPSERVER_H__
#define BTCPSERVER_H__

#include <QTcpServer>
#include "btcpsocket.h"

class BTcpServer : public QTcpServer {
	Q_OBJECT

public:
	explicit BTcpServer(QObject* parent = nullptr);
	~BTcpServer();

protected:
	virtual void incomingConnection(qintptr socketDescriptor);
};


#endif // BTCPSERVER_H__

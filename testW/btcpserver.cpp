#include "btcpserver.h"
#include <QThreadPool>

BTcpServer::BTcpServer(QObject* parent)
	: QTcpServer(parent) {
}

BTcpServer::~BTcpServer() {
}

void BTcpServer::incomingConnection(qintptr socketDescriptor) {
	BTcpSocket* bTcpSocket = new BTcpSocket(socketDescriptor);
	addPendingConnection(bTcpSocket);
}

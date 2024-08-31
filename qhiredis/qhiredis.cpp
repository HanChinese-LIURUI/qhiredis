#include "qhiredis.h"

QHiRedis::QHiRedis(const QString& ip, const qint64& port, QObject* parent)
	:QObject(parent),
	rIp(ip),
	rPort(port),
	redis(nullptr) {

}

QHiRedis::~QHiRedis() {
}

bool QHiRedis::connectServer(
	QString* const error,
	const QString& useName,
	const QString& passWord,
	const int& timeOut) {

	bool state = true;
	timeval time{ 0,timeOut };

	redis = redisConnectWithTimeout(rIp.toStdString().c_str(), rPort, time);
	if (!redis) {
		state = false;
	}
	else {
		*error = QString(redis->errstr);
		if (redis->err != 0) {
			state = false;
		}
	}
	//验证密码
	if (state && !useName.isEmpty() && !passWord.isEmpty()) {
		redisReply* reply = (redisReply*)redisCommand(
			redis,
			QString("AUTH %1").arg(useName).toStdString().c_str(),
			passWord.toStdString().c_str());

		if (reply->type == REDIS_REPLY_ERROR) {
			state = false;
		}
		else {
			*error = "Authentication failure";
		}
		freeReplyObject(reply);
	}
	return state;
}

bool QHiRedis::ping() const {
	bool state = true;
	redisReply* reply = (redisReply*)redisCommand(redis, "PING");

	if (reply) {
		if (reply->type == REDIS_REPLY_ERROR) {
			state = false;
		}
		else if (QString(reply->str) != "PONG") {
			state = false;
		}
	}
	else {
		state = false;
	}

	return state;
}

bool QHiRedis::del(const QVariant& key) {
	bool state = true;
	QString command = "DEL " + key.toString();

	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type != REDIS_REPLY_INTEGER) {
			state = false;
		}
		//else {
		//	reply->integer != 1 ? state = false : state = true;
		//}
	}
	else {
		state = false;
	}

	return state;
}

bool QHiRedis::exists(const QVariant& key) {
	bool state = true;
	QString command = "EXISTS " + key.toString();

	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type != REDIS_REPLY_INTEGER) {
			state = false;
		}
		else {
			reply->integer != 1 ? state = false : state = true;
		}
	}
	else {
		state = false;
	}

	return state;
}

bool QHiRedis::setString(const QVariant& key, const QVariant& value) const {
	bool state = true;
	QString command = "SET " + key.toString() + " " + value.toString();
	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type == REDIS_REPLY_ERROR) {
			state = false;
		}
		else if (QString(reply->str) != "OK") {
			state = false;
		}
	}
	else {
		state = false;
	}

	return state;
}

bool QHiRedis::setnxString(const QVariant& key, const QVariant& value) const {
	bool state = true;
	QString command = "SETNX " + key.toString() + " " + value.toString();
	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type != REDIS_REPLY_INTEGER) {
			state = false;
		}
		else if (reply->integer != 1) {
			state = false;
		}
	}
	else {
		state = false;
	}

	return state;
}

bool QHiRedis::mSetString(const QVariantMap& map) const {
	bool state = true;
	QString command = "MSET ";
	for (QString var : map.keys()) {
		command += var + " " + map.value(var).toString() + " ";

	}

	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type == REDIS_REPLY_ERROR) {
			state = false;
		}
		else if (QString(reply->str) != "OK") {
			state = false;
		}
	}
	else {
		state = false;
	}

	return state;
}

bool QHiRedis::mSetnxString(const QVariantMap& map) const {
	bool state = true;
	QString command = "MSETNX ";
	for (QString var : map.keys()) {
		command += var + " " + map.value(var).toString() + " ";

	}

	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type != REDIS_REPLY_INTEGER) {
			state = false;
		}
		else {
			if (reply->integer != 1) {
				state = false;
			}
		}
	}
	else {
		state = false;
	}

	return state;
}

QVariant QHiRedis::getString(const QVariant& key) const {
	QVariant value;

	QString command = "GET " + key.toString();
	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type == REDIS_REPLY_STRING) {
			value = QString(reply->str);
		}
	}

	return value;
}

bool QHiRedis::incrString(const QVariant& key, qint64* const value) const {
	bool state = true;
	*value = 0;

	QString command = "INCR " + key.toString();
	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type == REDIS_REPLY_INTEGER) {
			*value = reply->integer;
		}
		else {
			state = false;
		}
	}
	else {
		state = false;
	}

	return state;
}

bool QHiRedis::decrString(const QVariant& key, qint64* const value) const {
	bool state = true;
	*value = 0;

	QString command = "DECR " + key.toString();
	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type == REDIS_REPLY_INTEGER) {
			*value = reply->integer;
		}
		else {
			state = false;
		}
	}
	else {
		state = false;
	}

	return state;
}

bool QHiRedis::setHash(const QVariant& key, const QVariantHash& value) const {
	bool state = true;

	QString command = "HMSET " + key.toString() + " ";
	for (QString var : value.keys()) {
		command += var + " " + value[var].toString() + " ";
	}

	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type == REDIS_REPLY_ERROR) {
			state = false;
		}
	}
	else {
		state = false;
	}

	return state;
}

QVariantHash QHiRedis::getHash(const QVariant& key) const {
	QVariantHash valueHash;

	QString command = "HGETALL " + key.toString();
	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type == REDIS_REPLY_ARRAY) {
			size_t size = reply->elements;
			QString key, value;
			for (size_t i = 0; i < size; i++) {

				redisReply* tReply = reply->element[i];

				if (tReply) {

					if (tReply->type == REDIS_REPLY_STRING) {
						QString tString = tReply->str;
						if (i % 2 == 0)
							key = tString;
						else {
							value = tString;
							valueHash.insert(key, value);
							key.clear();
							value.clear();
						}
					}
				}
			}
		}
	}

	return valueHash;
}

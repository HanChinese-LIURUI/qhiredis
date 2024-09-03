#include "qhiredis.h"
#include <QDebug>

QHiRedis::QHiRedis(const QString& ip, const qint64& port, QObject* parent)
	:QObject(parent),
	rIp(ip),
	rPort(port),
	redis(nullptr) {

}

QHiRedis::~QHiRedis() {
	redisFree(redis);
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
	if (state && !passWord.isEmpty()) {
		QString command = "AUTH " + passWord;
		redisReply* reply = (redisReply*)redisCommand(redis, command.toUtf8());

		if (reply) {
			if (QString(reply->str) != "OK") {
				state = false;
				*error = "Authentication failure";
			}
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
	freeReplyObject(reply);
	return state;
}

bool QHiRedis::del(const QVariant& key) const {
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
	freeReplyObject(reply);
	return state;
}

bool QHiRedis::exists(const QVariant& key) const {
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
	freeReplyObject(reply);
	return state;
}

bool QHiRedis::expire(const QVariant& key, const int& sec) {
	bool state = true;
	QString command = "EXPIRE " + key.toString() + " " + QString::number(sec);

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
	freeReplyObject(reply);
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
	freeReplyObject(reply);
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
	freeReplyObject(reply);
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
	freeReplyObject(reply);
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
	freeReplyObject(reply);
	return state;
}

bool QHiRedis::setexString(const QVariant& key, const int& sec, const QVariant& value) const {
	bool state = true;
	QString command = "SETEX " + key.toString() + " " + QString::number(sec) + " " + value.toString();
	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type != REDIS_REPLY_STATUS) {
			state = false;
		}
		else {
			QString(reply->str) != "OK" ? state = false : state = true;
		}
	}
	else {
		state = false;
	}

	freeReplyObject(reply);
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
	freeReplyObject(reply);
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
	freeReplyObject(reply);
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
	freeReplyObject(reply);
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
			qWarning() << QString(reply->str);
		}
	}
	else {
		state = false;
	}
	freeReplyObject(reply);
	return state;
}

QVariant QHiRedis::getHashField(const QVariant& key, const QVariant& field) const {
	bool state = true;
	QVariant value;

	QString command = "HGET " + key.toString() + " " + field.toString();
	redisReply* reply = (redisReply*)redisCommand(redis, command.toStdString().c_str());

	if (reply) {
		if (reply->type != REDIS_REPLY_STRING) {
			state = false;
		}
		else {
			value = reply->str;
		}
	}
	else {
		state = false;
	}
	freeReplyObject(reply);
	return value;
}

QVariantHash QHiRedis::getHashAll(const QVariant& key) const {
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
						}
					}
				}
			}
		}
	}
	freeReplyObject(reply);
	return valueHash;
}

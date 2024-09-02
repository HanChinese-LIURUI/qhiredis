#ifndef QHIREDIS_H__
#define QHIREDIS_H__

#if _MSC_VER
#pragma warning(disable : 4200)

struct timeval {
	long tv_sec;     // 秒
	long tv_usec;    // 微秒
};

#endif
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "qhiredis_global.h"
#include "hiredis.h"
#include <QObject>
#include <QVariant>
#include <QHash>

class QHiRedis : public QObject {
	Q_OBJECT

public:
	explicit QHiRedis(const QString& ip = "127.0.0.1", const qint64& port = 6379, QObject* parent = nullptr);
	~QHiRedis();

	//! @brief 连接服务器
	//! @param timeOut 阻塞连接
	//! @param error 返回的错误状态
	//! @return 成功:true 
	bool connectServer(
		QString* const error,
		const QString& useName = "",
		const QString& passWord = "",
		const int& timeOut = 1000);

	//! @brief 检测服务是否正常
	//! @return 
	bool ping() const;

	//! @brief 删除指定的key
	//! @param key 
	//! @return 成功:true(不存在这个数据也会返回true)
	bool del(const QVariant& key) const;

	//! @brief 检查给定key是否存在
	//! @param key 
	//! @return 存在:true
	bool exists(const QVariant& key) const;

	//! @brief 设置一个key-value对，key如果存在，则更新value。
	//! @param key 
	//! @param value 
	//! @return 成功:true 
	bool setString(const QVariant& key, const QVariant& value) const;

	//! @brief 只有在key不存在时设置key的值
	//! @param key 
	//! @param value 
	//! @return 成功:true(代表数据不存在且插入成功) false:(数据插入失败，但不代表一定是数据存在返回的失败)
	bool setnxString(const QVariant& key, const QVariant& value) const;

	//! @brief 同时设置一个或多个key-value对，key如果存在，则更新value。
	//! @param map 
	//! @return 
	bool mSetString(const QVariantMap& map) const;

	//! @brief 同时设置一个或多个key-value对，当且仅当所有给定key都不存在
	//! @param key 
	//! @param value 
	//! @return 成功:true(代表所有的数据都不存在且插入成功) false:(数据插入失败，但不代表一定是数据存在返回的失败)
	bool mSetnxString(const QVariantMap& map) const;

	//! @brief 获取一个key字符串值
	//! @param key 
	//! @return 
	QVariant getString(const QVariant& key) const;

	//! @brief 将key的值自增1(value必须为数字值),如果不存在key则先插入再自增
	//! @param key 
	//! @param value 
	//! @return 成功:true 
	bool incrString(const QVariant& key, qint64* const value) const;

	//! @brief 将key的值自减1(value必须为数字值),如果不存在key则先插入再自减
	//! @param key 
	//! @param value 
	//! @return 
	bool decrString(const QVariant& key, qint64* const value) const;

	//! @brief 添加一个Hash
	//! @param key 
	//! @param value 
	//! @return 成功:true 
	bool setHash(const QVariant& key, const QVariantHash& value) const;

	//! @brief 获取一个keyHash值
	//! @param key 
	//! @return 
	QVariantHash getHash(const QVariant& key) const;
protected:

private:
	QString rIp;
	qint64 rPort;
	redisContext* redis;
};

#endif // QHIREDIS_H__

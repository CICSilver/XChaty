#pragma once

#include <QByteArray>
#include <QDataStream>
#include "chatyDef.h"
typedef struct protoHead
{
	protoHead()
	{
		type = 0;
		checkSum = 0;
		memset(time, 0, sizeof(time));
	}
	quint8 type;
	char time[24];
	quint16 checkSum;
}proto_head;

typedef struct ProtoBody
{
	protoHead msg_head;
	chaty::ClientMessage msg_body;
};
class AbstractMessage
{
public:
	virtual QByteArray Serrialize(const char* msg)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream << msg;
		return buffer;
	};
	virtual const char* DeSerrialize(const QByteArray& ba)
	{
		QByteArray _temp_ba(ba);
		QDataStream stream(&_temp_ba, QIODevice::ReadOnly);
		QString str;
		stream >> str;
		return str.toLatin1().data();
	};
protected:
	protoHead m_proto_head;
};

class LoginRequest : public AbstractMessage
{
public:
	LoginRequest() {};
	explicit LoginRequest(const char* time, quint16 _check_sum);
	QByteArray Serrialize(const char* msg) override;
	const char* DeSerrialize(const QByteArray& ba) override;
};



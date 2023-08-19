#pragma once
#include <QDateTime>
#include <memory>

namespace protochat
{
	struct BaseHead
	{
		BaseHead()
		{
			time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
		}
		QString time;
		quint8 msgType;
	};

	typedef struct MessageBase
	{
		MessageBase(QString customMsg = ""){}
		virtual ~MessageBase() {}

		virtual std::unique_ptr<MessageBase> clone() = 0;
	}BaseMsg;
}
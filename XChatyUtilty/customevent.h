#pragma once

#include <QEvent>
#include <QString>
const static QEvent::Type qEventRecvMsg = (QEvent::Type)(QEvent::User + 1);
class QStringEvent  : public QEvent
{
public:
	QStringEvent(const QString& msg = "", QEvent::Type type = qEventRecvMsg) : QEvent(type)
	{
		m_msg = msg;
	}
	QString m_msg;
};

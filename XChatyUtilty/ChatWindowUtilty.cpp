#include "chatwindowutilty.h"
#include"customevent.h"
#include <QDateTime>
#include <QApplication>
void ChatWindowUtilty::ShowMsg(const QString& str)
{
	QString newStr(str);
	checkAndAddLineBreak(newStr);
	m_chatEdit->setText(newStr);
	m_chatEdit->repaint();
}
void ChatWindowUtilty::AppendMsg(const QString& str)
{
	// "....\n...\n...\n"
	QString chatContent = m_chatEdit->toPlainText();
	chatContent = chatContent
		+ QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz] ")
		+ str;
	
	ShowMsg(chatContent);
}

void ChatWindowUtilty::ClearMsg()
{
	m_chatEdit->clear();
}

void ChatWindowUtilty::OverWriteMsg(const QString& str)
{
	ClearMsg();
	ShowMsg(str);
}

void ChatWindowUtilty::PostMsg(QObject* reciver, const QString& str)
{
	QStringEvent* event = new QStringEvent(str);
	QApplication::postEvent(reciver, event);
}

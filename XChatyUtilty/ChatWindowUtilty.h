#pragma once
#include <QString>
#include <QTextEdit>
#include "TcpServer.h"
class ChatWindowUtilty
{
public:
	ChatWindowUtilty* GetInstance()
	{
		ChatWindowUtilty* instance = new ChatWindowUtilty;
		return instance;
	}
	~ChatWindowUtilty(){}

	void ShowMsg(QTextEdit* textWidget, const QString& str);

	void AppendMsg(QTextEdit* textWidget, const QString& str);

	void ClearMsg(QTextEdit* textWidget, const QString& str);
private:
	ChatWindowUtilty();
};


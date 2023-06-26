#include "ChatWindowUtilty.h"
#include <QDateTime>
void ChatWindowUtilty::ShowMsg(QTextEdit* textWidget, const QString& str)
{
	textWidget->setText(str);
	textWidget->repaint();
}

void ChatWindowUtilty::AppendMsg(QTextEdit* textWidget, const QString& str)
{
	// "....\n...\n...\n"
	QString chatContent = textWidget->toPlainText();
	chatContent = chatContent 
		+ QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz] ")
		+ str;
	if (chatContent.back() != "\n")
		chatContent.append("\n");
	ShowMsg(textWidget, chatContent);
}

void ChatWindowUtilty::ClearMsg(QTextEdit* textWidget, const QString& str)
{
	textWidget->clear();
}

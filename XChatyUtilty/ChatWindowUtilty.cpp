#include "ChatWindowUtilty.h"

void ChatWindowUtilty::ShowMsg(QTextEdit* textWidget, const QString& str)
{
	textWidget->setText(str);
	textWidget->repaint();
}

void ChatWindowUtilty::AppendMsg(QTextEdit* textWidget, const QString& str)
{
	QString chatContent = textWidget->toPlainText();
	
}

void ChatWindowUtilty::ClearMsg(QTextEdit* textWidget, const QString& str)
{
}

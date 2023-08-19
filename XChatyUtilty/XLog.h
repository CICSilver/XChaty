#pragma once
#include <QDebug>
namespace
{
	// 递归处理多个参数
	template<typename T>
	void handleArgs(QDebug& debug, T t)
	{
		debug << t;
	}
	template<>
	void handleArgs(QDebug& debug, std::string t)
	{
		debug << QString::fromStdString(t);
	}
	template<typename T, typename... Args>
	void handleArgs(QDebug& debug, T t, Args... args)
	{
		debug << t << " ";
		handleArgs(debug, args...);
	}
}
#define __XCHATYLOG(x, str, ...) do { \
    QDebug debug = x().noquote().nospace(); \
    debug << __FILE__ << ":" << __LINE__ << "    "; \
    handleArgs(debug, ##__VA_ARGS__);     \
} while (0) 
#define XLOG(...) __XCHATYLOG(qDebug, "LOG: ", __VA_ARGS__)
#define XWAR(...) __XCHATYLOG(qWarning, "Warning: ", __VA_ARGS__)
#define XERR(...) __XCHATYLOG(qCritical, "Error: ", __VA_ARGS__)
#define CURRENT_TR(a) toUnicode(a)
#define PrintString(a)  qUtf8Printable(a)
#define PrintChinese(a) PrintString(CURRENT_TR(a))
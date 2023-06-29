#pragma once
#include <qcoreapplication.h>
#include <QString>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDataStream>

#define CHATY_DB_NAME "ChatDB.db"
#define CHATY_DB_USER_TABLE "UserTable"
namespace
{
	// 递归处理多个参数
	template<typename T>
	void handleArgs(QDebug& debug, T t) 
	{
		debug << t;
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

namespace chaty
{
	typedef int (*Callback)(void*, int, char**, char**);

	enum ClientMsgType
	{
		LOGIN = 0,
		REGIST,
		
		FAILED = 1000
	};

	typedef struct UserStruct
	{
		UserStruct()
		{
			userName = "";
			passwd = "";
			id = "";
		}

		UserStruct operator=(UserStruct& _user)
		{
			this->id = _user.id;
			this->userName = _user.userName;
			this->passwd = _user.passwd;
		}

		QString userName;
		QString passwd;
		QString id;
	}User;

	typedef struct LoginMessage
	{
		// login message from client
		LoginMessage(User _user, ClientMsgType _type)
			:user(_user),
			msgType(_type) {}
		User user;
		ClientMsgType msgType;
	}LogMsg;

	static QString GetDataBasePath(QString db_name) {
		// 获取执行文件目录
		QString exe_dir = QCoreApplication::applicationDirPath();
		// 拼接执行文件目录和文件目录
		QString file_dir = exe_dir + "/sql_data";
		// 创建文件目录，如果已经存在则忽略
		QDir().mkdir(file_dir);
		// 拼接文件目录和文件名
		QString file_path = file_dir + "/" + db_name;
		return file_path;
	}

	static QDataStream& operator<<(QDataStream& stream, const UserStruct& user) {
		stream << user.userName << user.passwd << user.id;
		return stream;
	}

	static QDataStream& operator>>(QDataStream& stream, UserStruct& user) {
		stream >> user.userName >> user.passwd >> user.id;
		return stream;
	}

	static QDataStream& operator<<(QDataStream& stream, const LoginMessage& msg) {
		stream << msg.user << static_cast<qint32>(msg.msgType);
		return stream;
	}

	static QDataStream& operator>>(QDataStream& stream, LoginMessage& msg) {
		qint32 type;
		stream >> msg.user >> type;
		msg.msgType = static_cast<ClientMsgType>(type);
		return stream;
	}

}// end namespace chaty
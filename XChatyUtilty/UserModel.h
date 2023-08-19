#pragma once
#include <QString>
#include <QCoreApplication>
#include <QDir>
namespace chaty
{
	typedef int (*Callback)(void*, int, char**, char**);

	typedef struct UserStruct
	{
		UserStruct()
		{
			userName = "";
			passwd = "";
			id = "";
		}

		UserStruct& operator=(const UserStruct& _user)
		{
			this->id = _user.id;
			this->userName = _user.userName;
			this->passwd = _user.passwd;
			return *this;
		}
		UserStruct operator=(const UserStruct* _user)
		{
			this->id = _user->id;
			this->userName = _user->userName;
			this->passwd = _user->passwd;
			return *this;
		}

		QString userName;
		QString passwd;
		QString id;
	}User;

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
}// end namespace chaty
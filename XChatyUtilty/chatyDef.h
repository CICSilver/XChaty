#pragma once
#include "xchatyutilty_global.h"
#include <qcoreapplication.h>
#include <QString>
#include <QFile>
#include <QDir>
namespace chaty
{
#define CHATY_DB_NAME "ChatDB.db"
#define CHATY_DB_USER_TABLE "UserTable"

	typedef int (*Callback)(void*, int, char**, char**);

	typedef struct UserStruct
	{
		UserStruct()
		{
			userName = "";
			passwd = "";
			id = "";
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
}// end namespace chaty

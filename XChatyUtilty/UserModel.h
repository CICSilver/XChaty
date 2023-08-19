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
		// ��ȡִ���ļ�Ŀ¼
		QString exe_dir = QCoreApplication::applicationDirPath();
		// ƴ��ִ���ļ�Ŀ¼���ļ�Ŀ¼
		QString file_dir = exe_dir + "/sql_data";
		// �����ļ�Ŀ¼������Ѿ����������
		QDir().mkdir(file_dir);
		// ƴ���ļ�Ŀ¼���ļ���
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
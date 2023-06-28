#include "UserTableDAO.h"

UserTableDAO::UserTableDAO()
{
	m_queryUserCB = [] (void* data, int argc, char** argv, char** colname) -> int
	{
		auto user = (chaty::User*)data;
		user->id = atoi(argv[0]);
		user->userName = argv[1];
		user->passwd = argv[2];
		return 0;
	};
}

UserTableDAO::~UserTableDAO()
{
}

void UserTableDAO::InsertUser(chaty::User* user)
{
	InsertUser(user->userName, user->passwd);
}

void UserTableDAO::InsertUser(QString& name, QString& passwd)
{
	auto helper = SqlHelper::GetInstance();
	helper->Open(CHATY_DB_NAME);
	helper->InsertData(CHATY_DB_USER_TABLE, QString("%1, %2").arg(name).arg(passwd));
	helper->Close();
}

chaty::User* UserTableDAO::GetUser(const QString name, const int id)
{
	if (name.isEmpty() && id == NULL)
	{
		XWAR("name and id all empty, query user failed.");
		return nullptr;
	}
	QString sql_where = name.isEmpty() ? QString("id = %1").arg(id) : QString("name = %1").arg(name);
	return nullptr;
}

chaty::User* UserTableDAO::GetUser(const QString name)
{
	chaty::User* pUser = new chaty::User;
	auto helper = SqlHelper::GetInstance();
	helper->Open(CHATY_DB_NAME);
	helper->Query(CHATY_DB_USER_TABLE, "id name passwd", QString("name = %1").arg(name), pUser, m_queryUserCB);
	helper->Close();
	return pUser;
}

chaty::User* UserTableDAO::GetUser(const int id)
{
	return nullptr;
}

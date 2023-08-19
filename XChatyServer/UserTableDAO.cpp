#include "UserTableDAO.h"
#include "XLog.h"
#include "chatyDef.h"
UserTableDAO::UserTableDAO()
{
	m_queryUserCB = [] (void* data, int argc, char** argv, char** colname) -> int
	{
		auto user = (chaty::User*)data;
		user->id = atoi(argv[0]);
		user->userName = argv[1];
		user->passwd = argv[2];
		XLOG(user->id, user->userName, user->passwd);
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
	QString columns = "name, passwd";
	auto helper = SqlHelper::GetInstance();
	helper->Open(CHATY_DB_NAME);
	helper->InsertData(CHATY_DB_USER_TABLE, columns, QString("%1, %2").arg(name).arg(passwd));
	helper->Close();
}

chaty::User* UserTableDAO::GetUser(const QString name)
{
	chaty::User* pUser = new chaty::User;
	auto helper = SqlHelper::GetInstance();
	helper->Open(CHATY_DB_NAME);
	helper->Query("UserTable", "id,name,passwd", QString("name = %1").arg(name), pUser, m_queryUserCB);
	helper->Close();
	return pUser;
}

chaty::User* UserTableDAO::GetUser(const int id)
{
	XLOG("GetUser");
	chaty::User* pUser = new chaty::User;
	auto helper = SqlHelper::GetInstance();
	helper->Open(CHATY_DB_NAME);
	helper->Query("UserTable", "id,name,passwd", QString("id=%1").arg(id), pUser, m_queryUserCB);
	helper->Close();
	return pUser;
}

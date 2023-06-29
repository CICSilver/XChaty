#pragma once
#include "chatyDef.h"
#include "SqlHelper.h"
class UserTableDAO
{
public:
	UserTableDAO();
	~UserTableDAO();
	void InsertUser(chaty::User* user);
	void InsertUser(QString& name, QString& passwd);
	chaty::User* GetUser(const QString name);
	chaty::User* GetUser(const int id);

private:
	chaty::Callback m_queryUserCB;
};


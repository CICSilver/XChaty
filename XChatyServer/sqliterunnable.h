#pragma once

#include <QRunnable>
#include "SqlHelper.h"
#include "chatyDef.h"
class SqliteRunnable  : public QRunnable
{
public:
	void run() override
	{
		SqlHelper* helper = SqlHelper::GetInstance();
		QString dbPath = chaty::GetDataBasePath(CHATY_DB_NAME);
		helper->Open(dbPath);
		
		helper->Close();
	}

private:

};

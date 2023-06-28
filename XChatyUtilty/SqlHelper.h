#pragma once
#include "sqlite3.h"
#include "hv.h"
#include <QString>
#include <functional>
#include "xchatyutilty_global.h"
#include "chatyDef.h"

using chaty::Callback;
class XCHATYUTILTY_EXPORT SqlHelper
{
public:
	static SqlHelper* GetInstance()
	{
		static SqlHelper instance;
		return &instance;
	}

	void Open(QString fileName)
	{
		QString dbPath = chaty::GetDataBasePath(fileName);
		Open(dbPath.toStdString().c_str());
	}

	void Open(const char* fileName)
	{
		int rc = sqlite3_open(fileName, &m_db);
		if (rc != SQLITE_OK)
			XLOG("open db failed!");
	}

	void Close()
	{
		if (m_db)
			sqlite3_close(m_db);
		m_db = nullptr;
	}

	void CreateTable(const QString& table, const QString& columns, void* data = nullptr, Callback callback = DefaultCB)
	{
		QString sql = QString("CREATE TABLE IF NOT EXISTS %1 (%2)").arg(table).arg(columns);
		Exec(sql, data, callback, "Error creating table : ");
	}

	void InsertData(const QString& table, const QString& values, void* data = nullptr, Callback callback = DefaultCB)
	{
		QString sql = QString("INSERT INTO %1 VALUES (%2)").arg(table).arg(values);
		Exec(sql, data, callback, "Error inserting data: ");
	}

	void Query(const QString& table, const QString& columns, const QString& _where, void* data = nullptr, Callback callback = DefaultCB)
	{
		QString sql = QString("SELECT %1 FROM %2 WHERE %3;").arg(columns).arg(table).arg(_where);
		Exec(sql, data, callback, "Error querying data: ");
	}

	void Exec(QString sql, void* data, Callback callback = DefaultCB, QString errStr = "Error executing sql: ")
	{
		int rc = sqlite3_exec(m_db, sql.toStdString().c_str(), callback, data, &m_errmsg);
		if (rc != SQLITE_OK) {
			XWAR(errStr, m_errmsg);
			sqlite3_free(m_errmsg);
		}
	}

	bool isOpen(QString DataName)
	{
		if (!m_db) return true;
		return sqlite3_db_filename(m_db, DataName.toStdString().c_str()) == NULL;
	}
private:
	SqlHelper() 
	{
		m_db = nullptr;
		m_errmsg = new char[1024];
	}
	~SqlHelper() 
	{
		SAFE_DELETE(m_errmsg);
	}
	static int DefaultCB(void* data, int argc, char** argv, char** colname) {
		// Ĭ�ϻص�����ӡִ�н��
		for (int i = 0; i < argc; i++) {
			XLOG(colname[i], " = ", argv[i]);
		}
		return 0;
	}
	sqlite3* m_db;
	char* m_errmsg;
};


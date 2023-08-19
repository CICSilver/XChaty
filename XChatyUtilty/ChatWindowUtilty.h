#pragma once

#include "xchatyutilty_global.h"
#include "hloop.h"
#include <QTextEdit>

class XCHATYUTILTY_EXPORT ChatWindowUtilty
{
public:
	static ChatWindowUtilty* GetInstance(QTextEdit* edit)
	{
		static ChatWindowUtilty instance(edit);
		return &instance;
	}
	~ChatWindowUtilty() 
	{
		SAFE_DELETE(m_unpackCfg);
	}

	void ShowMsg(const QString& str);

	void AppendMsg(const QString& str);

	void ClearMsg();

	void OverWriteMsg(const QString& str);

	void PostMsg(QObject* reciver, const QString& str);

	// 确保信息末尾有换行符
	inline void checkAndAddLineBreak(QString& str)
	{
		str.append(str.back() == "\n" ? "" : "\n");
	}

	unpack_setting_t* GetUnpackCfg(){return m_unpackCfg;}


private:
	ChatWindowUtilty(QTextEdit* edit) :m_chatEdit(edit)
	{
		if(!m_unpackCfg) m_unpackCfg = new unpack_setting_t;
		m_unpackCfg->mode = UNPACK_BY_DELIMITER;
		m_unpackCfg->package_max_length = DEFAULT_PACKAGE_MAX_LENGTH;
		m_unpackCfg->delimiter[0] = '\0';
		m_unpackCfg->delimiter_bytes = 1;
	};
	QTextEdit* m_chatEdit;
	unpack_setting_t* m_unpackCfg;
};

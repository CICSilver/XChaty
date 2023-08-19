#pragma once
#include "xchatyutilty_global.h"
//#include "chatyDef.h"
#include "baseMsg.h"
#include <QByteArray>
#include <QFile>
#include <QCryptographicHash>
#define BYTE_TO_KB(byte) (byte / 1024.0)
#define BYTE_TO_MB(byte) (BYTE_TO_KB(byte) / 1024.0)
#define BYTE_TO_GB(byte) (BYTE_TO_MB(byte) / 1024.0)

#define KB_TO_BYTE(kb) (kb * 1024)
#define MB_TO_BYTE(mb) (KB_TO_BYTE(mb) * 1024)
#define GB_TO_BYTE(gb) (MB_TO_BYTE(gb) * 1024)

namespace ftp
{
	struct FilePackageHead
	{
		FilePackageHead()
		{
			fileName = "";
			fileMd5 = QByteArray();
			packCount = 0;
		}
		QString fileName;
		QByteArray fileMd5;
		qint32 packCount;
	};
	typedef struct SubFilePackage
	{
		SubFilePackage()
		{
			fileData = QByteArray();
			packId = 0;
			md5 = QByteArray();
		}
		void SetData(QByteArray data, int id)
		{
			fileData = data;
			md5 = QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
			packId = id;
		}
		int operator>(const SubFilePackage& other)
		{
			return packId > other.packId;
		}
		int operator<(const SubFilePackage& other)
		{
			return packId < other.packId;
		}
		QByteArray fileData;
		qint32 packId;
		QByteArray md5;
	}FileSubPack;

	struct FilePackage
	{
		FilePackageHead head;
		QList<SubFilePackage> subPackList;
	};

	bool CompareSubFilePacks(FileSubPack a, FileSubPack b)
	{
		return a < b;
	}
}

class XCHATYUTILTY_EXPORT FileTransportUtilty
{
public:
	typedef ftp::FilePackage FilePack;
	typedef ftp::FilePackageHead FilePackHead;
	typedef ftp::SubFilePackage FileSubPack;
	typedef QList<FileSubPack> FilePackList;

	static FileTransportUtilty* GetInstance()
	{
		static FileTransportUtilty instance;
		return &instance;
	}

	// 打包整个文件
	FilePack PackFile(QString fileName);

	void UnPackFile(FilePack filePack);

private:
	FileTransportUtilty()
	{
		m_PakSize = MB_TO_BYTE(100);
	}
	~FileTransportUtilty() {};

	qint64 m_PakSize;
};


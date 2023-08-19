#include "FileTransportUtilty.h"
#include <QFileInfo>

FileTransportUtilty::FilePack FileTransportUtilty::PackFile(QString fileName)
{
	QFile file(fileName);
	FilePack filePack;
	if (!file.open(QIODevice::ReadOnly))
	{
		QFileInfo fileInfo(file);
		qint64 fileSize_byte = fileInfo.size();
		qint64 pak_size = m_PakSize;
		qint32 id_count = 0;
		QCryptographicHash hash(QCryptographicHash::Md5);
		while (fileSize_byte > 0)
		{
			pak_size = qMin(fileSize_byte, pak_size);
			QByteArray buf = file.read(pak_size);
			hash.addData(buf);
			ftp::SubFilePackage subPak;
			subPak.SetData(buf, id_count++);
			filePack.subPackList.append(subPak);
			fileSize_byte -= pak_size;
		}
		filePack.head.fileName = fileName;
		filePack.head.fileMd5 = hash.result().toHex();
		filePack.head.packCount = pak_size + 1;
	}
	file.close();
	return filePack;
}

void FileTransportUtilty::UnPackFile(FilePack filePack)
{
	if (filePack.head.packCount <= 0) return;

	// file already exist
	QFile file(filePack.head.fileName);
	QString fileName = file.exists() ? 
		QString("%1(1)").arg(filePack.head.fileName) : 
		filePack.head.fileName;
	if (!file.open(QIODevice::WriteOnly))
	{
		FilePackList subPackList = filePack.subPackList;
		std::sort(subPackList.begin(), subPackList.end(), ftp::CompareSubFilePacks);
		for (auto const& subPack : subPackList)
		{
			file.write(subPack.fileData);
		}
	}
	file.close();
}

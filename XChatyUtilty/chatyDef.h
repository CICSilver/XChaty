#pragma once
#include <qcoreapplication.h>
#include <QString>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDataStream>
#include <QDateTime>
#include <memory>

#define CHATY_DB_NAME "ChatDB.db"
#define CHATY_DB_USER_TABLE "UserTable"

namespace
{
	// 递归处理多个参数
	template<typename T>
	void handleArgs(QDebug& debug, T t) 
	{
		debug << t;
	}
	template<typename T, typename... Args>
	void handleArgs(QDebug& debug, T t, Args... args) 
	{
		debug << t << " ";
		handleArgs(debug, args...);
	}
}
#define __XCHATYLOG(x, str, ...) do { \
    QDebug debug = x().noquote().nospace(); \
    debug << __FILE__ << ":" << __LINE__ << "    "; \
    handleArgs(debug, ##__VA_ARGS__);     \
} while (0) 
#define XLOG(...) __XCHATYLOG(qDebug, "LOG: ", __VA_ARGS__)
#define XWAR(...) __XCHATYLOG(qWarning, "Warning: ", __VA_ARGS__)
#define XERR(...) __XCHATYLOG(qCritical, "Error: ", __VA_ARGS__)
#define CURRENT_TR(a) toUnicode(a)
#define PrintString(a)  qUtf8Printable(a)
#define PrintChinese(a) PrintString(CURRENT_TR(a))

namespace chaty_client
{
	typedef enum RequestType
	{
		REQ_LOGIN = 0,
		REQ_REGIST = 2,
		REQ_CHAT = 4,
		REQ_DEFAULT = 254,	
		REQ_HB = 255	// Max of 8 bit unsigned
	}ReqType;
}// end of namespace chaty_client

namespace chaty_server
{
	typedef enum ResponseType
	{
		RESP_LOGIN_OK = 0,
		RESP_LOGIN_FAILED,
		RESP_REGIST_OK,
		RESP_REGIST_FAILED,
		RESP_CHAT_OK,
		RESP_CHAT_FAILED,
		RESP_DEFAULT_OK = 254,
		RESP_DEFAULT_FAILED = 255	// Max of 8 bit unsigned
	};
}// end of namespace chaty_server

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

	static QDataStream& operator<<(QDataStream& stream, const UserStruct& user) {
		stream << user.userName << user.passwd << user.id;
		return stream;
	}
	static QDataStream& operator>>(QDataStream& stream, UserStruct& user) {
		stream >> user.userName >> user.passwd >> user.id;
		return stream;
	}


}// end namespace chaty

namespace protochat
{
	typedef struct MessageBase
	{
		MessageBase(QString customMsg = "")
			:custom_msg(customMsg) {}
		virtual ~MessageBase() {}

		virtual std::unique_ptr<MessageBase> clone() = 0;
		QString custom_msg;
	}BaseMsg;
	typedef struct LoginMsg : public BaseMsg
	{
		LoginMsg() :user() {}
		LoginMsg(const LoginMsg& _msg)
			: user(_msg.user) {}
		LoginMsg(chaty::User _user)
		{
			user = _user;
		}
		std::unique_ptr<BaseMsg> clone() override
		{
			return std::make_unique<LoginMsg>(*this);
		}
		LoginMsg& operator=(const LoginMsg& _msg)
		{
			this->user = _msg.user;
			return *this;
		}
		chaty::User user;
	}RegistMsg;

	typedef struct ClientMessage: public BaseMsg
	{
		// login message from client
		ClientMessage()
		{
			userName = "";
			chatMsg = "";
			chatRoom = 0;
		}
		ClientMessage(const ClientMessage& msg)
			: userName(msg.userName), chatMsg(msg.chatMsg), chatRoom(msg.chatRoom) {}
		std::unique_ptr<BaseMsg> clone() override
		{
			return std::make_unique<ClientMessage>(*this);
		}
		ClientMessage& operator=(const ClientMessage& _msg)
		{
			this->userName = _msg.userName;
			this->chatMsg = _msg.chatMsg;
			this->chatRoom = _msg.chatRoom;
			return *this;
		}
		QString userName;
		QString chatMsg;
		int chatRoom;
	}ChatMsg;

	struct BaseHead
	{
		BaseHead()
		{
			time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
		}
		QString time;
		quint8 msgType;
	};

	struct chatyHead : public BaseHead
	{
		chatyHead(chaty_client::RequestType _type = chaty_client::REQ_DEFAULT)
		{
			this->msgType = _type;
		}
	};

	typedef struct ResponseHead : public BaseHead
	{
		ResponseHead(chaty_server::ResponseType _type = chaty_server::RESP_DEFAULT_OK)
		{
			this->msgType = _type;
		}
	}RespHead;

	typedef struct ResponseMessage : public BaseMsg
	{
		ResponseMessage()
			:respHead() {}
		ResponseMessage(chaty_server::ResponseType _type, QString custom_str= "")
		{
			this->respHead.msgType = _type;
			this->custom_msg = custom_str;
		}
		std::unique_ptr<BaseMsg> clone() override
		{
			return std::make_unique<ResponseMessage>(*this);
		}
		ResponseHead respHead;
	}RespMsg;

	struct ChatyMessage
	{
		ChatyMessage()
			:msgHead(), pMsgBody(Q_NULLPTR) {}
		ChatyMessage(chaty_client::RequestType _type, BaseMsg *msg_body = Q_NULLPTR)
		{
			msgHead.msgType = _type;
			switch (_type)
			{
				case chaty_client::REQ_LOGIN:
				case chaty_client::REQ_REGIST:
				{
					RegistMsg* msg = dynamic_cast<RegistMsg*>(msg_body);
					pMsgBody = std::make_unique<RegistMsg>(*msg);
					break;
				}
				case chaty_client::REQ_CHAT:
				{
					ChatMsg* msg = dynamic_cast<ChatMsg*>(msg_body);
					pMsgBody = std::make_unique<ChatMsg>(*msg);
					break;
				}
				case chaty_client::REQ_HB:
				{
					pMsgBody = nullptr;
				}
				default:
					break;
			}
		}
		ChatyMessage(const ChatyMessage& msg)
			: msgHead(msg.msgHead)
			, pMsgBody(msg.pMsgBody ? msg.pMsgBody->clone() : Q_NULLPTR) {}
		void Init()
		{
			pMsgBody = nullptr;
			msgHead.msgType = chaty_client::REQ_DEFAULT;
			msgHead.time = "";
		}
		~ChatyMessage() {}
		std::unique_ptr<BaseMsg> pMsgBody;
		chatyHead msgHead;
	};

	// 序列化
	static QDataStream& operator<<(QDataStream& stream, const ClientMessage& msg) {
		stream << msg.userName << msg.chatMsg << msg.chatRoom;
		return stream;
	}
	static QDataStream& operator<<(QDataStream& stream, const LoginMsg& msg) {
		stream << msg.user;
		return stream;
	}
	static QDataStream& operator<<(QDataStream& stream, const chatyHead& msg) {
		stream << msg.msgType << msg.time;
		return stream;
	}
	static QDataStream& operator<<(QDataStream& stream, ChatyMessage& msg)
	{
		stream << msg.msgHead;
		switch (msg.msgHead.msgType)
		{
			case chaty_client::REQ_LOGIN:
			case chaty_client::REQ_REGIST:
			{
				XLOG("Login/Regist");
				LoginMsg* login_msg = dynamic_cast<LoginMsg*>(msg.pMsgBody.get());
				stream << login_msg->user << login_msg->custom_msg;
			}
			case chaty_client::REQ_CHAT:
			{
				XLOG("Chat");
				ChatMsg* chat_msg = dynamic_cast<ChatMsg*>(msg.pMsgBody.get());
				stream << chat_msg->userName << chat_msg->chatMsg << chat_msg->chatRoom;
			}
			default:
				break;
		}
		XLOG("ChatyMessage seriallize done");
		return stream;
	}

	// 反序列化
	static QDataStream& operator>>(QDataStream& stream, ClientMessage& msg) {
		stream >> msg.userName >> msg.chatMsg >> msg.chatRoom;
		return stream;
	}
	static QDataStream& operator>>(QDataStream& stream, LoginMsg& msg) {
		stream >> msg.user;
		return stream;
	}
	static QDataStream& operator>>(QDataStream& stream, chatyHead& msg) {
		stream >> msg.msgType >> msg.time;
		XLOG(msg.msgType, msg.time);
		return stream;
	}
	static QDataStream& operator>>(QDataStream& stream, ChatyMessage& msg)
	{
		stream >> msg.msgHead;
		XLOG(msg.msgHead.msgType);
		switch (msg.msgHead.msgType)
		{
			case chaty_client::REQ_LOGIN:
			case chaty_client::REQ_REGIST:
			{
				RegistMsg _msg_body;
				stream >> _msg_body;
				msg.pMsgBody = std::make_unique<RegistMsg>(_msg_body);
				break;
			}
			case chaty_client::REQ_CHAT:
			{
				ChatMsg _msg_body;
				stream >> _msg_body;
				msg.pMsgBody = std::make_unique<ChatMsg>(_msg_body);
				break;
			}
			default:
				break;
		}
		return stream;
	}

	static QByteArray Serrialize(protochat::ChatyMessage msg)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream << msg;
		XLOG(msg.msgHead.msgType);
		return buffer;
	};
	static protochat::ChatyMessage DeSerrialize(const QByteArray& ba)
	{
		QByteArray _temp_ba(ba);
		QDataStream stream(&_temp_ba, QIODevice::ReadOnly);
		protochat::ChatyMessage msg;
		msg.Init();
		stream >> msg;
		return msg;
	}
}// end namespace msgProto
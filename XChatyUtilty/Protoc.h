#pragma once

#include <QByteArray>
#include <QDataStream>

#include "UserModel.h"
#include "msgType.h"
#include "baseMsg.h"
#include "XLog.h"
#include "hv.h"
// ========================== MSG ==========================
namespace protochat
{
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

	typedef struct ClientMessage : public BaseMsg
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
		ResponseMessage(chaty_server::ResponseType _type, QString custom_str = "")
		{
			this->respHead.msgType = _type;
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
		ChatyMessage(chaty_client::RequestType _type, BaseMsg* msg_body = Q_NULLPTR)
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
}

// ========================== Serilize/Unserilize ==========================
namespace protochat 
{
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
				stream << login_msg->user;
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
}

// ========================== msg funcs ====================================
namespace protochat
{
	// convert from hv::Buffer to ChatyMessage
	static ChatyMessage ConvertBuf2ChatyMsg(hv::Buffer* buf)
	{
		QByteArray qbuffer((char*)buf->data(), buf->size());
		ChatyMessage msg = protochat::DeSerrialize(qbuffer);
		return msg;
	}
}
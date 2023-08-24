#pragma once
#ifndef MSGTYPE_H
#define MSGTYPE_H

namespace protoc
{
	typedef enum RequestType
	{
		REQ_LOGIN = 0,
		REQ_REGIST = 2,
		REQ_CHAT = 4,
		REQ_SEND_FILE = 5,
		REQ_DEFAULT = 254,
		REQ_HB = 255	// Max of 8 bit unsigned
	}ReqType;

	enum ResponseType
	{
		RESP_LOGIN_SUCCESS = 0,
		RESP_LOGIN_FAILED,
		RESP_REGIST_SUCCESS,
		RESP_REGIST_FAILED,
		RESP_CHAT_SUCCESS,
		RESP_CHAT_FAILED,
		RESP_DEFAULT_SUCCESS = 254,
		RESP_DEFAULT_FAILED = 255	// Max of 8 bit unsigned
	};
}// end of namespace chaty_server
#endif // MSGTYPE_H

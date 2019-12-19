// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// TCP client session for transferring data.
//

#ifndef TCP_CLIENT_SESSION_H
#define TCP_CLIENT_SESSION_H

#include "boost/asio.hpp"
#include "boost/function.hpp"

namespace framework
{
	namespace network
	{
		typedef enum class tagTCPClientEvent_t : unsigned char
		{
			TCP_CLIENT_EVENT_NONE = 0,
			TCP_CLIENT_EVENT_CONNECTED,
			TCP_CLIENT_EVENT_DISCONNECTED,
			TCP_CLIENT_EVENT_READ
		}TCPClientEvent;

		typedef boost::function<
			void(const int, const unsigned char, const unsigned char*, const unsigned long long, void*)> PostMediaStreamDataCallback;

		class TCPClientSession
		{
		public:
			TCPClientSession(
				const int ID = 0, PostMediaStreamDataCallback callback = NULL, void* ctx = NULL);
			virtual ~TCPClientSession(void);

		public:
			int createNewSession(boost::asio::io_service& service, const std::string url);
			int destroySession(void);

// 		protected:
// 			void receivingData(
// 				boost::system::error_code error, std::size_t transferBytes, const char* transferData = nullptr) override;
// 			void settingTimeout(const UInt32 timeo = 0) override;

		private:
			void postConnectedCallback(boost::asio::ip::tcp::socket* s, boost::system::error_code e);

		private:
			const int sessionID;
			std::string streamUrl;
			PostMediaStreamDataCallback postMediaStreamDataCallback;
			void* userData;
		};//class TCPClientSession
	}//namespace network
}//namespace framework

#endif//TCP_CLIENT_SESSION_H

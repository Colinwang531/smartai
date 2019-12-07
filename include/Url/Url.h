// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Stream URL wrapper.
// URL sample :
//				livestream://admin:12345@192.168.1.100:8000?channel=1&stream=hikvision[&width=1920&height=1080&...]
//				playback://?path=c:\video\sample.mp4
//				rtsp://admin:12345@192.168.1.100:8000?channel=1&...
//

#ifndef URL_H
#define URL_H

#include <string>

namespace framework
{
	namespace wrapper
	{
		typedef enum class tagURLProtocol_t
		{
			URL_PROTOCOL_NONE = 0,
			URL_PROTOCOL_LIVESTREAM,
			URL_PROTOCOL_PLAYBACK,
			URL_PROTOCOL_RTSP
		}URLProtocol;

		class URL
		{
		public:
			URL(void);
			~URL(void);

		public:
			inline void setUrl(const std::string url)
			{
				this->url = url;
			}
			inline const std::string getUrl(void) const
			{
				return url;
			}
			void setProtocol(const URLProtocol type = URLProtocol::URL_PROTOCOL_NONE);
			const URLProtocol getProtocol(void) const;
			void setAddressPort(const std::string& address, const unsigned short port = 0);
			void getAddressPort(std::string& address, unsigned short& port);
			void addAuthentication(const std::string& name, const std::string& password);
			void getAuthentication(std::string& name, std::string& password);
			void addParameter(const std::string& key, const std::string& value);
			const std::string getParameter(const std::string& key);

		private:
			std::string url;
		};//class URL
	}//namespace wrapper
}//namespace framework

#endif//URL_H

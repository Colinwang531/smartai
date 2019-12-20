// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Stream URL wrapper.
// URL sample :
//				livestream://admin:12345@192.168.1.100:8000?channel=1&stream=0&status=1[&sms=192.168.1.100:8000]
//						@stream : 1-HIKVISION_PRIVATE, 
//								  2-HIKVISION_PS, 
//								  3-DAHUA_PRIVATE, 
//								  4-DAHUA_PS,
//								  5-H264, 
//								  6-H265, 
//								  7-RTP/RTSP.
//						@status : 1-Open stream,
//								  2-Close stream.
//				playback://admin:12345@192.168.1.100:8000?channel=1&stream=0&status=1[&start=20191211000000&stop=20191211235959]
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
		typedef enum class tagURLProtocol_t : unsigned char
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
			URL(const std::string s);
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

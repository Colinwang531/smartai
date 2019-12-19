//
//		Copyright :				@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2018-04-11
//		Description:				RTP流会话
//
//		History:						Author									Date											Description
//											王科威									2018-04-11									创建
//

#ifndef RTP_STREAM_SESSION_H
#define RTP_STREAM_SESSION_H

#include "Network/Session.h"
using ReusableSession = NS(network, 1)::ReusableSession;
using NonreusableSession = NS(network, 1)::NonreusableSession;

NS_BEGIN(media_stream, 1)

class RTPRealplaySession final : public ReusableSession
{
public:
	RTPRealplaySession(void);
	~RTPRealplaySession(void);

protected:
	int createDataReceiverWithAddressAndPort(const std::string& address, const UInt16 port = 10000) override;
	int createDataSenderWithAddressAndPort(const std::string& address, const UInt16 port = 10000) override;
};//class RTPRealplaySession

class RTPPlaybackSession final : public NonreusableSession
{
public:
	RTPPlaybackSession(void);
	~RTPPlaybackSession(void);

protected:
	int createDataReceiverWithAddressAndPort(const std::string& address, const UInt16 port = 10000) override;
	int createDataSenderWithAddressAndPort(const std::string& address, const UInt16 port = 10000) override;
};//class RTPPlaybackSession

NS_END

#endif//RTP_STREAM_SESSION_H

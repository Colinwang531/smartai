#include "MediaStream/RTPStreamSession.h"

NS_BEGIN(media_stream, 1)

RTPRealplaySession::RTPRealplaySession() : ReusableSession()
{}

RTPRealplaySession::~RTPRealplaySession()
{}

int RTPRealplaySession::createDataReceiverWithAddressAndPort(const std::string& address, const UInt16 port /*= 10000*/)
{

}

int RTPRealplaySession::createDataSenderWithAddressAndPort(const std::string& address, const UInt16 port /*= 10000*/)
{

}

RTPPlaybackSession::RTPPlaybackSession() : NonreusableSession()
{}

RTPPlaybackSession::~RTPPlaybackSession()
{}

int RTPPlaybackSession::createDataReceiverWithAddressAndPort(const std::string& address, const UInt16 port /*= 10000*/)
{

}

int RTPPlaybackSession::createDataSenderWithAddressAndPort(const std::string& address, const UInt16 port /*= 10000*/)
{

}

NS_END

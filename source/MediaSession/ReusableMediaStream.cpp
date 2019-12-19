#include "MediaStream/ReusableMediaStream.h"

NS_BEGIN(media, 2)

ReusableMediaStream::ReusableMediaStream()
{}

ReusableMediaStream::~ReusableMediaStream()
{}

Int32 ReusableMediaStream::addReceiverSession(TCPSessionPtr sessionPtr)
{
	Int32 status{ ERR_INVALID_PARAM };

	if (sessionPtr)
	{
		status = receiverSessionPtrGroup.Insert(sessionPtr) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

Int32 ReusableMediaStream::removeReceiverSession(TCPSessionPtr sessionPtr)
{
	Int32 status{ ERR_INVALID_PARAM };

	if (sessionPtr && receiverSessionPtrGroup.Remove(sessionPtr))
	{
		status = 0 < receiverSessionPtrGroup.Size() ? ERR_OK : ERR_EMPTY;
	}

	return status;
}

Int32 ReusableMediaStream::removeAllReceiverSession(void)
{
	std::vector<TCPSessionPtr> tempReceiverSessionPtrGroup{ receiverSessionPtrGroup.Data() };
	for (std::vector<TCPSessionPtr>::iterator it = tempReceiverSessionPtrGroup.begin(); it != tempReceiverSessionPtrGroup.end(); ++it)
	{
		(*it)->stopSession();
	}

	return ERR_OK;
}

Int32 ReusableMediaStream::sendData(const char* data /*= nullptr*/, const Int32 bytes /*= 0*/)
{
	if (data && 0 < bytes)
	{
		std::vector<TCPSessionPtr> tempReceiverSessionPtrGroup{ receiverSessionPtrGroup.Data() };
		for (std::vector<TCPSessionPtr>::iterator it = tempReceiverSessionPtrGroup.begin(); it != tempReceiverSessionPtrGroup.end(); ++it)
		{
			if ((*it)->opened())
			{
				(*it)->sendData(data, bytes);
			}
			else
			{
				receiverSessionPtrGroup.Remove(*it);
			}
		}
	}

	return 0 < receiverSessionPtrGroup.Size() ? ERR_OK : ERR_EMPTY;
}

NS_END

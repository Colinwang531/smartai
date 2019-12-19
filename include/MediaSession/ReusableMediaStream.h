//
//		Copyright :					@2018, HHJT, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2018-06-27
//		Description:					�ɸ�����
//
//		History:						Author									Date													Description
//											������									2018-06-27									����
//

#ifndef REUSABLE_MEDIA_STREAM_H
#define REUSABLE_MEDIA_STREAM_H

#include "boost/shared_ptr.hpp"
#include "DataStructure/Vector.h"
#include "ASIO/TCPSession.h"
using TCPSessionPtr = boost::shared_ptr<NS(asio, 2)::TCPSession>;
using ReceiverSessionPtrGroup = NS(vector, 1)::Vector<TCPSessionPtr>;

NS_BEGIN(media, 2)

class ReusableMediaStream
{
public:
	ReusableMediaStream(void);
	~ReusableMediaStream(void);

	//	���� :				��Ӹ��������ն�
	//
	//	���� :				sessionPtr													[in]																���ն����Ự
	//
	//	����ֵ :				������
	//
	//	��ע :
	//
	Int32 addReceiverSession(TCPSessionPtr sessionPtr);

	//	���� :				�رո��������ն�
	//
	//	���� :				sessionPtr													[in]																���ն����Ự
	//
	//	����ֵ :				������
	//
	//	��ע :
	//
	Int32 removeReceiverSession(TCPSessionPtr sessionPtr);

	//	���� :				�ر����и��������ն�
	//
	//	���� :
	//
	//	����ֵ :				������
	//
	//	��ע :
	//
	Int32 removeAllReceiverSession(void);

	//	���� :				���ݷ���
	//
	//	���� :				data																[in]																����
	//							bytes															[in]																�ֽ���
	//
	//	����ֵ :				������
	//
	//	��ע :
	//
	Int32 sendData(const char* data = nullptr, const Int32 bytes = 0);

protected:
	ReceiverSessionPtrGroup receiverSessionPtrGroup;
};//class ReusableMediaStream

NS_END

#endif//REUSABLE_MEDIA_STREAM_H

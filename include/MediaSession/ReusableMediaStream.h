//
//		Copyright :					@2018, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2018-06-27
//		Description:					可复用流
//
//		History:						Author									Date													Description
//											王科威									2018-06-27									创建
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

	//	功能 :				添加复用流接收端
	//
	//	参数 :				sessionPtr													[in]																接收端流会话
	//
	//	返回值 :				错误码
	//
	//	备注 :
	//
	Int32 addReceiverSession(TCPSessionPtr sessionPtr);

	//	功能 :				关闭复用流接收端
	//
	//	参数 :				sessionPtr													[in]																接收端流会话
	//
	//	返回值 :				错误码
	//
	//	备注 :
	//
	Int32 removeReceiverSession(TCPSessionPtr sessionPtr);

	//	功能 :				关闭所有复用流接收端
	//
	//	参数 :
	//
	//	返回值 :				错误码
	//
	//	备注 :
	//
	Int32 removeAllReceiverSession(void);

	//	功能 :				数据发送
	//
	//	参数 :				data																[in]																数据
	//							bytes															[in]																字节数
	//
	//	返回值 :				错误码
	//
	//	备注 :
	//
	Int32 sendData(const char* data = nullptr, const Int32 bytes = 0);

protected:
	ReceiverSessionPtrGroup receiverSessionPtrGroup;
};//class ReusableMediaStream

NS_END

#endif//REUSABLE_MEDIA_STREAM_H

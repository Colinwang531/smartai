//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					王科威
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-09-12
//		Description:				消息队列工作者模型
//
//		History:					Author										Date												Description
//										王科威										2017-09-12								创建
//

#ifndef WORKER_MODEL_H
#define WORKER_MODEL_H

#include "MQModel/MQModel.h"

NS_BEGIN(model, 1)

class WorkerModel
{
public:
	TerminatorModel(void);
	virtual ~TerminatorModel(void);

	//	功能 : 启动/停止终端模型
	//
	//	参数 : 
	//			  @ctx [IN] MQ上下文
	//			  @rAddr [IN] 服务端地址
	//			  @tid [IN] 终端ID标识
	//
	//	返回值 :
	//
	//	备注 :
	//
	Int32 startModel(MQContext& ctx, const std::string& rAddr, const std::string& tid);
	void stopModel(MQContext& ctx);
	Int32 sendMessage(const std::string& msg);

protected:
	virtual Int32 connectingDealer(MQContext& ctx, const std::string& rAddr, const std::string& tid);
// 	int backendListen(std::string&&) override;
	virtual Int32 sendingMessage(const std::string& msg);

private:
	static void receiverThreadHandler(HANDLE ctx = nullptr);

private:
	MQThread receiveDataThread;
	SOCKET dealerSocket;
};//class TerminatorModel

NS_END

#endif//WORKER_MODEL_H

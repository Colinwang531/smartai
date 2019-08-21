//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					������
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-09-12
//		Description:				��Ϣ���й�����ģ��
//
//		History:					Author										Date												Description
//										������										2017-09-12								����
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

	//	���� : ����/ֹͣ�ն�ģ��
	//
	//	���� : 
	//			  @ctx [IN] MQ������
	//			  @rAddr [IN] ����˵�ַ
	//			  @tid [IN] �ն�ID��ʶ
	//
	//	����ֵ :
	//
	//	��ע :
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

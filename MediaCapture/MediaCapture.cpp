// MediaCapture.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "boost/bind.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "Hardware/Cpu.h"
using CPU = framework::hardware::Cpu;
#include "Network/Asio/TCPServer.h"
using TCPServer = framework::network::TCPServer;
using TCPPacket = framework::network::TCPPacket;

static void postReadTCPPacketCallback(const TCPPacket& pkt)
{
	std::string eventName;

	if (framework::network::TCPEvent::TCP_EVENT_ACCEPT == pkt.event)
	{
		eventName.append("ACCEPT");
	}
	else if (framework::network::TCPEvent::TCP_EVENT_READ == pkt.event)
	{
		eventName.append("READ");
	}

	LOG(INFO) << eventName << " " << pkt.ID << " " << pkt.data << ".";
}

int main(int argc, char* argv[])
{
	FLAGS_stderrthreshold = GLOG_INFO;
	FLAGS_colorlogtostderr = 1;
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(GLOG_INFO,
#ifdef _WINDOWS
		".\\"
#else
		"./"
#endif
	);

	TCPServer server{ boost::bind(&postReadTCPPacketCallback, _1) };
	server.startServer();
	getchar();
	server.stopServer();
	google::ShutdownGoogleLogging();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

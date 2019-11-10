// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "boost/process.hpp"

//#define CHILD_PROCESS

int main(int argc, char* argv[])
{
#ifdef CHILD_PROCESS
	std::cout << "Child process start, ID = [ " << GetCurrentProcessId() << " ]." << std::endl;
	std::cout << "Child process name is [ " << argv[0] << " ]." << std::endl;
	std::cout << "Child process parameter is [ " << argv[1] << " ]." << std::endl;
	std::cout << "Child process stop." << std::endl;

	getchar();
#else
	std::cout << "Parent process start, ID = [ " << GetCurrentProcessId() << " ]." << std::endl;
//	int result{ boost::process::system("./ConsoleApplication1_child.exe", "Nice to meet you, Colin !") };
//	boost::process::spawn("./ConsoleApplication1_child.exe", "Nice to meet you, Colin !");
	boost::process::child c("./ConsoleApplication1_child.exe", "Nice to meet you, Colin !");
	c.detach();
	std::cout << "Child process exit code is [ " << c.exit_code() << " ]." << std::endl;
	std::cout << "Parent process stop." << std::endl;

	getchar();
#endif//CHILD_PROCESS

	return 1;
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

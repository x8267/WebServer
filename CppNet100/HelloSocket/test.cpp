#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>	//windowssocket头文件

//#pragma comment(lib, "ws2_32.lib")	//引用win32socket动态库

#include <iostream>

using namespace std;

int main()
{
	//启动Windows socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//------------
	//--用socket API建立简易TCP客户端
	// 1 建立一个socket
	// 2 连接服务器 connect
	// 3 接收服务器信息 recv
	// 4 关闭套接字 closesocket


	//-------------
	//清除windows socket环境


	WSACleanup();
	return 0;
}
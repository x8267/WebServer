#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS


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
	//--用socket API建立简易TCP服务器
	// 1 建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	//ip地址类型  数据传输方式  传输协议
	if (INVALID_SOCKET == _sock)
	{
		cout << "create server sock error!" << endl;
	}
	else
	{
		cout << "create server sock success!" << endl;
	}

	// 2 bind绑定用于接受客户端连接的网络端口
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);	//host to net unsigned short将主机序转换为网络序
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;	//随机指定一个外网地址
	//sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	//定义访问的ip地址，如果是内网的只能内网访问
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)(&_sin), sizeof(sockaddr_in)))	//有可能绑定失败，判断
	{
		cout << "bind error!" << endl;
	}
	else
	{
		cout << "bind success!" << endl;
	}

	// 3 listen监听网络接口
	//listen(_sock, 5);	//5为请求队列的最大长度，能存放多少个客户端请求
	if (SOCKET_ERROR == listen(_sock, 5))
	{
		cout << "listen error!" << endl;
	}
	else
	{
		cout << "listen success!" << endl;
	}

	// 4 accept等待接收客户端连接
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;

	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock)
	{
		cout << "accept error!" << endl;
	}
	cout << "accept new client, " << "sock is: " << _cSock << " ip is: " << inet_ntoa(clientAddr.sin_addr) << endl;

	char _recvBuf[256] = {};
	while (true)
	{
		// 5 接收客户端请求
		int nlen = recv(_cSock, _recvBuf, 256, 0);
		if (nlen <= 0)
		{
			cout << "recv error!" << endl;
			cout << "server has been stop!" << endl;
			break;
		}
		else
		{
			cout << "recv client msg: " << _recvBuf << endl;
		}

		// 6 处理客户端请求
		if (0 == strcmp(_recvBuf, "getName"))
		{
			char Buff[] = "I'm server.";
			// 7 send向客户端发送一条数据
			send(_cSock, Buff, strlen(Buff) + 1, 0);
		}
		else if (0 == strcmp(_recvBuf, "getAge"))
		{
			char Buff[] = "23";
			// 7 send向客户端发送一条数据
			send(_cSock, Buff, strlen(Buff) + 1, 0);
		}
		else
		{
			char Buff[] = "??";
			send(_cSock, Buff, strlen(Buff) + 1, 0);
		}
	}
	
	closesocket(_sock);

	// 8 关闭套接字closesocket

	//-------------
	//清除windows socket环境


	WSACleanup();
	cout << "server has been stop!" << endl;
	system("pause");
	return 0;
}
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>	//windowssocket头文件

//#pragma comment(lib, "ws2_32.lib")	//引用win32socket动态库

#include <iostream>

using namespace std;

struct DataPackage
{
	int age;
	char name[32];
};

int main()
{
	//启动Windows socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//------------
	//--用socket API建立简易TCP客户端
	// 1 建立一个socket
	SOCKET _csock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _csock)
	{
		cout << "create client sock error!" << endl;
	}
	else
	{
		cout << "create client sock success!" << endl;
	}

	// 2 连接服务器 connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_csock, (sockaddr*)(&_sin), sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		cout << "connect error!" << endl;
	}
	else
	{
		cout << "connect success!" << endl;
	}

	while (true)
	{
		// 3 输入请求命令
		cout << "Please input your operation: " << endl;
		char inputBuff[256] = {};
		cin >> inputBuff;
		// 4 处理请求
		if (0 == strcmp(inputBuff, "exit"))
		{
			cout << "client has been stop!" << endl;
			break;
		}
		else
		{
			// 5 向服务器发送请求
			send(_csock, inputBuff, strlen(inputBuff) + 1, 0);
		}

		// 6 接收服务器信息 recv
		char recvBuff[256] = {};
		int nlen = recv(_csock, recvBuff, 256, 0);
		if (nlen <= 0)
		{
			cout << "recv error!\n";
		}
		else
		{
			DataPackage* dp = (DataPackage*)recvBuff;
			cout << "recv data: \n" << "age = "<< dp->age << " name = " << dp->name << endl;
		}
	}

	// 4 关闭套接字 closesocket
	closesocket(_csock);

	//-------------
	//清除windows socket环境


	WSACleanup();
	cout << "client has been stop!" << endl;
	system("pause");
	return 0;
}
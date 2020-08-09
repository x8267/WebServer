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

enum CMD
{
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_ERROR
};
//消息头部
struct DataHeader
{
	short dataLength;	//数据长度
	short cmd;	//命令
};
//登录
struct Login
{
	char userName[32];
	char passWord[32];
};
//登录结果
struct LoginResult
{
	int result;
};
//登出
struct Logout
{
	char userName[32];
};
//登出结果
struct LogoutResult
{
	int result;
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
		cout << "Please input your operator: " << endl;
		char inputBuff[256] = {};
		cin >> inputBuff;
		// 4 处理请求
		if (0 == strcmp(inputBuff, "exit"))
		{
			cout << "client has been stop!" << endl;
			break;
		}
		else if (0 == strcmp(inputBuff, "login"))
		{
			// 5 向服务器发送请求
			Login login = {"yang", "yang123"};
			DataHeader dh = { sizeof(login), CMD_LOGIN };
			//strcpy(login.userName, "yang");
			//strcpy(login.passWord, "yang123");
			send(_csock, (const char*)&dh, sizeof(DataHeader), 0);
			send(_csock, (const char*)&login, sizeof(login), 0);
			// 6 接收服务器返回的信息 recv
			DataHeader retHeader = {};
			LoginResult loginRet = {};
			recv(_csock, (char*)&retHeader, sizeof(DataHeader), 0);
			recv(_csock, (char*)&loginRet, sizeof(loginRet), 0);
			cout << "login result is: " << loginRet.result << endl;
		}
		else if (0 == strcmp(inputBuff, "logout"))
		{
			// 5 向服务器发送请求
			Logout logout = { "yang" };
			DataHeader dh = { sizeof(logout), CMD_LOGOUT };
			send(_csock, (const char*)&dh, sizeof(dh), 0);
			send(_csock, (const char*)&logout, sizeof(logout), 0);
			// 6 接收服务器返回的信息 recv
			DataHeader retHeader = {};
			LogoutResult logoutRet = {};
			recv(_csock, (char*)&retHeader, sizeof(retHeader), 0);
			recv(_csock, (char*)&logoutRet, sizeof(logoutRet), 0);
			cout << "logout result is: " << logoutRet.result << endl;
		}
		else
		{
			cout << "input error, please input again!" << endl;
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
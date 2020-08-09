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
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};
//消息头部
struct DataHeader
{
	short dataLength;	//数据长度
	short cmd;	//命令
};
//登录
struct Login : public DataHeader
{
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};
//登录结果
struct LoginResult : public DataHeader
{
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 1;
	}
	int result;
};
//登出
struct Logout : public DataHeader
{
	Logout()
	{
		dataLength = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
//登出结果
struct LogoutResult : public DataHeader
{
	LogoutResult()
	{
		dataLength = sizeof(LogoutResult);
		cmd = CMD_LOGOUT_RESULT;
		result = 1;
	}
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
			Login login;
			strcpy(login.userName, "yang");
			strcpy(login.passWord, "yang123");
			send(_csock, (const char*)&login, sizeof(login), 0);
			// 6 接收服务器返回的信息 recv
			LoginResult loginRet = {};
			recv(_csock, (char*)&loginRet, sizeof(loginRet), 0);
			cout << "login result is: " << loginRet.result << endl;
		}
		else if (0 == strcmp(inputBuff, "logout"))
		{
			// 5 向服务器发送请求
			Logout logout;
			strcpy(logout.userName, "yang");
			send(_csock, (const char*)&logout, sizeof(logout), 0);
			// 6 接收服务器返回的信息 recv
			LogoutResult logoutRet = {};
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
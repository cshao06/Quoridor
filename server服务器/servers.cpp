#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT	3579 //服务器将要和客户端连接的端口
#define BUFFLEN 256
#define READY	9

struct bans
{
	int x; //隔板左上角坐标
	int y; 
	int a1;//隔板所占三格中左或上一个的数组坐标
	int b1;
	int a2;//同上，中间
	int b2;
	int a3;//同上，右或下
	int b3;
	int d;  //-1横1竖
};

struct pack
{
	char flag;
	char sender;
	char buff[BUFFLEN];
	int px, py;
	int a, b;
	struct bans buffban;
};

int err;
//管理客户端数
char num;
int	countusername = 0;
int	countconnect = 0;
int newclient = 0;

//为服务器端建立一个名为ListenSocket的SOCKET对象用来监听客户端的连接
SOCKET ListenSocket; 	

//客户端结构体
struct clients
{
	WSAOVERLAPPED	overlap;//必须放在第一个，因为完成例程中要将这个指针转换成结构体的指针
	char		clientNo;
	char		username[20];
	SOCKET		Socket;
	struct		sockaddr_in addr;
	struct pack	recvbuff;
	WSABUF		buffer;
	DWORD		BytesRecvd;
	DWORD		Flags;
} client[5] = {{NULL}};

int ready[5];

int cliaddrlen = sizeof (client[1].addr);

/*BOOL WINAPI ConsoleHandler (DWORD CEvent)
{
	if (CEvent == CTRL_CLOSE_EVENT)
	{
		err = MessageBox (NULL,"确定要退出服务器？","退出服务器",MB_YESNO);
		if (err == IDYES)
		{
			SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, FALSE);
		}
	}
	return TRUE;
}*/

void error (struct clients *pt)
{
	closesocket (pt->Socket);//关闭这个套接字
	if (pt->clientNo != '\0')
	{
		countconnect -= 1;
	}
	if (pt->username[0] != '\0')
	{
		countusername -= 1;
	}
	printf ("客户端[%d]断开连接\n还有%d个客户端已经连接\n", pt->clientNo, countconnect);
	ZeroMemory (pt, sizeof (struct clients));
}

int initserver () 
{
	WSADATA wsaData;
	WORD    wVersionRequested = MAKEWORD (2, 2);
	
	struct   sockaddr_in seraddr;
	char     hostname[256];
	PHOSTENT hostinfo;
	char     *hostip;      //主机ip
	int      port = PORT;
	
	/*if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)	
	{
		printf ("Unable to install Consolehandler!\n\n");				
	}*/
	//初始化Winsock
	wVersionRequested = MAKEWORD (2, 2);
	err = WSAStartup (wVersionRequested, &wsaData);
	if (err != 0) 
	{
		printf ("WSAStartup 失败：%d\n", err);
		return 0;
	}
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		return 0; 
	}
	
	//获取本机IP地址
	if(gethostname (hostname, sizeof(hostname)) != 0)   //返回标准的本机主机名发生错误	
	{
		printf("gethostname() Error!"); 
		printf("The Error is:%d\n",WSAGetLastError()); //取得错误代码
		return 0;
	}
	else	
	{
		//printf("gethostname() sucessfully!\n");
		if((hostinfo = gethostbyname(hostname)) != NULL) //根据主机名返回主机信息	
		{	
			hostip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);	
			printf("本服务器的局域网IP是:%s\n广域网IP请查看路由器\n",hostip);  //输出本机的Ip地址	
		}
		else//gethostbyname失败	
		{	
			printf("gethostbyname() Error!"); 	
			printf("The Error is:%d\n",WSAGetLastError()); //取得错误代码	
			return 0;	
		}	
	} 
	
	printf ("请输入端口号后按回车：");
	scanf ("%d", &port);

	ZeroMemory (&seraddr, sizeof (seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port); //端口
	seraddr.sin_addr.S_un.S_addr = INADDR_ANY;  //ip

	//在服务器建立一个socket用来监听客户端连接
	ListenSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//检错确保此套接字是有效的
	if (ListenSocket == INVALID_SOCKET) 
	{
		printf("Error at socket (): %ld\n", WSAGetLastError ());
		WSACleanup ();
		return 0;
	}

	//绑定套接字
	err = bind (ListenSocket, (sockaddr *)&seraddr, sizeof (seraddr));
	if (err == SOCKET_ERROR) 
	{
		printf("绑定失败：%d\n", WSAGetLastError ());
		closesocket (ListenSocket);
		WSACleanup ();
		return 0;
	}	

	//监听一个Socket 最大：SOMAXCONN
	printf ("请输入要连接的客户端数量后按回车:");
	fflush (stdin);
	scanf ("%d", &num);
	if (listen (ListenSocket, num) == SOCKET_ERROR) 
	{
		printf ("监听错误：%d\n", WSAGetLastError());
		closesocket (ListenSocket);
		WSACleanup ();
		return 0;
	}
	printf ("正在监听客户端连接请求******\n");
	return 1;
}

//用select模型来处理连接和获取用户名的过程，客户端都输入用户名之前可以断开后反复重新连接，最后再分配玩家号。
//因为连接阶段会有各种不同的出错方式，为了清除得知出错方式，没有用封装的I/O函数，所以大部分代码都是查错。
//可能可以和之后的完成例程重叠I/O结合，不过不改了，就当是练习select的纪念。
int connect ()
{
	int	i;
	fd_set	fdread;//结构
	struct	timeval tv = {2, 0};//超时时间
	int	maxconnect = 0;
	int	next = 1;
	int	start = 0;
	while (1)
	{
		if (countusername == num && start == 0)
		{
			start = 1;
			for (i = 1; i <= num; i++)
			{ 
				printf ("客户端[%d]的用户名是：%s\n", i, client[i].username);		
			}
			printf ("开始游戏\n\n");
		}
		//准备select
		FD_ZERO (&fdread);//清空fd_set结构
		for (i = 1; i <= maxconnect; i++)
		{
			if (client[i].clientNo != '\0' && client[i].username[0] == '\0')
			{
				FD_SET (client[i].Socket, &fdread);//把客户套接字放到SELECT要求的数组中
			}
		}
		if (countconnect != num)
		{
			FD_SET (ListenSocket, &fdread);
		}
		err = select (0, &fdread, NULL, NULL, &tv);
		//select查错
		if (err == 0)
		{
			// Time expired 超时
			continue;
		}
		if (err < 0 && WSAGetLastError () != 10022)
		{
			printf ("select时有套接字出错：%ld\n", WSAGetLastError ());
			return 0;
		}
		//如果SELECT返回大于0
		for (i = 1; i <= maxconnect; i++)
		{
			if (FD_ISSET (client[i].Socket, &fdread))
			{
				//一个可读发生在这个套接字上
				err = recv (client[i].Socket, client[i].username, sizeof (client[i].username), 0);
				//把它读出到缓冲区
				if (err <= 0)
				{
					//客户端关闭
					printf ("接受客户端[%d]用户名出错：%ld\n", i, WSAGetLastError());
					error (&client[i]);
					printf ("还有%d个客户端已经准备游戏\n", countusername);
				}
				else
				{
					//接收到一个客户端的用户名
					countusername += 1;
					printf ("已经有%d个客户端准备游戏\n", countusername);
					newclient = i;
					
				}
			}
		}
		if (FD_ISSET (ListenSocket, &fdread))
		{
			//确定用哪个套接字连接
			for (i = 1; i <= maxconnect + 1; i++)
			{
				if (client[i].clientNo == '\0')
				{
					next = i;
					break;
				}
			}
			//接受
			client[next].Socket = accept (ListenSocket, (sockaddr *)&client[next].addr, &cliaddrlen);
			if (client[next].Socket == INVALID_SOCKET) 
			{
				printf ("重新接受客户端失败：%d\n", WSAGetLastError());
			}
			else
			{
				client[next].clientNo = next;
				if (next > maxconnect)
				{
					maxconnect = next;
				}
				countconnect += 1;
				printf ("已经有%d个客户端连接成功\n", countconnect);
				//发送游戏人数
				err = send (client[next].Socket, (char *)&num, sizeof (char), 0);
				//发送客户端号
				err = send (client[next].Socket, (char *)&client[next].clientNo, sizeof (char), 0);	
				if (err == SOCKET_ERROR ) 
				{
					error (&client[next]);
				}
			}
		}
		Sleep (2000);
	}
	return 1;	
}

/*unsigned __stdcall sendThread (void *pParam)
{
	int i;
	struct clients *pt = (struct clients *)pParam;
	for (i = 1; i <= num; i++)
	{
		if (i != pt->recvbuff.sender)
		{
			send (client[i].Socket, (char *)&pt->recvbuff, sizeof (pt->recvbuff), 0);
		}
	}
	return 0;
}*/

void CALLBACK CompletionROUTINE(DWORD dwError,
                                DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped,
                                DWORD dwFlags)
{
	//HANDLE sendThreadH;
	//unsigned sendThreadID;
 	struct clients *pt = (struct clients *)lpOverlapped;
	struct pack sendready;
	int i;
	if (dwError != 0 || cbTransferred == 0)
	{
		//客户端关闭套接字
		if (pt->clientNo != '\0')
		{
			printf ("客户端[%d]完成例程出错\n", pt->clientNo);
			error (pt);
		}
		return;
	}
	if (pt->recvbuff.flag == READY)
	{
		ready[pt->recvbuff.sender] = 1;
		for (i = 1; i <= num; i++)
		{
			
			if (ready[i] == 1)
			{
				ZeroMemory (&sendready, sizeof (struct pack));
				sendready.flag = READY;
				sendready.sender = i;
				strcpy (sendready.buff, client[i].username);
				send (client[pt->recvbuff.sender].Socket, (char *)&sendready, sizeof (struct pack), 0);
				send (client[i].Socket, (char *)&pt->recvbuff, sizeof (pt->recvbuff), 0);
			}
		}
	}
	else
	{
		for (i = 1; i <= num; i++)
		{
			if (i != pt->clientNo)
			{
				send (client[i].Socket, (char *)&pt->recvbuff, sizeof (pt->recvbuff), 0);
			}
		}
	}
	//sendThreadH = (HANDLE)_beginthreadex (NULL, 0, &sendThread, (LPVOID)client, 0, &sendThreadID);
	//CloseHandle (sendThreadH);
	// Launch another asynchronous operation
	//重新激活另一个WSARecv异步操作
	ZeroMemory (&pt->overlap, sizeof (WSAOVERLAPPED));
	pt->buffer.len = sizeof (pt->recvbuff);
	pt->buffer.buf = (char *)&pt->recvbuff;    
	err = WSARecv(pt->Socket,
		&pt->buffer,
		1,
		&pt->BytesRecvd,
		&pt->Flags,
		&pt->overlap,
		CompletionROUTINE);
	if (err == SOCKET_ERROR && WSAGetLastError () != WSA_IO_PENDING)
	{
		printf ("客户端[%d]重叠I/O接收操作出错\n", pt->clientNo);
		error (pt);
		return;
	}
}

//使用完成例程重叠I/O
unsigned __stdcall mainThread (void *pParam)
{
	int i;
	int err;
	int a = 0;
	DWORD index;
	BOOL OverlappedResult;
	unsigned long dwTrans;
	unsigned long dwFlags;
	while (1)
	{
		if (newclient != 0)
		{
			a = newclient;
			newclient = 0;
			client[a].buffer.len = sizeof (client[a].recvbuff);
			client[a].buffer.buf = (char *)&client[a].recvbuff;
			client[a].BytesRecvd = 0;
			client[a].Flags = 0;
			ZeroMemory (&client[a].recvbuff, sizeof (client[a].recvbuff));
			ZeroMemory (&client[a].overlap, sizeof (WSAOVERLAPPED));
			err = WSARecv(client[a].Socket,
				&client[a].buffer,
				1,
				&client[a].BytesRecvd,
				&client[a].Flags,
				&client[a].overlap,
				CompletionROUTINE);
			if (err == SOCKET_ERROR && WSAGetLastError () != WSA_IO_PENDING)
			{
				printf ("客户端[%d]投递重叠I/O接收请求时出错\n", a);
				error (&client[a]);
				return 0;
			}
			a = 0;
		}
		index = SleepEx (1000, TRUE);
		if (index != WAIT_IO_COMPLETION && index != WAIT_TIMEOUT && a != 0)
		{
			for (i = 1; i <= num; i++)
			{
				OverlappedResult = WSAGetOverlappedResult (client[i].Socket, &client[i].overlap, &dwTrans, FALSE, &dwFlags);
				if (OverlappedResult == FALSE && WSAGetLastError () != WSA_IO_INCOMPLETE)
				{
					printf ("客户端[%d]重叠I/O接收操作出错\n", i);
					error (&client[i]);
					return 0;
				}	
			}
			return 0;
		}
	}
	return 1;
}



int main (void)
{
	HANDLE mainThreadH;
	unsigned mainThreadID;
	if (initserver () == 0)
	{
		printf ("按任意键退出");
		getch ();
		return 0;
	}
	mainThreadH = (HANDLE)_beginthreadex (NULL, 0, &mainThread, NULL, 0, &mainThreadID);
	CloseHandle (mainThreadH);
	if (connect () == 0)
	{
		printf ("按任意键退出");
		getch ();
		return 0;
	}

	getch ();
	return 0;
}


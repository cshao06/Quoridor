#include "client.h"

char clientNo;

static int err;
static SOCKET ConnectSocket = INVALID_SOCKET;

//初始化客户端
int initclient() 
{	
	WSADATA wsaData;
	WORD    wVersionRequested;
	
	//初始化客户端并连接服务器
	wVersionRequested = MAKEWORD (2, 2);
	err = WSAStartup (wVersionRequested, &wsaData);
	if (err != 0) 
	{
		WSACleanup ();
		return 0;
	}
	if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion ) != 2) 
	{
		WSACleanup ();
		return 0;
	}
	printf("初始化客户端成功!\n");
	return 1;
}


int initconnect ()
{
	struct sockaddr_in seraddr;  
	char   serIp[25];
	int    port = PORT;
	//建立一个SOCKET来连接服务器
	ConnectSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_IP);

	//检查错误确保这个socket是有效的
	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf ("建立套接字失败：%ld\n", WSAGetLastError ());
		return 0;
	}
	
	//输入服务器IP
	printf("请输入服务器广域网IP后按回车:");    
	fflush (stdin);
	scanf("%s", &serIp);
	printf ("请输入端口号后按回车：");
	fflush (stdin);
	scanf ("%d", &port);

	ZeroMemory (&seraddr, sizeof (seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons (port);
	seraddr.sin_addr.S_un.S_addr = inet_addr(serIp);
	//连接到服务器
	err = connect (ConnectSocket, (struct sockaddr *)&seraddr, sizeof(seraddr));
	if (err == SOCKET_ERROR) 
	{
		closesocket (ConnectSocket);
		printf ("无法连接到服务器：%ld\n\n", WSAGetLastError ());
		return 0;
	}
	else
	{
		printf ("成功连接到服务器\n");
	}
	//接收服务器确定的玩家人数
	err = recv (ConnectSocket, (char *)&number, sizeof(char), 0);
	if (err <= 0)
	{
		printf ("接受玩家人数时出错: %d\n", WSAGetLastError ());
		closesocket (ConnectSocket);
		return 0;
	}
	//接收客户端号
	err = recv (ConnectSocket, (char *)&clientNo, sizeof(char), 0);
	if (err <= 0 )
	{
		closesocket (ConnectSocket);
		printf ("接受服务器分配客户端号时出错: %d\n", WSAGetLastError ());
		return 0;
	}
	else
	{
		printf ("你是客户端[%d]。\n\n", clientNo);	
	}
	
	//输入用户名
	printf ("请输入你的用户名：");
	fflush (stdin);
	fgets (p[clientNo]->username, 20, stdin);
	p[clientNo]->username[strlen (p[clientNo]->username) - 1] = '\0';
	//发送用户名
	err = send (ConnectSocket, p[clientNo]->username, (int)strlen (p[clientNo]->username) + 1, 0);
	if (err == SOCKET_ERROR ) 
	{
		printf("发送用户名失败: %d\n", WSAGetLastError());
		closesocket (ConnectSocket);
		return 0;
	}
	printf ("请等待其他玩家开始游戏******\n\n");
	printf ("***********此窗口用于聊天**********\n\n在游戏界面中按下 y 键即可输入聊天内容\n\n");
	printf ("按任意键准备，之后请等待其他玩家准备\n\n");
	getch ();
	return 1;
}

int waitforgame ()
{	
	int i;
	int readynum = 0;
	struct pack sendsignal;
	RECT r[5] = {{NULL}, {250, Y2 + 65, 450, Y2 + 85}, 
			{250, Y1 - 80, 450, Y1 - 60}, 
			{20, 425, X1 - 65, 500}, 
			{X2 + 70, 425, X0 - 20, 500}};
	sendsignal.sender = clientNo;
	strcpy (sendsignal.buff, p[clientNo]->username);
	sendsignal.flag = READY;
	mysend ((char *)&sendsignal, sizeof (struct pack));
	putpiece (clientNo);
	//接收各玩家用户名和准备消息
	while (readynum != number)
	{
		for (i = 1; i <= number; i++)
		{
			if (ready[i] == 1)
			{
				ready[i] = 0;
				readynum += 1;
				putpiece (i);
				setcolor (LIGHTRED);
				setfont (15  , 0, "楷体", 0, 0, 600, false, false, false);
				drawtext (p[recvready.sender]->username, &r[recvready.sender], DT_CENTER | DT_WORDBREAK);
			}
		}
		Sleep (100);
	}
	//SetWindowPos (console, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	//SetWindowPos (console, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	return 1;
}

//发送数据
void mysend (char *sendbuff, int len)
{
	int err;
	err = send (ConnectSocket, sendbuff, len, 0);
	if (err == SOCKET_ERROR) 
	{
		printf("与服务器断开连接: %d\n", WSAGetLastError());
		MessageBox(graph, TEXT ("与服务器断开连接，请关闭后重新开启客户端\n"), TEXT ("断开连接"), MB_OK); 
		closesocket (ConnectSocket);
		keykeep = 0;
		while (initconnect () == 0)
		{
			printf ("按任意键重新连接\n");
			getch ();
		}
		keykeep = 1;
	}
}

//接受数据
void myrecv (char *recvbuff, int len)
{
	err = recv(ConnectSocket, recvbuff, len, 0);
	if (err <= 0)
	{
		printf("与服务器断开连接: %d\n", WSAGetLastError());
		MessageBox(graph, TEXT ("与服务器断开连接，请关闭后重新开启客户端\n"), TEXT ("断开连接"), MB_OK); 
		closesocket (ConnectSocket);
		keykeep = 0;
		while (initconnect () == 0)
		{
			printf ("按任意键重新连接\n");
			getch ();
		}
		keykeep = 1;
	}
}

/*/一旦没有更多数据需要发送，关闭发送连接，客户端仍然可以用ClientSocket来接收数据
void shutsock ()
{	
	err = shutdown(ConnectSocket, SD_SEND);
	if (err == SOCKET_ERROR) 
	{
		printf ("关闭发送连接失败：%d\n", WSAGetLastError());
		closesocket (ConnectSocket);
		WSACleanup ();
		getch ();
		return;
	}
}*/
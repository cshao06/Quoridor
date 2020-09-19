#include "client.h"

char clientNo;

static int err;
static SOCKET ConnectSocket = INVALID_SOCKET;

//��ʼ���ͻ���
int initclient() 
{	
	WSADATA wsaData;
	WORD    wVersionRequested;
	
	//��ʼ���ͻ��˲����ӷ�����
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
	printf("��ʼ���ͻ��˳ɹ�!\n");
	return 1;
}


int initconnect ()
{
	struct sockaddr_in seraddr;  
	char   serIp[25];
	int    port = PORT;
	//����һ��SOCKET�����ӷ�����
	ConnectSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_IP);

	//������ȷ�����socket����Ч��
	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf ("�����׽���ʧ�ܣ�%ld\n", WSAGetLastError ());
		return 0;
	}
	
	//���������IP
	printf("�����������������IP�󰴻س�:");    
	fflush (stdin);
	scanf("%s", &serIp);
	printf ("������˿ںź󰴻س���");
	fflush (stdin);
	scanf ("%d", &port);

	ZeroMemory (&seraddr, sizeof (seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons (port);
	seraddr.sin_addr.S_un.S_addr = inet_addr(serIp);
	//���ӵ�������
	err = connect (ConnectSocket, (struct sockaddr *)&seraddr, sizeof(seraddr));
	if (err == SOCKET_ERROR) 
	{
		closesocket (ConnectSocket);
		printf ("�޷����ӵ���������%ld\n\n", WSAGetLastError ());
		return 0;
	}
	else
	{
		printf ("�ɹ����ӵ�������\n");
	}
	//���շ�����ȷ�����������
	err = recv (ConnectSocket, (char *)&number, sizeof(char), 0);
	if (err <= 0)
	{
		printf ("�����������ʱ����: %d\n", WSAGetLastError ());
		closesocket (ConnectSocket);
		return 0;
	}
	//���տͻ��˺�
	err = recv (ConnectSocket, (char *)&clientNo, sizeof(char), 0);
	if (err <= 0 )
	{
		closesocket (ConnectSocket);
		printf ("���ܷ���������ͻ��˺�ʱ����: %d\n", WSAGetLastError ());
		return 0;
	}
	else
	{
		printf ("���ǿͻ���[%d]��\n\n", clientNo);	
	}
	
	//�����û���
	printf ("����������û�����");
	fflush (stdin);
	fgets (p[clientNo]->username, 20, stdin);
	p[clientNo]->username[strlen (p[clientNo]->username) - 1] = '\0';
	//�����û���
	err = send (ConnectSocket, p[clientNo]->username, (int)strlen (p[clientNo]->username) + 1, 0);
	if (err == SOCKET_ERROR ) 
	{
		printf("�����û���ʧ��: %d\n", WSAGetLastError());
		closesocket (ConnectSocket);
		return 0;
	}
	printf ("��ȴ�������ҿ�ʼ��Ϸ******\n\n");
	printf ("***********�˴�����������**********\n\n����Ϸ�����а��� y ������������������\n\n");
	printf ("�������׼����֮����ȴ��������׼��\n\n");
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
	//���ո�����û�����׼����Ϣ
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
				setfont (15  , 0, "����", 0, 0, 600, false, false, false);
				drawtext (p[recvready.sender]->username, &r[recvready.sender], DT_CENTER | DT_WORDBREAK);
			}
		}
		Sleep (100);
	}
	//SetWindowPos (console, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	//SetWindowPos (console, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	return 1;
}

//��������
void mysend (char *sendbuff, int len)
{
	int err;
	err = send (ConnectSocket, sendbuff, len, 0);
	if (err == SOCKET_ERROR) 
	{
		printf("��������Ͽ�����: %d\n", WSAGetLastError());
		MessageBox(graph, TEXT ("��������Ͽ����ӣ���رպ����¿����ͻ���\n"), TEXT ("�Ͽ�����"), MB_OK); 
		closesocket (ConnectSocket);
		keykeep = 0;
		while (initconnect () == 0)
		{
			printf ("���������������\n");
			getch ();
		}
		keykeep = 1;
	}
}

//��������
void myrecv (char *recvbuff, int len)
{
	err = recv(ConnectSocket, recvbuff, len, 0);
	if (err <= 0)
	{
		printf("��������Ͽ�����: %d\n", WSAGetLastError());
		MessageBox(graph, TEXT ("��������Ͽ����ӣ���رպ����¿����ͻ���\n"), TEXT ("�Ͽ�����"), MB_OK); 
		closesocket (ConnectSocket);
		keykeep = 0;
		while (initconnect () == 0)
		{
			printf ("���������������\n");
			getch ();
		}
		keykeep = 1;
	}
}

/*/һ��û�и���������Ҫ���ͣ��رշ������ӣ��ͻ�����Ȼ������ClientSocket����������
void shutsock ()
{	
	err = shutdown(ConnectSocket, SD_SEND);
	if (err == SOCKET_ERROR) 
	{
		printf ("�رշ�������ʧ�ܣ�%d\n", WSAGetLastError());
		closesocket (ConnectSocket);
		WSACleanup ();
		getch ();
		return;
	}
}*/
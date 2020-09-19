#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT	3579 //��������Ҫ�Ϳͻ������ӵĶ˿�
#define BUFFLEN 256
#define READY	9

struct bans
{
	int x; //�������Ͻ�����
	int y; 
	int a1;//������ռ�����������һ������������
	int b1;
	int a2;//ͬ�ϣ��м�
	int b2;
	int a3;//ͬ�ϣ��һ���
	int b3;
	int d;  //-1��1��
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
//����ͻ�����
char num;
int	countusername = 0;
int	countconnect = 0;
int newclient = 0;

//Ϊ�������˽���һ����ΪListenSocket��SOCKET�������������ͻ��˵�����
SOCKET ListenSocket; 	

//�ͻ��˽ṹ��
struct clients
{
	WSAOVERLAPPED	overlap;//������ڵ�һ������Ϊ���������Ҫ�����ָ��ת���ɽṹ���ָ��
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
		err = MessageBox (NULL,"ȷ��Ҫ�˳���������","�˳�������",MB_YESNO);
		if (err == IDYES)
		{
			SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, FALSE);
		}
	}
	return TRUE;
}*/

void error (struct clients *pt)
{
	closesocket (pt->Socket);//�ر�����׽���
	if (pt->clientNo != '\0')
	{
		countconnect -= 1;
	}
	if (pt->username[0] != '\0')
	{
		countusername -= 1;
	}
	printf ("�ͻ���[%d]�Ͽ�����\n����%d���ͻ����Ѿ�����\n", pt->clientNo, countconnect);
	ZeroMemory (pt, sizeof (struct clients));
}

int initserver () 
{
	WSADATA wsaData;
	WORD    wVersionRequested = MAKEWORD (2, 2);
	
	struct   sockaddr_in seraddr;
	char     hostname[256];
	PHOSTENT hostinfo;
	char     *hostip;      //����ip
	int      port = PORT;
	
	/*if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)	
	{
		printf ("Unable to install Consolehandler!\n\n");				
	}*/
	//��ʼ��Winsock
	wVersionRequested = MAKEWORD (2, 2);
	err = WSAStartup (wVersionRequested, &wsaData);
	if (err != 0) 
	{
		printf ("WSAStartup ʧ�ܣ�%d\n", err);
		return 0;
	}
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		return 0; 
	}
	
	//��ȡ����IP��ַ
	if(gethostname (hostname, sizeof(hostname)) != 0)   //���ر�׼�ı�����������������	
	{
		printf("gethostname() Error!"); 
		printf("The Error is:%d\n",WSAGetLastError()); //ȡ�ô������
		return 0;
	}
	else	
	{
		//printf("gethostname() sucessfully!\n");
		if((hostinfo = gethostbyname(hostname)) != NULL) //��������������������Ϣ	
		{	
			hostip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);	
			printf("���������ľ�����IP��:%s\n������IP��鿴·����\n",hostip);  //���������Ip��ַ	
		}
		else//gethostbynameʧ��	
		{	
			printf("gethostbyname() Error!"); 	
			printf("The Error is:%d\n",WSAGetLastError()); //ȡ�ô������	
			return 0;	
		}	
	} 
	
	printf ("������˿ںź󰴻س���");
	scanf ("%d", &port);

	ZeroMemory (&seraddr, sizeof (seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port); //�˿�
	seraddr.sin_addr.S_un.S_addr = INADDR_ANY;  //ip

	//�ڷ���������һ��socket���������ͻ�������
	ListenSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���ȷ�����׽�������Ч��
	if (ListenSocket == INVALID_SOCKET) 
	{
		printf("Error at socket (): %ld\n", WSAGetLastError ());
		WSACleanup ();
		return 0;
	}

	//���׽���
	err = bind (ListenSocket, (sockaddr *)&seraddr, sizeof (seraddr));
	if (err == SOCKET_ERROR) 
	{
		printf("��ʧ�ܣ�%d\n", WSAGetLastError ());
		closesocket (ListenSocket);
		WSACleanup ();
		return 0;
	}	

	//����һ��Socket ���SOMAXCONN
	printf ("������Ҫ���ӵĿͻ��������󰴻س�:");
	fflush (stdin);
	scanf ("%d", &num);
	if (listen (ListenSocket, num) == SOCKET_ERROR) 
	{
		printf ("��������%d\n", WSAGetLastError());
		closesocket (ListenSocket);
		WSACleanup ();
		return 0;
	}
	printf ("���ڼ����ͻ�����������******\n");
	return 1;
}

//��selectģ�����������Ӻͻ�ȡ�û����Ĺ��̣��ͻ��˶������û���֮ǰ���ԶϿ��󷴸��������ӣ�����ٷ�����Һš�
//��Ϊ���ӽ׶λ��и��ֲ�ͬ�ĳ���ʽ��Ϊ�������֪����ʽ��û���÷�װ��I/O���������Դ󲿷ִ��붼�ǲ��
//���ܿ��Ժ�֮�����������ص�I/O��ϣ����������ˣ��͵�����ϰselect�ļ��
int connect ()
{
	int	i;
	fd_set	fdread;//�ṹ
	struct	timeval tv = {2, 0};//��ʱʱ��
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
				printf ("�ͻ���[%d]���û����ǣ�%s\n", i, client[i].username);		
			}
			printf ("��ʼ��Ϸ\n\n");
		}
		//׼��select
		FD_ZERO (&fdread);//���fd_set�ṹ
		for (i = 1; i <= maxconnect; i++)
		{
			if (client[i].clientNo != '\0' && client[i].username[0] == '\0')
			{
				FD_SET (client[i].Socket, &fdread);//�ѿͻ��׽��ַŵ�SELECTҪ���������
			}
		}
		if (countconnect != num)
		{
			FD_SET (ListenSocket, &fdread);
		}
		err = select (0, &fdread, NULL, NULL, &tv);
		//select���
		if (err == 0)
		{
			// Time expired ��ʱ
			continue;
		}
		if (err < 0 && WSAGetLastError () != 10022)
		{
			printf ("selectʱ���׽��ֳ���%ld\n", WSAGetLastError ());
			return 0;
		}
		//���SELECT���ش���0
		for (i = 1; i <= maxconnect; i++)
		{
			if (FD_ISSET (client[i].Socket, &fdread))
			{
				//һ���ɶ�����������׽�����
				err = recv (client[i].Socket, client[i].username, sizeof (client[i].username), 0);
				//����������������
				if (err <= 0)
				{
					//�ͻ��˹ر�
					printf ("���ܿͻ���[%d]�û�������%ld\n", i, WSAGetLastError());
					error (&client[i]);
					printf ("����%d���ͻ����Ѿ�׼����Ϸ\n", countusername);
				}
				else
				{
					//���յ�һ���ͻ��˵��û���
					countusername += 1;
					printf ("�Ѿ���%d���ͻ���׼����Ϸ\n", countusername);
					newclient = i;
					
				}
			}
		}
		if (FD_ISSET (ListenSocket, &fdread))
		{
			//ȷ�����ĸ��׽�������
			for (i = 1; i <= maxconnect + 1; i++)
			{
				if (client[i].clientNo == '\0')
				{
					next = i;
					break;
				}
			}
			//����
			client[next].Socket = accept (ListenSocket, (sockaddr *)&client[next].addr, &cliaddrlen);
			if (client[next].Socket == INVALID_SOCKET) 
			{
				printf ("���½��ܿͻ���ʧ�ܣ�%d\n", WSAGetLastError());
			}
			else
			{
				client[next].clientNo = next;
				if (next > maxconnect)
				{
					maxconnect = next;
				}
				countconnect += 1;
				printf ("�Ѿ���%d���ͻ������ӳɹ�\n", countconnect);
				//������Ϸ����
				err = send (client[next].Socket, (char *)&num, sizeof (char), 0);
				//���Ϳͻ��˺�
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
		//�ͻ��˹ر��׽���
		if (pt->clientNo != '\0')
		{
			printf ("�ͻ���[%d]������̳���\n", pt->clientNo);
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
	//���¼�����һ��WSARecv�첽����
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
		printf ("�ͻ���[%d]�ص�I/O���ղ�������\n", pt->clientNo);
		error (pt);
		return;
	}
}

//ʹ����������ص�I/O
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
				printf ("�ͻ���[%d]Ͷ���ص�I/O��������ʱ����\n", a);
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
					printf ("�ͻ���[%d]�ص�I/O���ղ�������\n", i);
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
		printf ("��������˳�");
		getch ();
		return 0;
	}
	mainThreadH = (HANDLE)_beginthreadex (NULL, 0, &mainThread, NULL, 0, &mainThreadID);
	CloseHandle (mainThreadH);
	if (connect () == 0)
	{
		printf ("��������˳�");
		getch ();
		return 0;
	}

	getch ();
	return 0;
}


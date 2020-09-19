#include "clientplay.h"

int remoteend = 0;
struct pack recvpiece;
struct pack recvbar;
struct pack recvready;
int reply;
int ready[5];

//线程
unsigned __stdcall recvThread (void *pParam)
{
	struct pack recvbuff;
	char str[20];
	struct pack sendsignal;
	while (1)
	{
		ZeroMemory (&recvbuff, sizeof (struct pack));
		myrecv ((char *)&recvbuff, sizeof (struct pack));
		switch (recvbuff.flag)
		{
		case MSG:
			printf (" %s 说：\n%s\n\n", p[recvbuff.sender]->username, recvbuff.buff);
			break;
		case PIECE:
			recvpiece = recvbuff;
			remoteend = 1;
			break;
		case BAR:
			recvbar = recvbuff;
			remoteend = 1;
			break;
		case RESTART:
			sprintf (str, "%s请求重新开始，是否同意？", p[recvbuff.sender]->username);
			if (MessageBox(graph, TEXT (str), TEXT ("重新开始"),MB_YESNO) == IDYES)
			{
				sendsignal.flag = AGREE;
			}
			else
			{
				sendsignal.flag = DISAGREE;
			}
			mysend ((char *)&sendsignal, sizeof (struct pack));
			break;
		case AGREE:
			reply += 1;
			break;
		case DISAGREE:
			reply += 1;
			restart = IDNO;
			break;
		case YESRESTART:
			restart = IDYES;
			MessageBox(graph, TEXT ("所有玩家都同意重新开始"), TEXT ("重新开始"),MB_OK);
			remoteend = 1;
			break;
		case NORESTART:
			MessageBox(graph, TEXT ("有人不同意重新开始"), TEXT ("重新开始"),MB_OK);
			break;
		case READY:
			recvready = recvbuff;
			ready[recvready.sender] = 1;
			strcpy (p[recvready.sender]->username, recvready.buff);
			break;
		}
	}
	return 0;
}

unsigned __stdcall talkThread (void *pParam)
{
	struct pack sendmsg;
	//HANDLE sendThreadH;
	//unsigned sendThreadID;
	ZeroMemory (&sendmsg, sizeof (pack));
	if (InputBox (sendmsg.buff, BUFFLEN, "你输入的内容将发送给其他所以用户", "输入聊天内容", NULL, 0, 0, FALSE) == TRUE)
	{
		//sendmsg.buff[BUFFLEN - 2] = '\n';
		sendmsg.flag = MSG;
		sendmsg.sender = clientNo;
		//sendThreadH = (HANDLE)_beginthreadex (NULL, 0, sendThread, (LPVOID)&sendmsg, 0, &sendThreadID);
		//CloseHandle (sendThreadH);
		mysend ((char *)&sendmsg, sizeof (struct pack));
		printf (" 你 说:\n%s\n\n", sendmsg.buff);
	}
	return 0;
}

/*unsigned __stdcall sendThread (void *pParam)
{
	struct pack *pt = (struct pack *)pParam;
	mysend ((char *)pt, sizeof (struct pack));
	return 0;
}*/

void remoteturn (struct player *pn)
{
	while (remoteend == 0)
	{
		Sleep (50);
	}
	remoteend = 0;
	if (restart == IDYES)
	{
		return;
	}
	if (recvpiece.flag == PIECE)
	{
		setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);  //利用移动前存储的信息恢复各项坐标和图片
		bar (pn->px, pn->py, pn->px + 39, pn->py + 39);
		b[pn->a][pn->b] = 0;
		pn->px = recvpiece.px;
		pn->py = recvpiece.py;
		pn->a = recvpiece.a;
		pn->b = recvpiece.b;
		putimage (pn->px, pn->py, &piece[pn->name]);
		b[pn->a][pn->b] = 1;
		ZeroMemory (&recvpiece, sizeof (recvpiece));
	}
	else if (recvbar.flag == BAR)
	{
		ban = recvbar.buffban;
		b[ban.a1][ban.b1] = 1;
		b[ban.a2][ban.b2] = 1;
		b[ban.a3][ban.b3] = 1;
		if (ban.d == -1)
			putimage (ban.x, ban.y, &piece[5]);
		else
			putimage (ban.x, ban.y, &piece[7]);
		pn->bars -= 1;
		remainbar (pn);
		ZeroMemory (&recvbar, sizeof (recvbar));
	}
}

/*void replacewords()
{
	COORD crDest;
	SMALL_RECT rc;
　	CHAR_INFO chFill;
	HANDLE hOut;
	CONSOLE_CURSOR_INFO cursor;
	//CONSOLE_SCREEN_BUFFER_INFO bInfo;
	char str[BUFFLEN + 40];
	hOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	//GetConsoleScreenBufferInfo (hOut, &bInfo); 
	GetConsoleCursorInfo (hOut, &cursor);
	crDest = {x, y};
	rc = {0, };
	sprintf (str, " %s 说：\n%s\n", p[recvbuff.sender]->username, recvbuff.buff);
	ScrollConsoleScreenBuffer(hOut, &rc, NULL, crDest, str);
}*/
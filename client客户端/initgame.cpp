#include "initgame.h"

//初始化图形界面
void init ()
{
	HANDLE recvThreadH;
	unsigned recvThreadID;
	HANDLE keyThreadH;
	unsigned keyThreadID;
	char strTitle[255];
	int x, y;
	int i;
	FILE *fp;
	if ((fp = fopen ("config.txt", "r")) != NULL)
	{
		fscanf (fp, "%d*%d", &x, &y); //从config.txt中读取设定的界面大小
		fclose (fp);
		xasp = (float)x / X0; 
		yasp = (float)y / Y0;
	}
	else
	{
		x = X0;
		y = Y0;
		xasp = 1;
		yasp = 1;
	}
	//初始化客户端
	GetConsoleTitle(strTitle, 255);
	console = FindWindow("ConsoleWindowClass", strTitle);
	online = MessageBox (console, TEXT ("是否联网对战？"), TEXT ("联网对战"),MB_YESNO);
	if (online == IDYES)
	{
		if (initclient () == 0)
		{
			printf ("初始化客户端失败，按任意键开始单机游戏\n");
			getch ();
			online = IDNO;
			initgraph (x, y);
		}
		else
		{
			while (initconnect () == 0)
			{
				printf ("按任意键重新连接\n");
				getch ();
			}
			initgraph (x, y, SHOWCONSOLE);
			recvThreadH = (HANDLE)_beginthreadex (NULL, 0, recvThread, NULL, 0, &recvThreadID);
			CloseHandle (recvThreadH);
		}
	}
	else
	{
		initgraph (x, y);	
	}
	
	graph = GetHWnd ();
	setaspectratio (xasp, yasp); //改变逻辑坐标
	loadimage (&piece[1], "IMAGE", "red", 40, 40);  //导入4个棋子图片
	loadimage (&piece[2], "IMAGE", "blue", 40, 40);
	loadimage (&piece[3], "IMAGE", "yellow", 40, 40);
	loadimage (&piece[4], "IMAGE", "green", 40, 40);
	//隔板长宽85和5是有棋盘决定的，以下的bar函数里的84和4即85-1和5-1
	loadimage (&piece[5], "IMAGE", "bar", 85, 5);  //横的隔板
	loadimage (&piece[6], "IMAGE", "bar", 80, 10); //横的隔板标示 
	loadimage (&piece[7], "IMAGE", "bar2", 5, 85); //竖的隔板
	for (i = 1; i <= 4; i++)  //复制原始玩家信息，用于重玩时恢复
	{
		pbuff[i] = *p[i];
	}
	keyThreadH = (HANDLE)_beginthreadex (NULL, 0, keyThread, NULL, 0, &keyThreadID);
	CloseHandle (keyThreadH);
}

//初始化游戏数据
void initdata ()
{
	int i, j;
	setbkcolor (LIGHTBLUE);  //背景色
	cleardevice ();
	draw ();
	rule ();
	
	for (i = 1; i <= 17; i++)  //把棋盘边缘全记1，内部全记0
	{
		for (j = 1; j <= 17; j++)
		{
			b[i][j] = 0;
		}
	}
	for (i = 0; i <= 18; i++)
	{
		b[0][i] = 1;
		b[18][i] = 1;
		b[i][0] = 1;
		b[i][18] = 1;
	}
	for (i = 1; i <= 4; i++)  //初始化玩家信息
	{
		*p[i] = pbuff[i];
	}	
	pieceorbar = 0;
	restart = IDNO;//默认不重新开始
	undo = 0;
	//准备开始游戏
	if (online == IDYES)
	{
		waitforgame ();
	}
	else
	{
		number = set ();//获取玩家人数
		for (i = 1; i <= number; i++)
		{
			putpiece (i);
		}
	}
}

//画棋盘和2个按钮
//放棋子的格子边长40，放隔板的槽宽5，这两个数据贯穿整个程序：45=40+5, 85=40+5+40 
void draw ()
{
	int i;
	setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
	bar (X1, Y1, X2 - 1, Y2 - 1);
	setfillstyle (WHITE, SOLID_FILL, NULL);
	for (i = 0; i < 10; i++)
	{
		bar (X1, Y1 + i * 45, X2 - 1, Y1 + i * 45 + 4);	
	}
	for (i = 0; i < 10; i++)
	{
		bar (X1 + i * 45, Y1, X1 + i * 45 + 4, Y2 - 1);
	}
	setfillstyle (DARKGRAY, SOLID_FILL, NULL);
	setcolor (YELLOW);
	setbkcolor (DARKGRAY);
	setfont (20, 0 ,"楷体", 0, 0, 600, false, false, false);
	bar (X2 - 140, Y1 - 130, X2 - 40, Y1 - 100);
	outtextxy (X2 - 130, Y1 - 125, "重新开始");
	if (online == IDNO)
	{
		bar (X2 - 20, Y1 - 130, X2 + 80, Y1 - 100);
		outtextxy (X2 + 10, Y1 - 125, "悔棋");
	}
	setbkcolor (LIGHTBLUE);
}

//输出游戏指南和作者信息
void rule ()
{
	RECT r = {10, Y0 - 30, X0 - 10, Y0};
	setcolor (LIGHTRED);
	setfont (60, 0, "楷体", 0, 0, 800, false, false, false);
	outtextxy (90, 6, "Quoridor 步步为营");
	setcolor (WHITE);
	setfont (15  , 0, "楷体", 0, 0, 600, false, false, false);
	drawtext ("by sck，QQ:369661500，2012年8月，版本v1.5", &r, DT_CENTER);
}

//设置玩家人数：i=2, 2人; i=4, 4人
int set ()
{
	int i;  
	i = MessageBox(graph, TEXT ("按yes则为2人游戏，否则为4人游戏"), TEXT ("开始游戏"),MB_YESNO | MB_ICONQUESTION);
	if (i == IDYES)
		i = 2;
	else
		i = 4;
	return i;
}

//根据玩家数摆棋子和隔板标示
void putpiece (int i)
{
	if (number == 4) //若四人玩则每人5块隔板
	{
		p[i]->bars = 5;
	}
	putimage (p[i]->px, p[i]->py, &piece[i]);//摆上棋子并且使对应格子标志为1，即有棋状态
	b[p[i]->a][p[i]->b] =1;
	putimage (p[i]->bx, p[i]->by, &piece[6]);//不同位置摆上隔板标示并输出总隔板数
	remainbar (p[i]);
	switch (i)
	{
	case 1: putimage (330, Y2 + 20, &piece[1]); break; //摆上棋子标示
	case 2: putimage (330, Y1 - 60, &piece[2]); break;
	case 3: putimage (X1 - 60, 410, &piece[3]); break;
	case 4: putimage (X2 + 20, 410, &piece[4]); break;
	}		
}


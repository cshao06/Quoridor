#include "initgame.h"

//��ʼ��ͼ�ν���
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
		fscanf (fp, "%d*%d", &x, &y); //��config.txt�ж�ȡ�趨�Ľ����С
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
	//��ʼ���ͻ���
	GetConsoleTitle(strTitle, 255);
	console = FindWindow("ConsoleWindowClass", strTitle);
	online = MessageBox (console, TEXT ("�Ƿ�������ս��"), TEXT ("������ս"),MB_YESNO);
	if (online == IDYES)
	{
		if (initclient () == 0)
		{
			printf ("��ʼ���ͻ���ʧ�ܣ����������ʼ������Ϸ\n");
			getch ();
			online = IDNO;
			initgraph (x, y);
		}
		else
		{
			while (initconnect () == 0)
			{
				printf ("���������������\n");
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
	setaspectratio (xasp, yasp); //�ı��߼�����
	loadimage (&piece[1], "IMAGE", "red", 40, 40);  //����4������ͼƬ
	loadimage (&piece[2], "IMAGE", "blue", 40, 40);
	loadimage (&piece[3], "IMAGE", "yellow", 40, 40);
	loadimage (&piece[4], "IMAGE", "green", 40, 40);
	//���峤��85��5�������̾����ģ����µ�bar�������84��4��85-1��5-1
	loadimage (&piece[5], "IMAGE", "bar", 85, 5);  //��ĸ���
	loadimage (&piece[6], "IMAGE", "bar", 80, 10); //��ĸ����ʾ 
	loadimage (&piece[7], "IMAGE", "bar2", 5, 85); //���ĸ���
	for (i = 1; i <= 4; i++)  //����ԭʼ�����Ϣ����������ʱ�ָ�
	{
		pbuff[i] = *p[i];
	}
	keyThreadH = (HANDLE)_beginthreadex (NULL, 0, keyThread, NULL, 0, &keyThreadID);
	CloseHandle (keyThreadH);
}

//��ʼ����Ϸ����
void initdata ()
{
	int i, j;
	setbkcolor (LIGHTBLUE);  //����ɫ
	cleardevice ();
	draw ();
	rule ();
	
	for (i = 1; i <= 17; i++)  //�����̱�Եȫ��1���ڲ�ȫ��0
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
	for (i = 1; i <= 4; i++)  //��ʼ�������Ϣ
	{
		*p[i] = pbuff[i];
	}	
	pieceorbar = 0;
	restart = IDNO;//Ĭ�ϲ����¿�ʼ
	undo = 0;
	//׼����ʼ��Ϸ
	if (online == IDYES)
	{
		waitforgame ();
	}
	else
	{
		number = set ();//��ȡ�������
		for (i = 1; i <= number; i++)
		{
			putpiece (i);
		}
	}
}

//�����̺�2����ť
//�����ӵĸ��ӱ߳�40���Ÿ���Ĳۿ�5�����������ݹᴩ��������45=40+5, 85=40+5+40 
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
	setfont (20, 0 ,"����", 0, 0, 600, false, false, false);
	bar (X2 - 140, Y1 - 130, X2 - 40, Y1 - 100);
	outtextxy (X2 - 130, Y1 - 125, "���¿�ʼ");
	if (online == IDNO)
	{
		bar (X2 - 20, Y1 - 130, X2 + 80, Y1 - 100);
		outtextxy (X2 + 10, Y1 - 125, "����");
	}
	setbkcolor (LIGHTBLUE);
}

//�����Ϸָ�Ϻ�������Ϣ
void rule ()
{
	RECT r = {10, Y0 - 30, X0 - 10, Y0};
	setcolor (LIGHTRED);
	setfont (60, 0, "����", 0, 0, 800, false, false, false);
	outtextxy (90, 6, "Quoridor ����ΪӪ");
	setcolor (WHITE);
	setfont (15  , 0, "����", 0, 0, 600, false, false, false);
	drawtext ("by sck��QQ:369661500��2012��8�£��汾v1.5", &r, DT_CENTER);
}

//�������������i=2, 2��; i=4, 4��
int set ()
{
	int i;  
	i = MessageBox(graph, TEXT ("��yes��Ϊ2����Ϸ������Ϊ4����Ϸ"), TEXT ("��ʼ��Ϸ"),MB_YESNO | MB_ICONQUESTION);
	if (i == IDYES)
		i = 2;
	else
		i = 4;
	return i;
}

//��������������Ӻ͸����ʾ
void putpiece (int i)
{
	if (number == 4) //����������ÿ��5�����
	{
		p[i]->bars = 5;
	}
	putimage (p[i]->px, p[i]->py, &piece[i]);//�������Ӳ���ʹ��Ӧ���ӱ�־Ϊ1��������״̬
	b[p[i]->a][p[i]->b] =1;
	putimage (p[i]->bx, p[i]->by, &piece[6]);//��ͬλ�ð��ϸ����ʾ������ܸ�����
	remainbar (p[i]);
	switch (i)
	{
	case 1: putimage (330, Y2 + 20, &piece[1]); break; //�������ӱ�ʾ
	case 2: putimage (330, Y1 - 60, &piece[2]); break;
	case 3: putimage (X1 - 60, 410, &piece[3]); break;
	case 4: putimage (X2 + 20, 410, &piece[4]); break;
	}		
}


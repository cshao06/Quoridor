#include "quoridor.h"

/**********************************全局变量****************************/
HWND graph;
HWND console;

int online = IDNO;
int restart = IDYES;    //用于重新开始功能
int pieceorbar; //用于判断返回棋子1还是隔板2,用于悔棋功能
int undo; //0为不可悔棋，1为可以悔棋，2为要悔棋

IMAGE piece[8];
char number; //游戏人数
float xasp, yasp;
int b[19][19];  //整个程序判断机制的核心，数组下标为格子的坐标，内容为占用状态，0未占用，1占用。
		//一个棋子占一格；一个隔板占三格，中间一格的值为1。
		//共9格加8槽，0和18为最边缘，不放子，但用于判断是否出界

struct player p1 = {"", 1, X1 + 4 * 45 + 5, Y2 - 45, 10, X2 - 80, Y2 + 10, 9, 17, 1, &p1.b}; //红色 下方
struct player p2 = {"", 2, X1 + 4 * 45 + 5, Y1 + 5, 10, X1, Y1 - 40, 9, 1, 17, &p2.b};       //蓝色 上方
struct player p3 = {"", 3, X1 + 5, Y1 + 4 * 45 + 5, 10, X1 - 100, Y2 - 50, 1, 9, 17, &p3.a}; //黄色 左方
struct player p4 = {"", 4, X2 - 45, Y1 + 4 * 45 + 5, 10, X2 + 10, Y1 + 20, 17, 9, 1, &p4.a}; //绿色 右方
struct player *p[5] = {NULL, &p1, &p2, &p3, &p4};  //各个函数传递玩家信息都用pn指针
struct player pbuff[5]; //pbuff用于在主函数中缓存后给重新开始游戏赋值

struct bans ban; //隔板的坐标和有关数组的行列数

int mkeep = 1;//控制鼠标循环
int keykeep = 1;
char move;

/***********************************************************************/


//判断是否有人胜利，若无则返回0，若有则返回下一玩家编号
int winornext (struct player *pn)
{
	int next;
	if (*pn->nowline != pn->winline) //判断胜利时的位置==标准，则胜利
	{		
		if (number == 2)  //根据玩家人数判断下一玩家
		{
			if (pn->name == 1)
				next = 2;
			else
				next = 1;
		}
		else
		{
			switch (pn->name)
			{
			case 1: next = 3; break;
			case 3: next = 2; break;
			case 2: next = 4; break;
			case 4: next = 1; break;
			}
		}		
	}
	else
	{
		next = 0;
	}
	return next;
}



int play ()
{
	int n = 1; //本回合玩家名字
	int previous = 1; //上一回合玩家名字，用于悔棋
	struct pack sendpiece;
	struct pack sendbar;
	while (1)
	{
		//输出回合信息
		setcolor (YELLOW);
		setfont (20, 0, "楷体", 0, 0, 400, false, false, false);
		outtextxy (X1 - 50, Y1 - 130, "本回合是：");
		putimage (X1 + 50, Y1 - 140, &piece[p[n]->name]);
		if (online == IDYES)
		{
			if (n == clientNo)
			{
				myturn (p[n]);
				if (restart == IDYES) 
				{
					restart = IDNO;
					undo = 0;
					pkeep = 0;
					bkeep = 0;
					return 1;     //若play函数中按了重新开始，则直接重新开始游戏
				}
				if (pieceorbar == 1)
				{
					sendpiece.sender = clientNo;
					sendpiece.flag = PIECE;
					sendpiece.px = p[n]->px;
					sendpiece.py = p[n]->py;
					sendpiece.a = p[n]->a;
					sendpiece.b = p[n]->b;
					mysend ((char *)&sendpiece, sizeof (struct pack));
				}
				else if (pieceorbar == 2)
				{
					sendbar.sender = clientNo;
					sendbar.flag = BAR;
					sendbar.buffban = ban;
					mysend ((char *)&sendbar, sizeof (struct pack));
				}
				if ((n = winornext (p[n])) == 0) //判断胜利和改变玩家
				{
					//是否重新开始
					restart = MessageBox(graph, TEXT ("玩家到边获胜！ 是否重新开始？"), TEXT ("游戏结束"),MB_YESNO); 
					if (restart == IDYES)
					{
						
						return 1;
					}
					else
					{
						return 0;
					}
				}
			}
			else
			{
				remoteturn (p[n]);
				if (restart == IDYES)
				{
					restart = IDNO;
					return 1;
				}
				if ((n = winornext (p[n])) == 0) //判断胜利和改变玩家
				{
					//是否重新开始
					restart = MessageBox(graph, TEXT ("玩家到边获胜！ 是否重新开始？"), TEXT ("游戏结束"),MB_YESNO); 
					if (restart == IDYES)
					{
						return 1;
					}
					else
					{
						return 0;
					}
				}
			}
		}
		else
		{
			myturn (p[n]);
			if (restart == IDYES) 
			{
				restart = IDNO;
				undo = 0;
				pkeep = 0;
				bkeep = 0;
				return 1;     //若play函数中按了重新开始，则直接重新开始游戏
			}
			if (undo == 2)
			{
				undo = 0;
				undofunc (p[previous]);
				n = previous; //使该回合玩家名恢复为上一回合
				continue;
			}
			previous = n; //因为接下来判断胜利后n会成为下一回合玩家，所以previous先继承n
			if ((n = winornext (p[n])) == 0) //判断胜利和改变玩家
			{
				//是否重新开始
				restart = MessageBox(graph, TEXT ("玩家到边获胜！ 是否重新开始？"), TEXT ("游戏结束"),MB_YESNO); 
				if (restart == IDYES)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

//一个回合的控制函数，根据参数判断是哪一个玩家，函数内的各项操作以及调用各种函数都根据不同玩家自动变化
//定义的全局结构体变量和指针的作用就在于此
void myturn (struct player *pn)
{
	int sign = 0;
	MOUSEMSG m;
	HANDLE pThreadH, bThreadH;
	unsigned pThreadID, bThreadID;
	struct pack sendsignal;
	mkeep = 1;
	while (1)  //鼠标循环
	{	
		FlushMouseMsgBuffer();
		while (!MouseHit ())
		{
			if (mkeep == 0)
			{
				sign = 1;
				break;
			}
			Sleep (50);
		}
		if (sign == 1) break;
		m = GetMouseMsg ();
		if (m.uMsg == WM_LBUTTONDOWN) //若鼠标消息不是按下左键，则反复获取m
		{
			//若鼠标按在棋子范围内
			if (m.x >= pn->px * xasp && m.x <= (pn->px + 39) * xasp && m.y >= pn->py * yasp && m.y <= (pn->py + 39) * yasp && pkeep == 0)  
			{	
				undo = 0;
				bkeep = 0;
				pkeep = 1;
				pThreadH = (HANDLE)_beginthreadex (NULL, 0, pThread, (LPVOID)pn, 0, &pThreadID);
				CloseHandle (pThreadH);
			}
			//若鼠标按在隔板标示范围内
			else if (m.x >= pn->bx * xasp && m.x <= (pn->bx + 79) * xasp && m.y >= pn->by * yasp && m.y <= (pn->by + 9) *yasp && pn->bars != 0 && bkeep == 0) 
			{
				undo = 0;
				pkeep = 0;
				bkeep = 1;
				bThreadH = (HANDLE)_beginthreadex (NULL, 0, bThread, (LPVOID)pn, 0, &bThreadID);
				CloseHandle (bThreadH);
			}
			//若鼠标在“重新开始”范围内
			else if (m.x >= (X2 - 140) * xasp && m.x <= (X2 - 40) * xasp && m.y >= (Y1 - 130) * yasp && m.y <= (Y1 - 100) * yasp) 
			{
				if (online == IDYES)
				{	
					restart = MessageBox(graph, TEXT ("是否重新开始游戏？若是，请等待其他玩家同意"), TEXT ("重新开始"),MB_YESNO);
					if (restart == IDYES)
					{
						sendsignal.flag = RESTART;
						sendsignal.sender = clientNo;
						mysend ((char *)&sendsignal, sizeof (struct pack));
						while (reply != number - 1)
						{
							Sleep (100);
						}
						reply = 0;
						if (restart == IDYES)
						{
							sendsignal.flag = YESRESTART;
							mysend ((char *)&sendsignal, sizeof (struct pack));
							MessageBox(graph, TEXT ("所有人都同意重新开始"), TEXT ("重新开始"),MB_OK);
							break;
						}
						else
						{
							sendsignal.flag = NORESTART;
							mysend ((char *)&sendsignal, sizeof (struct pack));
							MessageBox(graph, TEXT ("有人不同意重新开始"), TEXT ("重新开始"),MB_OK);
						}
					}
				}
				else
				{
					restart = MessageBox(graph, TEXT ("是否重新开始游戏？"), TEXT ("重新开始"),MB_YESNO);
					if (restart == IDYES) 
					{
						break;
					}
				}
			}
			//若鼠标在“悔棋”范围内且有棋可悔
			else if (m.x >= (X2 - 20) * xasp && m.x <= (X2 + 80) * xasp && m.y >= (Y1 - 130) * yasp && m.y <= (Y1 - 100) * yasp && undo == 1 && online == IDNO) 
			{
				undo = 2;
				break;
			}	
		}
		Sleep (10);
	}
}

void undofunc (struct player *pre)
{
	if (pieceorbar == 1)
	{ 
		setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);  //利用移动前存储的信息恢复各项坐标和图片
		bar (pre->px, pre->py, pre->px + 39, pre->py + 39);
		b[pre->a][pre->b] = 0;
		pre->px = buffpx; pre->py = buffpy;
		pre->a = buffa;   pre->b = buffb;
		putimage (buffpx, buffpy, &piece[pre->name]);
		b[buffa][buffb] = 1;
	}
	else
	{
		b[ban.a1][ban.b1] = 0; //直接删去占用和图片
		b[ban.a2][ban.b2] = 0;
		b[ban.a3][ban.b3] = 0;
		setfillstyle (WHITE, SOLID_FILL, NULL);
		if (ban.d == -1)
			bar (ban.x, ban.y, ban.x + 84, ban.y + 4);
		else
			bar (ban.x, ban.y, ban.x + 4, ban.y + 84);
		pre->bars += 1; //前一玩家的隔板数恢复
		remainbar (pre);
	}
}

unsigned __stdcall keyThread (void *pParam)
{
	char key;
	HANDLE talkThreadH;
	unsigned talkThreadID;
	
	while (1)
	{
		if (keykeep == 1)
		{
			key = getch ();
			if (key == 'y')
			{
				talkThreadH = (HANDLE)_beginthreadex (NULL, 0, talkThread, NULL, 0, &talkThreadID);
				CloseHandle (talkThreadH);
			}
			else
			{
				move = key;
			}
		}
		Sleep (100);
	}
	return 0;
}

//主函数，循环控制重新开始游戏
int main (void)
{
	init ();
	while (1) 
	{	
		Sleep (200);
		initdata ();
		if (play () == 0)
		{
			break;
		}
	}
	closegraph ();
	return 0;
}

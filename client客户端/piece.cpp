#include "piece.h"

//暂时存储移动前的棋子或隔板信息，之后一系列判断中虽然不改变图像但会改变各项坐标的值，所以如果最后判断出不能移动，则可恢复各项坐标
int buffpx, buffpy, buffa, buffb; //棋子
static int kbkeep = 1; //用于关闭棋子闪烁线程

int pkeep = 0;

//总控制棋子
unsigned __stdcall pThread (void *pParam)
{
	struct player *pn = (struct player *)pParam;
	char c;   //获取键盘按键
	HANDLE kbThreadH;
	unsigned kbThreadID;
	
	kbkeep = 1;
	kbThreadH = (HANDLE)_beginthreadex (NULL, 0, &kbThread, (LPVOID)pn, 0, &kbThreadID);
	CloseHandle (kbThreadH);
	
	while (1)  //键盘控制棋子移动循环
	{
		move = 0;
		while (move != 'w' && move != 's' && move != 'a' && move != 'd')//等待按键时判断是否关闭线程
		{
			if (pkeep == 0)
			{
				kbkeep = 0;
				putimage (pn->px, pn->py, &piece[pn->name]);
				return 0;
			}
			Sleep (50);
		}
		c = move;
		if (judgepiece (pn, c) == 1) //若按键所指下一步无效则继续循环重新获取按键
		{	
			//存储棋子移动前的各项坐标
			buffa = pn->a; buffb = pn->b;
			buffpx = pn->px; buffpy = pn->py;
			movepiece (pn, c);    //先移动坐标，但不移动图像
			if (!b[pn->a][pn->b]) //若移动后的格子上本来没棋，则擦去旧图像，输出新图像
			{
				realmovepiece (pn);
				break;  //按键有效之后就跳出键盘循环
			}
			else			//若移动后的格子上本来就有棋
			{
				if (judgepiece (pn, c) == 1) //则判断是否能跳过该棋，若能则
				{
					movepiece (pn, c); //再先移动坐标，但不移动图像
					if (!b[pn->a][pn->b]) //再判断移动后的格子是否被占用
					{
						realmovepiece (pn); //未被占用就移动
						break;  //棋子移动之后就退出键盘循环
					}		
				}
				else if (judgepiece (pn, c) == 2)  //若因为有隔板而不能跳，则
				{
					if (obliquejump (pn, c)) //判断能否斜跳
					{	
						realmovepiece (pn); //若能则移动
						break; //棋子移动之后就退出键盘循环
					}
				}
				//若下一步无效或有棋，则把坐标回归未移动前，并重新循环获取按键
				pn->a = buffa; pn->b = buffb;
				pn->px = buffpx; pn->py = buffpy;
			}  
		}
	}
	return 0;
}

//闪烁棋子
unsigned __stdcall kbThread (void *pParam)
{
	int i = 0;
	struct player *buff = (struct player *)pParam;
	int x = buff->px;
	int y = buff->py;
	int n = buff->name;
	while (kbkeep)
	{
		if (i == 0)
		{
			setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
			bar (x, y, x + 39, y + 39);	
		}
		if (i == 10)
		{
			putimage (x, y, &piece[n]);
		}
		Sleep (50);
		i = (i + 1) % 20;
	}
	return 0;
}

//判断棋子按键有效性: 若按键是wsad之一，则判断棋子是否移动后超出棋盘或有隔板阻挡
//可移动则返回1，超出棋盘则为0，有隔板则为2，若为其他按键则返回0
//根据棋子的数组坐标判断是否下一步会超出棋盘，根据棋子边上的格子占用状态判断是否有隔板
int judgepiece (struct player *pn, char key)
{
	int sign = 1;
	switch (key)
	{
	case 'w': 
		if (pn->b - 2 < 0)  sign = 0; 
		if (b[pn->a][pn->b - 1] == 1) sign = 2; 
		break;
	case 's': 
		if (pn->b + 2 > 18) sign = 0; 
		if (b[pn->a][pn->b + 1] == 1) sign = 2;
		break;	
	case 'a': 
		if (pn->a - 2 < 0)  sign = 0; 
		if (b[pn->a - 1][pn->b]) sign = 2;
		break;	
	case 'd': 
		if (pn->a + 2 > 18) sign = 0; 
		if (b[pn->a + 1][pn->b]) sign = 2;
		break;	
	default : sign = 0;
	}
	return sign;
}

//根据按键只移动棋子的各项坐标
void movepiece (struct player *pn, char key)
{
	switch (key)
	{
	case 'w': pn->py -= 45; pn->b -= 2; break;
	case 's': pn->py += 45; pn->b += 2; break;
	case 'a': pn->px -= 45; pn->a -= 2; break;
	case 'd': pn->px += 45; pn->a += 2; break;
	}
}

//判断能否斜跳，若能则返回1，且棋子的各项坐标已经移动，不能则返回0
//先获取按键判断用户朝哪里斜跳，然后判断是否能朝这个方向斜跳，即判断是否有隔板阻挡或下一格是否有棋子占据，方法同judgepiece函数
int obliquejump(struct player *pn, char c)
{
	
	char key;
	int sign = 0;
	int keep = 2;
	int w = 0, s = 0, a = 0, d = 0;
	if (c == 'w' || c == 's')
	{ 
		if (judgepiece (pn, 'a') == 1 && b[pn->a - 1][pn->b] == 0)  //若为有效按键，则keep=0退出循环
		{
			putimage (pn->px - 45, pn->py, &piece[pn->name]);
			a = 1;
		}
		else
			keep -= 1;
		if (judgepiece (pn, 'd') == 1 && b[pn->a + 1][pn->b] == 0)
		{
			putimage (pn->px + 45, pn->py, &piece[pn->name]);
			d = 1;
		}
		else
			keep -= 1;
		while (keep)
		{
			move = 0;
			while (move != 'a' && move != 'd' && move != 'q')//等待按键时判断是否关闭线程
			{
				if (pkeep == 0)
				{
					//kbkeep = 0;
					key = 'q';
					break;
				}
				Sleep (50);
			}
			if (key != 'q')
			{
				key = move;
			}
			switch (key)
			{
			case 'a': 
				if (a == 1)
				{
					if (d = 1)
					{
						setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
						bar (pn->px + 45, pn->py, pn->px + 84, pn->py + 39);
					}
					movepiece (pn, key);
					sign = 1;
					keep = 0;
				}
				break;
			case 'd':
				if (d == 1)
				{
					if (a = 1)
					{
						setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
						bar (pn->px - 45, pn->py, pn->px - 6, pn->py + 39);
					}
					movepiece (pn, key);
					sign = 1;
					keep = 0;
				}
				break;
			case 'q':
				keep = 0;
				if (d = 1)
				{
					setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
					bar (pn->px + 45, pn->py, pn->px + 84, pn->py + 39);
				}
				if (a = 1)
				{
					setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
					bar (pn->px - 45, pn->py, pn->px - 6, pn->py + 39);
				}
				break;
			}
		}
	}
	if (c =='a' || c == 'd') 
	{
		if (judgepiece (pn, 'w') == 1 && b[pn->a][pn->b - 1] == 0)  //若为有效按键，则keep=0退出循环
		{
			putimage (pn->px, pn->py - 45, &piece[pn->name]);
			w = 1;
		}
		if (judgepiece (pn, 's') == 1 && b[pn->a][pn->b + 1] == 0)
		{
			putimage (pn->px, pn->py + 45, &piece[pn->name]);
			s = 1;
		}
		while (keep)
		{
			move = 0;
			while (move != 'w' && move != 's' && move != 'q')//等待按键时判断是否关闭线程
			{
				if (pkeep != 0)
				{
					//kbkeep = 0;
					key = 'q';
					break;
				}
				Sleep (50);
			}
			if (key == 'q')
			{
				key = move;
			}
			switch (key)
			{
			case 'w': 
				if (w == 1)
				{
					if (s = 1)
					{
						setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
						bar (pn->px, pn->py + 45, pn->px + 39, pn->py + 84);
					}
					movepiece (pn, key);
					sign = 1;
					keep = 0;
				}
				break;
			case 's':
				if (s == 1)
				{
					if (w = 1)
					{
						setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
						bar (pn->px, pn->py - 45, pn->px + 39, pn->py - 6);
					}
					movepiece (pn, key);
					sign = 1;
					keep = 0;
				}
				break;
			case 'q':
				keep = 0;
				if (s = 1)
				{
					setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
					bar (pn->px, pn->py + 45, pn->px + 39, pn->py + 84);
				}
				if (w = 1)
				{
					setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
					bar (pn->px, pn->py - 45, pn->px + 39, pn->py - 6);
				}
				break;
			}
		}
	}
	return sign;
}

//真正移动棋子：删除旧位置的棋子和格子占有状态，画出新位置的棋子，添加占有状态
void realmovepiece (struct player *pn)
{
	setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);
	bar (buffpx, buffpy, buffpx + 39, buffpy + 39);
	b[buffa][buffb] = 0;
	putimage (pn->px, pn->py, &piece[pn->name]);
	b[pn->a][pn->b] = 1;

	pieceorbar = 1;
	undo = 1;
	kbkeep = 0;
	mkeep = 0;
	pkeep = 0;
}



#include "bar.h"

int buffx, buffy, buffa1, buffb1, buffa2, buffa3, buffb2, buffb3, buffd; //隔板占三个，a1为横着时左边一格或竖着时上边一格

int bkeep = 0;

//总控制隔板
unsigned __stdcall bThread (void *pParam)
{
	struct player *pn = (struct player *)pParam;
	char c;
	ban.x = X1 + 5;   //隔板初始化在左上角最外层
	ban.y = Y1;
	putimage (ban.x, ban.y, &piece[5]);  
	ban.a1 = 1; ban.b1 = 0;
	ban.a2 = 2; ban.b2 = 0;
	ban.a3 = 3; ban.b3 = 0;
	ban.d = -1;
	while (1)  //用于多次移动隔板直到确认放下的循环
	{
		move = 0;
		while (move != 'w' && move != 's' && move != 'a' && move != 'd' && move != 'r' && move != ' ')//等待按键时判断是否关闭线程
		{
			if (bkeep == 0)
			{
				b[ban.a1][ban.b1] = 0; //直接删去占用和图片
				b[ban.a2][ban.b2] = 0;
				b[ban.a3][ban.b3] = 0;
				setfillstyle (WHITE, SOLID_FILL, NULL);
				if (ban.d == -1)
					bar (ban.x, ban.y, ban.x + 84, ban.y + 4);
				else
					bar (ban.x, ban.y, ban.x + 4, ban.y + 84);
				return 0;
			}
			Sleep (50);
		}
		c = move;
		if (c == ' ' && ban.b1 != 0)  //按回车并且隔板不在初始位置，就放下隔板并退出多次移动隔板的循环
		{
			if (judgeclose () == 0)
			{
				MessageBox(graph, TEXT ("有棋子被封闭，此处不能放置隔板！"), TEXT ("注意"),MB_OK);
				continue;
			}
			pn->bars -=1;  //该回合玩家的隔板数减1
			remainbar (pn);
			pieceorbar = 2;
			undo = 1;
			mkeep = 0;
			bkeep = 0;
			break;
		}
		if (c == 'r') rotate (); //按空格旋转隔板
		//存储旋转后的隔板各项坐标
		buffa1 = ban.a1; buffb1 = ban.b1;
		buffa2 = ban.a2; buffb2 = ban.b2;
		buffa3 = ban.a3; buffb3 = ban.b3;
		buffx = ban.x; buffy = ban.y;
		buffd = ban.d;
		while (1)  //移动一次隔板的循环
		{
			if (judgebar (c))  //判断按键所指下一步是否出界
			{
				b[buffa1][buffb1] = 0;  //未出界则清空旧的隔板占用，以方便之后判断是否需要跳
				b[buffa2][buffb2] = 0;
				b[buffa3][buffb3] = 0;
				movebar (c);  //先只移动坐标
				//若移动后的隔板所占三格之前均未被占用，则清空旧的占用和图片，添加新的占用和图片
				//若有占用，则重新开始移动一次隔板的循环来判断能否再移动
				if (b[ban.a1][ban.b1] == 0 && b[ban.a2][ban.b2] == 0 && b[ban.a3][ban.b3] == 0)
				{
					realmovebar ();
					break;  //移动一次隔板后跳出移动一次隔板的循环
				}
			}
			else  //若最后判断出界，则标示不能移动，恢复移动前的各项坐标和占用
			{
				ban.a1 = buffa1; ban.b1 = buffb1;
				ban.a2 = buffa2; ban.b2 = buffb2;
				ban.a3 = buffa3; ban.b3 = buffb3;
				ban.x = buffx; ban.y = buffy;
				b[buffa1][buffb1] = 1;
				b[buffa2][buffb2] = 1;
				b[buffa3][buffb3] = 1;
				break;  //最终判断按键无效后退出移动一次隔板的循环
			}
		}	
	}
	return 0;
}

//清除旧的隔板的占有状态和图像，添加新的占有状态和图像
void realmovebar ()
{
	b[buffa1][buffb1] = 0;
	b[buffa2][buffb2] = 0;
	b[buffa3][buffb3] = 0;
	setfillstyle (WHITE, SOLID_FILL, NULL);
	if (buffd == -1)
		bar (buffx, buffy, buffx + 84, buffy + 4);
	else
		bar (buffx, buffy, buffx + 4, buffy + 84);
	b[ban.a1][ban.b1] = 1;
	b[ban.a2][ban.b2] = 1;
	b[ban.a3][ban.b3] = 1;
	if (ban.d == -1)
		putimage (ban.x, ban.y, &piece[5]);
	else
		putimage (ban.x, ban.y, &piece[7]);
}

//旋转隔板
void rotate ()
{
	if (ban.b2 != 0 && b[ban.a1 + 1][ban.b1 + 1] == 0 && b[ban.a3 - 1][ban.b3 - 1] == 0)
	{
		//存储隔板旋转前的各项坐标
		buffa1 = ban.a1; buffb1 = ban.b1;
		buffa2 = ban.a2; buffb2 = ban.b2;
		buffa3 = ban.a3; buffb3 = ban.b3;
		buffx = ban.x; buffy = ban.y;
		buffd = ban.d;
		if (ban.d == -1) //若隔板横着
		{
			ban.a1 += 1; ban.b1 -= 1;
			ban.a3 -= 1; ban.b3 += 1;
			ban.x += 40; ban.y -= 40;
			ban.d = -ban.d; //改变横竖状态
			realmovebar ();
		}
		else  //若竖着
		{
			ban.a1 -= 1; ban.b1 += 1;
			ban.a3 += 1; ban.b3 -= 1;
			ban.x -= 40; ban.y += 40;
			ban.d = -ban.d; //改变横竖状态
			realmovebar ();
		}
	}
}

//判断隔板按键有效性: 若按键是wsad之一，则判断隔板是否移动后超出棋盘，可移动则返回1，若为其他按键则或不能移动，返回0
//根据隔板的数组坐标判断是否出界
int judgebar (char key)
{
	int sign = 1;
	switch (key)
	{
	case 'w': if (ban.b2 - 2 < 1)  sign = 0; break;
	case 's': if (ban.b2 + 2 > 17) sign = 0; break;	
	case 'a': if (ban.a2 - 2 < 1)  sign = 0; break;	
	case 'd': if (ban.a2 + 2 > 17) sign = 0; break;	 
	default : sign = 0;
	}
	return sign;
}


//根据按键只移动隔板的各项坐标
void movebar (char key)
{
	switch (key)
	{
	case 'w': 
		ban.y -= 45;
		ban.b1 -= 2; ban.b2 -= 2; ban.b3 -= 2;
		break;
	case 's':
		ban.y += 45;
		ban.b1 += 2; ban.b2 += 2; ban.b3 += 2;
		break;
	case 'a': 
		ban.x -= 45;
		ban.a1 -= 2; ban.a2 -= 2; ban.a3 -= 2;
		break;
	case 'd':
		ban.x += 45;
		ban.a1 += 2; ban.a2 += 2; ban.a3 += 2;
		break;
	}
}

//判断隔板是否封闭了某个棋子，即创建虚拟棋子判断能否到达终点
/*
思路：先将棋子向上移到有隔板挡住或到顶。然后记录下坐标，开始用走迷宫的方法，
即每一步都按上一步逆时针一个方向开始顺时针判断是否被挡住，没被挡住就移动，
如此保证棋子沿着记录时倚靠的一大片墙的一边运动，若该片墙连接边界，那棋子必然能到终点。
如果该片墙未连接边缘，又分两种情况：1 被包围，2 没被包围，这两种情况下，
走迷宫的方法会循环回记录点，为解决此问题，在走迷宫时记录到达过的最高点，
最高点若和记录点一样高，则一定是被包围的情况。若最高点高于记录点，则转成向上模式，
直到被挡住，重新记录初始点，再转换成走迷宫模式，如此循环，若未被包围就能到达终点。
这样实际上是利用向上模式脱离之前走迷宫循环过的不连接边界的墙。若是被包围的情况，
因为初始点必然越来越高，所以第二次记录的初始点就会在包围圈内的最高点，因此走迷宫循环回来后，
该循环最高点就是记录点，即判断出是被包围的情况。
还有特殊情况：在走迷宫时，原本应该可以走到终点，但是途中不可避免地经过了记录点。
这样分为两种情况，1 从右边到达记录点，则额外判断记录点的上左下是否被挡住，
三方中只要一方没有挡住，则允许继续走迷宫，若都有隔板，则认作循环了。
2 从下方到达记录点，则额外判断记录点左方是否被挡住，若没有则允许继续走迷宫。
*/

int judgeclose ()
{
	enum direct {up = 1, right, down, left} direct; //上一次移动的方向
	int alwaysupornot; //1为向上模式，0为走迷宫模式
	
	int copyx, copyy; //创建虚拟棋子
	int recordx, recordy; //记录向上模式被阻挡后开始走迷宫时的坐标
	
	int change; //用于判断是否需要由向上模式换成走迷宫模式
	int largex, largey; //用于记录走迷宫模式时走到的最高点坐标

	int sign; //该函数返回值，0为不能放下隔板，1为可以
	int i, j;
	
	b[1][0] = 1; //把隔板初始化移动后消去的三个格子的占用补上
	b[2][0] = 1;
	b[3][0] = 1;
	sign = 1;
	for (i = 1; i <= number; i++) //根据玩家个数判断封闭
	{
		copyx = p[i]->a;
		copyy = p[i]->b;
		//将虚拟棋子向上移到有隔板挡住或到顶为止
		while (b[copyx][copyy - 1] == 0)
		{
			copyy -= 2;
		}
		//初始化数据
		recordx = copyx;
		recordy = copyy;
		largex = recordx;
		largey = recordy;
		alwaysupornot = 0;
		change = 0;
		direct = right; //初始为右，以保证先判断上方
		while (1)
		{
			//根据不同棋子判断是否到达终点
			j = 0;
			switch (i)
			{
			case 1: if (copyy == 1) j = 1; break;
			case 2: if (copyy == 17) j = 1; break;
			case 3: if (copyx == 17) j = 1; break;
			case 4: if (copyx == 1) j = 1; break;
			}
			if (j == 1) break;
			//根据上一次移动的方向判断此次选择方向的顺序
			switch (direct) 
			{
			case up: //上一步向上，则按当时的规则分情况
				//若为走迷宫模式，则按上次移动方向的逆时针一个方向开始顺时针判断
				if (alwaysupornot == 0) 
				{
					if (b[copyx - 1][copyy] == 0) //左
					{
						copyx -= 2;
						direct = left;
					}
					else if (b[copyx][copyy - 1] == 0)//上
					{
						copyy -= 2;
						direct = up;
					}
					else if (b[copyx + 1][copyy] == 0)//右
					{
						copyx += 2;
						direct = right;
					}
					else if (b[copyx][copyy + 1] == 0)//下
					{
						copyy += 2;
						direct = down;
					}
					else
					{
						return 0;
					}
					break;
				}
				//若为向上模式时则直接向上，碰到上方有板，则转换成不带向上优先规则
				else 
				{
					if (b[copyx][copyy - 1] == 0)
					{
						copyy -= 2;
						direct = up;
					}
					else
					{
						change = 1; //为下面的判断标记要取消向上优先
					}
					break;
				}
			case down: //其他三个方向只会在走迷宫时用到
				if (b[copyx + 1][copyy] == 0)
				{
					copyx += 2;
					direct = right;
				}
				else if (b[copyx][copyy + 1] == 0)
				{
					copyy += 2;
					direct = down;
				}
				else if (b[copyx - 1][copyy] == 0)
				{
					copyx -= 2;
					direct = left;
				}
				else if (b[copyx][copyy - 1] == 0)
				{
					copyy -= 2;
					direct = up;
				}
				else
				{
					return 0;
				}
				break;
			case left:
				if (b[copyx][copyy + 1] == 0)
				{
					copyy += 2;
					direct = down;
				}
				else if (b[copyx - 1][copyy] == 0)
				{
					copyx -= 2;
					direct = left;
				}
				else if (b[copyx][copyy - 1] == 0)
				{
					copyy -= 2;
					direct = up;
				}
				else if (b[copyx + 1][copyy] == 0)
				{
					copyx += 2;
					direct = right;
				}
				else
				{
					return 0;
				}
				break;
			case right:
				if (b[copyx][copyy - 1] == 0)
				{
					copyy -= 2;
					direct = up;
				}
				else if (b[copyx + 1][copyy] == 0)
				{
					copyx += 2;
					direct = right;
				}
				else if (b[copyx][copyy + 1] == 0)
				{
					copyy += 2;
					direct = down;
				}
				else if (b[copyx - 1][copyy] == 0)
				{
					copyx -= 2;
					direct = left;
				}
				else
				{
					return 0;
				}
				break;
			} //switch
			//记录走迷宫循环的最高点
			if (alwaysupornot == 0 && copyy < largey)
			{
				largex = copyx;
				largey = copyy;
			}
			//若在走迷宫模式时回到了记录点，则为循环了
			if (alwaysupornot == 0 && copyx == recordx && copyy == recordy)
			{
				//若为两种特殊情况则允许继续走迷宫
				if ((direct == left && (b[copyx][copyy - 1] == 0 || b[copyx - 1][copyy] == 0 || b[copyx][copyy + 1] == 0)) || 
				    (direct == up && b[copyx - 1][copyy] == 0))
				{
					continue;
				}
				//若确认循环了，则转换成向上模式
				else if (largey < recordy)
				{
					alwaysupornot = 1;
					direct = up; //确保向上模式开始时就一直向上
					copyx = largex; //把棋子移到该循环的最高点
					copyy = largey;
				}
				//最高点和记录点等高，则为被包围
				else
				{
					return 0;
				}
			}
			//若在向上模式时被挡住，则转换成走迷宫模式
			else if (alwaysupornot == 1 && change == 1)
			{	
				alwaysupornot = 0;
				recordx = copyx; //记录新的初始点
				recordy = copyy;
				largex = recordx; //把最高点初始为记录点
				largey = recordy;
				direct = right; //确保走迷宫从上方开始判断
				change = 0; //调回不改变模式
			}
		} //while
	} //for
	return sign;
}

//用于不同玩家每次隔板数减1后重新输出剩余隔板数
void remainbar (struct player *pn)
{
	char str[20];
	setcolor (LIGHTRED);
	setfont (15  , 0, "楷体", 0, 0, 600, false, false, false);
	sprintf (str, "剩余隔板数：%d ", pn->bars);
	switch (pn->name)
	{
		case 1: outtextxy (X2 - 100, Y2 + 30, str); break;
		case 2: outtextxy (X1, Y1 - 20, str); break;
		case 3: outtextxy (X1 - 120, Y2 - 30, str); break;
		case 4: outtextxy (X2 + 10, Y1 + 40, str); break;
	}
}

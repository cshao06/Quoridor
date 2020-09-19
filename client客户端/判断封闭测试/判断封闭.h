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
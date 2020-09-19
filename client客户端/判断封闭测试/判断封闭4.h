//完成所有棋子的判断,采用多次记录取消向上优先时的起始点并记录次数的方法

//判断隔板是否封闭了某个棋子，即创建虚拟棋子判断能否到达终点
int judgeclose ()
{
	enum direct {up = 1, right, down, left} direct; //上一次移动的方向
	int alwaysupornot; //用于判断是否采用向上优先的规则
	
	int copyx, copyy; //创建虚拟棋子
	int recordx, recordy; //记录棋子第一次被阻挡时的坐标
	
	int recordx1, recordy1; //记录取消向上优先时的坐标
	int change; //用于判断是否需要取消向上优先的规则
	int countchange; //记录取消向上优先的次数
	int recordx2, recordy2; //记录第7次（足够多）取消向上优先时的坐标，用于判断循环取消向上优先的情况

	int sign; //该函数返回值，0为不能放下隔板，1为可以
	int i, j;
	
	sign = 1;
	for (i = 1; i <= number; i++) //根据玩家个数判断封闭
	{
		copyx = p[i]->a;
		copyy = p[i]->b;
		//将虚拟棋子向上移到有隔板挡住为止
		while (b[copyx][copyy - 1] == 0 && copyy != 1)
		{
			copyy -= 2;
		}
		//初始化数据
		recordx = copyx;
		recordy = copyy;
		recordx1 = recordx;
		recordy1 = recordy;
		alwaysupornot = 0;
		change = 0;
		countchange = 0;
		direct = right; //初始为右，以保证先判断上方
		while (1)
		{
			//根据棋子判断是否到达终点
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
			case up:
				if (alwaysupornot == 0) //若没有向上优先的规则，则按上次移动方向的逆时针一个方向开始顺时针判断
				{
					if (b[copyx - 1][copyy] == 0 && copyx != 1) //左
					{
						copyx -= 2;
						direct = left;
					}
					else if (b[copyx][copyy - 1] == 0 && copyy != 1)//上
					{
						copyy -= 2;
						direct = up;
					}
					else if (b[copyx + 1][copyy] == 0 && copyx != 17)//右
					{
						copyx += 2;
						direct = right;
					}
					else if (b[copyx][copyy + 1] == 0 && copyy != 17)//下
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
				else //带有向上优先规则时碰到上方有板，则转换成不带向上优先规则
				{
					if (b[copyx][copyy - 1] == 0 && copyy != 1)
					{
						copyy -= 2;
						direct = up;
					}
					else
					{
						change = 1;
					}
					break;
				}
			case down:
				if (b[copyx + 1][copyy] == 0 && copyx != 17)
				{
					copyx += 2;
					direct = right;
				}
				else if (b[copyx][copyy + 1] == 0 && copyy != 17)
				{
					copyy += 2;
					direct = down;
				}
				else if (b[copyx - 1][copyy] == 0 && copyx != 1)
				{
					copyx -= 2;
					direct = left;
				}
				else if (b[copyx][copyy - 1] == 0 && copyy != 1)
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
				if (b[copyx][copyy + 1] == 0 && copyy != 17)
				{
					copyy += 2;
					direct = down;
				}
				else if (b[copyx - 1][copyy] == 0 && copyx != 1)
				{
					copyx -= 2;
					direct = left;
				}
				else if (b[copyx][copyy - 1] == 0 && copyy != 1)
				{
					copyy -= 2;
					direct = up;
				}
				else if (b[copyx + 1][copyy] == 0 && copyx != 17)
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
				if (b[copyx][copyy - 1] == 0 && copyy != 1)
				{
					copyy -= 2;
					direct = up;
				}
				else if (b[copyx + 1][copyy] == 0 && copyx != 17)
				{
					copyx += 2;
					direct = right;
				}
				else if (b[copyx][copyy + 1] == 0 && copyy != 17)
				{
					copyy += 2;
					direct = down;
				}
				else if (b[copyx - 1][copyy] == 0 && copyx != 1)
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
			if (alwaysupornot == 0 && copyx == recordx1 && copyy == recordy1)
			{
				if (direct != left)
				{
					alwaysupornot = 1;
					direct = right;
				}
				else
				{
					continue;
				}
			}
			else if (alwaysupornot == 1 && copyx == recordx && copyy == recordy)
			{
				return 0;
			}
			else if (alwaysupornot == 1 && change == 1)
			{
				if (countchange >= 7 && copyx == recordx2 && copyy == recordy2)
				{
					return 0;	
				}
				else
				{		
					alwaysupornot = 0;
					recordx1 = copyx;
					recordy1 = copyy;
					direct = right;
					countchange += 1;
					if (countchange == 7)
					{
						recordx2 = copyx;
						recordy2 = copyy;
					}
					change = 0;
				}
			}
		} //while
	} //for
	return sign;
}
//在顺逆基础上加入向上优先规则的切换（不合理）

//顺时针尝试，返回0则为封闭，返回1则为不封闭，返回2则为转换成逆时针，返回3则为回到了第一次记录的位置

enum direct {up = 1, right, down, left} direct; //上一次移动的方向
int firstorsecond; //用于判断是第一次顺时针尝试还是第二次逆时针尝试
int alwaysupornot; //用于判断是否采用向上优先的规则

int clockwise (int *x, int *y, int recordx, int recordy)
{
	while (1) //顺时针尝试
	{
		if (*y == 1)
		{
			return 1;
		}
		switch (direct) //上一次移动的方向
		{
		case up:
			if (alwaysupornot == 0)
			{
				if (b[*x - 1][*y] == 0 && *x != 1)
				{
					*x -= 2;
					direct = left;
				}
				else if (b[*x][*y - 1] == 0 && *y != 1)
				{
					*y -= 2;
					direct = up;
				}
				else if (b[*x + 1][*y] == 0 && *x != 17)
				{
					*x += 2;
					direct = right;
				}
				
				else if (b[*x][*y + 1] == 0 && *y != 17)
				{
					*y += 2;
					direct = down;
				}
				else
				{
					return 0;
				}
				break;
			}
			else
			{
				if (b[*x][*y - 1] == 0 && *y != 1)
				{
					*y -= 2;
					direct = up;
				}
				else if (b[*x + 1][*y] == 0 && *x != 17)
				{
					*x += 2;
					direct = right;
				}
				else if (b[*x - 1][*y] == 0 && *x != 1)
				{
					*x -= 2;
					direct = left;
					return 2;
				}
				else if (b[*x][*y + 1] == 0 && *y != 17)
				{
					*y += 2;
					direct = down;
				}
				else
				{
					return 0;
				}
				break;
			}
		case down:
			if (b[*x + 1][*y] == 0 && *x != 17)
			{
				*x += 2;
				direct = right;
			}
			else if (b[*x][*y + 1] == 0 && *y != 17)
			{
				*y += 2;
				direct = down;
			}
			else if (b[*x - 1][*y] == 0 && *x != 1)
			{
				*x -= 2;
				direct = left;
			}
			else if (b[*x][*y - 1] == 0 && *y != 1)
			{
				*y -= 2;
				direct = up;
			}
			else
			{
				return 0;
			}
			break;
		case left:
			if (b[*x][*y + 1] == 0 && *y != 17)
			{
				*y += 2;
				direct = down;
			}
			else if (b[*x - 1][*y] == 0 && *x != 1)
			{
				*x -= 2;
				direct = left;
			}
			else if (b[*x][*y - 1] == 0 && *y != 1)
			{
				*y -= 2;
				direct = up;
			}
			else if (b[*x + 1][*y] == 0 && *x != 17)
			{
				*x += 2;
				direct = right;
			}
			else
			{
				return 0;
			}
			break;
		case right:
			if (b[*x][*y - 1] == 0 && *y != 1)
			{
				*y -= 2;
				direct = up;
			}
			else if (b[*x + 1][*y] == 0 && *x != 17)
			{
				*x += 2;
				direct = right;
			}
			else if (b[*x][*y + 1] == 0 && *y != 17)
			{
				*y += 2;
				direct = down;
			}
			else if (b[*x - 1][*y] == 0 && *x != 1)
			{
				*x -= 2;
				direct = left;
			}
			else
			{
				return 0;
			}
			break;
		}
		if (*x == recordx && *y == recordy)
		{	
			if (alwaysupornot == 1)
			{
				if (firstorsecond == 2)
					return 0;
				else
					return 3;
			}
			else
			{
				alwaysupornot = 1;
				continue;
			}
			
		}
	}
}

//逆时针尝试，返回0则为封闭，返回1则为不封闭，返回2则为转换成顺时针，返回3则为回到了第一次记录的位置
int anticlockwise (int *x, int *y, int recordx, int recordy)
{
	while (1) //逆时针尝试
	{
		if (*y == 1)
		{
			return 1;
		}
		switch (direct)
		{
		case up:
			if (b[*x][*y - 1] == 0 && *y != 1)
			{
				*y -= 2;
				direct = up;
			}
			else if (b[*x - 1][*y] == 0 && *x != 1)
			{
				*x -= 2;
				direct = left;
			}
			else if (b[*x + 1][*y] == 0 && *x != 17)
			{
				*x += 2;
				direct = right;
				return 2;
			}
			else if (b[*x][*y + 1] == 0 && *y != 17)
			{
				*y += 2;
				direct = down;
			}
			break;
		case left:
			if (b[*x][*y - 1] == 0 && *y != 1)
			{
				*y -= 2;
				direct = up;
			}
			else if (b[*x - 1][*y] == 0 && *x != 1)
			{
				*x -= 2;
				direct = left;
			}
			else if (b[*x][*y + 1] == 0 && *y != 17)
			{
				*y += 2;
				direct = down;
			}
			else if (b[*x + 1][*y] == 0 && *x != 17)
			{
				*x += 2;
				direct = right;
			}
			break;
		case right:
			if (b[*x][*y + 1] == 0 && *y != 17)
			{
				*y += 2;
				direct = down;
			}
			else if (b[*x + 1][*y] == 0 && *x != 17)
			{
				*x += 2;
				direct = right;
			}
			else if (b[*x][*y - 1] == 0 && *y != 1)
			{
				*y -= 2;
				direct = up;
			}
			else if (b[*x - 1][*y] == 0 && *x != 1)
			{
				*x -= 2;
				direct = left;
			}
			break;
		case down:
			if (b[*x - 1][*y] == 0 && *x != 1)
			{
				*x -= 2;
				direct = left;
			}
			else if (b[*x][*y + 1] == 0 && *y != 17)
			{
				*y += 2;
				direct = down;
			}
			else if (b[*x + 1][*y] == 0 && *x != 17)
			{
				*x += 2;
				direct = right;
			}
			else if (b[*x][*y - 1] == 0 && *y != 1)
			{
				*y -= 2;
				direct = up;
			}
			break;
		}
		if (*x == recordx && *y == recordy)
		{
			if (firstorsecond == 2)
				return 0;
			else
				return 3;
		}
	}
}

//创建虚拟棋子判断能否到达终点
int judgeclose ()
{
	int sign;
	int i;
	int copyx, copyy; //创建虚拟棋子
	int *x = &copyx, *y = &copyy;
	int recordx, recordy; //记录棋子第一次被阻挡时的坐标
	copyx = p[1]->a;
	copyy = p[1]->b;
	while (b[copyx][copyy - 1] == 0 && copyy != 1)
	{
		copyy -= 2;
	}
	recordx = copyx;
	recordy = copyy;
	firstorsecond = 1;
	alwaysupornot = 0;
	for (i = 1; ; i = -i)
	{
		if (i == 1)
		{
			if (sign == 3)
			{
				direct = (enum direct)((direct + 2) % 4);
				sign = clockwise (x, y, recordx, recordy);	
			}
			else
			{
				direct = right;
				sign = clockwise (x, y, recordx, recordy);
			}
			if (sign == 0)
				return 0;
			else if (sign == 1)
				return 1;
			else if (sign == 3)
				firstorsecond = 2;
		}
		else
		{
			if (sign == 3)
			{
				direct = (enum direct)((direct + 2) % 4);
				sign = anticlockwise (x, y, recordx, recordy);
			}
			else
			{
				direct = left;
				sign = anticlockwise (x, y, recordx, recordy);
			}
			if (sign == 0)
				return 0;
			else if (sign == 1)
				return 1;
			else if (sign == 3)
			{
				firstorsecond = 2;
				i = 1;
			}
		}	
	}	
}
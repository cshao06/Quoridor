//����������ӵ��ж�,���ö�μ�¼ȡ����������ʱ����ʼ�㲢��¼�����ķ���

//�жϸ����Ƿ�����ĳ�����ӣ����������������ж��ܷ񵽴��յ�
int judgeclose ()
{
	enum direct {up = 1, right, down, left} direct; //��һ���ƶ��ķ���
	int alwaysupornot; //�����ж��Ƿ�����������ȵĹ���
	
	int copyx, copyy; //������������
	int recordx, recordy; //��¼���ӵ�һ�α��赲ʱ������
	
	int recordx1, recordy1; //��¼ȡ����������ʱ������
	int change; //�����ж��Ƿ���Ҫȡ���������ȵĹ���
	int countchange; //��¼ȡ���������ȵĴ���
	int recordx2, recordy2; //��¼��7�Σ��㹻�ࣩȡ����������ʱ�����꣬�����ж�ѭ��ȡ���������ȵ����

	int sign; //�ú�������ֵ��0Ϊ���ܷ��¸��壬1Ϊ����
	int i, j;
	
	sign = 1;
	for (i = 1; i <= number; i++) //������Ҹ����жϷ��
	{
		copyx = p[i]->a;
		copyy = p[i]->b;
		//���������������Ƶ��и��嵲סΪֹ
		while (b[copyx][copyy - 1] == 0 && copyy != 1)
		{
			copyy -= 2;
		}
		//��ʼ������
		recordx = copyx;
		recordy = copyy;
		recordx1 = recordx;
		recordy1 = recordy;
		alwaysupornot = 0;
		change = 0;
		countchange = 0;
		direct = right; //��ʼΪ�ң��Ա�֤���ж��Ϸ�
		while (1)
		{
			//���������ж��Ƿ񵽴��յ�
			j = 0;
			switch (i)
			{
			case 1: if (copyy == 1) j = 1; break;
			case 2: if (copyy == 17) j = 1; break;
			case 3: if (copyx == 17) j = 1; break;
			case 4: if (copyx == 1) j = 1; break;
			}
			if (j == 1) break;
			//������һ���ƶ��ķ����жϴ˴�ѡ�����˳��
			switch (direct) 
			{
			case up:
				if (alwaysupornot == 0) //��û���������ȵĹ������ϴ��ƶ��������ʱ��һ������ʼ˳ʱ���ж�
				{
					if (b[copyx - 1][copyy] == 0 && copyx != 1) //��
					{
						copyx -= 2;
						direct = left;
					}
					else if (b[copyx][copyy - 1] == 0 && copyy != 1)//��
					{
						copyy -= 2;
						direct = up;
					}
					else if (b[copyx + 1][copyy] == 0 && copyx != 17)//��
					{
						copyx += 2;
						direct = right;
					}
					else if (b[copyx][copyy + 1] == 0 && copyy != 17)//��
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
				else //�����������ȹ���ʱ�����Ϸ��а壬��ת���ɲ����������ȹ���
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
/*
˼·���Ƚ����������Ƶ��и��嵲ס�򵽶���Ȼ���¼�����꣬��ʼ�����Թ��ķ�����
��ÿһ��������һ����ʱ��һ������ʼ˳ʱ���ж��Ƿ񱻵�ס��û����ס���ƶ���
��˱�֤�������ż�¼ʱ�п���һ��Ƭǽ��һ���˶�������Ƭǽ���ӱ߽磬�����ӱ�Ȼ�ܵ��յ㡣
�����Ƭǽδ���ӱ�Ե���ַ����������1 ����Χ��2 û����Χ������������£�
���Թ��ķ�����ѭ���ؼ�¼�㣬Ϊ��������⣬�����Թ�ʱ��¼���������ߵ㣬
��ߵ����ͼ�¼��һ���ߣ���һ���Ǳ���Χ�����������ߵ���ڼ�¼�㣬��ת������ģʽ��
ֱ������ס�����¼�¼��ʼ�㣬��ת�������Թ�ģʽ�����ѭ������δ����Χ���ܵ����յ㡣
����ʵ��������������ģʽ����֮ǰ���Թ�ѭ�����Ĳ����ӱ߽��ǽ�����Ǳ���Χ�������
��Ϊ��ʼ���ȻԽ��Խ�ߣ����Եڶ��μ�¼�ĳ�ʼ��ͻ��ڰ�ΧȦ�ڵ���ߵ㣬������Թ�ѭ��������
��ѭ����ߵ���Ǽ�¼�㣬���жϳ��Ǳ���Χ�������
������������������Թ�ʱ��ԭ��Ӧ�ÿ����ߵ��յ㣬����;�в��ɱ���ؾ����˼�¼�㡣
������Ϊ���������1 ���ұߵ����¼�㣬������жϼ�¼����������Ƿ񱻵�ס��
������ֻҪһ��û�е�ס��������������Թ��������и��壬������ѭ���ˡ�
2 ���·������¼�㣬������жϼ�¼�����Ƿ񱻵�ס����û��������������Թ���
*/

int judgeclose ()
{
	enum direct {up = 1, right, down, left} direct; //��һ���ƶ��ķ���
	int alwaysupornot; //1Ϊ����ģʽ��0Ϊ���Թ�ģʽ
	
	int copyx, copyy; //������������
	int recordx, recordy; //��¼����ģʽ���赲��ʼ���Թ�ʱ������
	
	int change; //�����ж��Ƿ���Ҫ������ģʽ�������Թ�ģʽ
	int largex, largey; //���ڼ�¼���Թ�ģʽʱ�ߵ�����ߵ�����

	int sign; //�ú�������ֵ��0Ϊ���ܷ��¸��壬1Ϊ����
	int i, j;
	
	b[1][0] = 1; //�Ѹ����ʼ���ƶ�����ȥ���������ӵ�ռ�ò���
	b[2][0] = 1;
	b[3][0] = 1;
	sign = 1;
	for (i = 1; i <= number; i++) //������Ҹ����жϷ��
	{
		copyx = p[i]->a;
		copyy = p[i]->b;
		//���������������Ƶ��и��嵲ס�򵽶�Ϊֹ
		while (b[copyx][copyy - 1] == 0)
		{
			copyy -= 2;
		}
		//��ʼ������
		recordx = copyx;
		recordy = copyy;
		largex = recordx;
		largey = recordy;
		alwaysupornot = 0;
		change = 0;
		direct = right; //��ʼΪ�ң��Ա�֤���ж��Ϸ�
		while (1)
		{
			//���ݲ�ͬ�����ж��Ƿ񵽴��յ�
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
			case up: //��һ�����ϣ��򰴵�ʱ�Ĺ�������
				//��Ϊ���Թ�ģʽ�����ϴ��ƶ��������ʱ��һ������ʼ˳ʱ���ж�
				if (alwaysupornot == 0) 
				{
					if (b[copyx - 1][copyy] == 0) //��
					{
						copyx -= 2;
						direct = left;
					}
					else if (b[copyx][copyy - 1] == 0)//��
					{
						copyy -= 2;
						direct = up;
					}
					else if (b[copyx + 1][copyy] == 0)//��
					{
						copyx += 2;
						direct = right;
					}
					else if (b[copyx][copyy + 1] == 0)//��
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
				//��Ϊ����ģʽʱ��ֱ�����ϣ������Ϸ��а壬��ת���ɲ����������ȹ���
				else 
				{
					if (b[copyx][copyy - 1] == 0)
					{
						copyy -= 2;
						direct = up;
					}
					else
					{
						change = 1; //Ϊ������жϱ��Ҫȡ����������
					}
					break;
				}
			case down: //������������ֻ�������Թ�ʱ�õ�
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
			//��¼���Թ�ѭ������ߵ�
			if (alwaysupornot == 0 && copyy < largey)
			{
				largex = copyx;
				largey = copyy;
			}
			//�������Թ�ģʽʱ�ص��˼�¼�㣬��Ϊѭ����
			if (alwaysupornot == 0 && copyx == recordx && copyy == recordy)
			{
				//��Ϊ�����������������������Թ�
				if ((direct == left && (b[copyx][copyy - 1] == 0 || b[copyx - 1][copyy] == 0 || b[copyx][copyy + 1] == 0)) || 
				    (direct == up && b[copyx - 1][copyy] == 0))
				{
					continue;
				}
				//��ȷ��ѭ���ˣ���ת��������ģʽ
				else if (largey < recordy)
				{
					alwaysupornot = 1;
					direct = up; //ȷ������ģʽ��ʼʱ��һֱ����
					copyx = largex; //�������Ƶ���ѭ������ߵ�
					copyy = largey;
				}
				//��ߵ�ͼ�¼��ȸߣ���Ϊ����Χ
				else
				{
					return 0;
				}
			}
			//��������ģʽʱ����ס����ת�������Թ�ģʽ
			else if (alwaysupornot == 1 && change == 1)
			{	
				alwaysupornot = 0;
				recordx = copyx; //��¼�µĳ�ʼ��
				recordy = copyy;
				largex = recordx; //����ߵ��ʼΪ��¼��
				largey = recordy;
				direct = right; //ȷ�����Թ����Ϸ���ʼ�ж�
				change = 0; //���ز��ı�ģʽ
			}
		} //while
	} //for
	return sign;
}
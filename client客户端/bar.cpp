#include "bar.h"

int buffx, buffy, buffa1, buffb1, buffa2, buffa3, buffb2, buffb3, buffd; //����ռ������a1Ϊ����ʱ���һ�������ʱ�ϱ�һ��

int bkeep = 0;

//�ܿ��Ƹ���
unsigned __stdcall bThread (void *pParam)
{
	struct player *pn = (struct player *)pParam;
	char c;
	ban.x = X1 + 5;   //�����ʼ�������Ͻ������
	ban.y = Y1;
	putimage (ban.x, ban.y, &piece[5]);  
	ban.a1 = 1; ban.b1 = 0;
	ban.a2 = 2; ban.b2 = 0;
	ban.a3 = 3; ban.b3 = 0;
	ban.d = -1;
	while (1)  //���ڶ���ƶ�����ֱ��ȷ�Ϸ��µ�ѭ��
	{
		move = 0;
		while (move != 'w' && move != 's' && move != 'a' && move != 'd' && move != 'r' && move != ' ')//�ȴ�����ʱ�ж��Ƿ�ر��߳�
		{
			if (bkeep == 0)
			{
				b[ban.a1][ban.b1] = 0; //ֱ��ɾȥռ�ú�ͼƬ
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
		if (c == ' ' && ban.b1 != 0)  //���س����Ҹ��岻�ڳ�ʼλ�ã��ͷ��¸��岢�˳�����ƶ������ѭ��
		{
			if (judgeclose () == 0)
			{
				MessageBox(graph, TEXT ("�����ӱ���գ��˴����ܷ��ø��壡"), TEXT ("ע��"),MB_OK);
				continue;
			}
			pn->bars -=1;  //�ûغ���ҵĸ�������1
			remainbar (pn);
			pieceorbar = 2;
			undo = 1;
			mkeep = 0;
			bkeep = 0;
			break;
		}
		if (c == 'r') rotate (); //���ո���ת����
		//�洢��ת��ĸ����������
		buffa1 = ban.a1; buffb1 = ban.b1;
		buffa2 = ban.a2; buffb2 = ban.b2;
		buffa3 = ban.a3; buffb3 = ban.b3;
		buffx = ban.x; buffy = ban.y;
		buffd = ban.d;
		while (1)  //�ƶ�һ�θ����ѭ��
		{
			if (judgebar (c))  //�жϰ�����ָ��һ���Ƿ����
			{
				b[buffa1][buffb1] = 0;  //δ��������վɵĸ���ռ�ã��Է���֮���ж��Ƿ���Ҫ��
				b[buffa2][buffb2] = 0;
				b[buffa3][buffb3] = 0;
				movebar (c);  //��ֻ�ƶ�����
				//���ƶ���ĸ�����ռ����֮ǰ��δ��ռ�ã�����վɵ�ռ�ú�ͼƬ������µ�ռ�ú�ͼƬ
				//����ռ�ã������¿�ʼ�ƶ�һ�θ����ѭ�����ж��ܷ����ƶ�
				if (b[ban.a1][ban.b1] == 0 && b[ban.a2][ban.b2] == 0 && b[ban.a3][ban.b3] == 0)
				{
					realmovebar ();
					break;  //�ƶ�һ�θ���������ƶ�һ�θ����ѭ��
				}
			}
			else  //������жϳ��磬���ʾ�����ƶ����ָ��ƶ�ǰ�ĸ��������ռ��
			{
				ban.a1 = buffa1; ban.b1 = buffb1;
				ban.a2 = buffa2; ban.b2 = buffb2;
				ban.a3 = buffa3; ban.b3 = buffb3;
				ban.x = buffx; ban.y = buffy;
				b[buffa1][buffb1] = 1;
				b[buffa2][buffb2] = 1;
				b[buffa3][buffb3] = 1;
				break;  //�����жϰ�����Ч���˳��ƶ�һ�θ����ѭ��
			}
		}	
	}
	return 0;
}

//����ɵĸ����ռ��״̬��ͼ������µ�ռ��״̬��ͼ��
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

//��ת����
void rotate ()
{
	if (ban.b2 != 0 && b[ban.a1 + 1][ban.b1 + 1] == 0 && b[ban.a3 - 1][ban.b3 - 1] == 0)
	{
		//�洢������תǰ�ĸ�������
		buffa1 = ban.a1; buffb1 = ban.b1;
		buffa2 = ban.a2; buffb2 = ban.b2;
		buffa3 = ban.a3; buffb3 = ban.b3;
		buffx = ban.x; buffy = ban.y;
		buffd = ban.d;
		if (ban.d == -1) //���������
		{
			ban.a1 += 1; ban.b1 -= 1;
			ban.a3 -= 1; ban.b3 += 1;
			ban.x += 40; ban.y -= 40;
			ban.d = -ban.d; //�ı����״̬
			realmovebar ();
		}
		else  //������
		{
			ban.a1 -= 1; ban.b1 += 1;
			ban.a3 += 1; ban.b3 -= 1;
			ban.x -= 40; ban.y += 40;
			ban.d = -ban.d; //�ı����״̬
			realmovebar ();
		}
	}
}

//�жϸ��尴����Ч��: ��������wsad֮һ�����жϸ����Ƿ��ƶ��󳬳����̣����ƶ��򷵻�1����Ϊ��������������ƶ�������0
//���ݸ�������������ж��Ƿ����
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


//���ݰ���ֻ�ƶ�����ĸ�������
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

//�жϸ����Ƿ�����ĳ�����ӣ����������������ж��ܷ񵽴��յ�
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

//���ڲ�ͬ���ÿ�θ�������1���������ʣ�������
void remainbar (struct player *pn)
{
	char str[20];
	setcolor (LIGHTRED);
	setfont (15  , 0, "����", 0, 0, 600, false, false, false);
	sprintf (str, "ʣ���������%d ", pn->bars);
	switch (pn->name)
	{
		case 1: outtextxy (X2 - 100, Y2 + 30, str); break;
		case 2: outtextxy (X1, Y1 - 20, str); break;
		case 3: outtextxy (X1 - 120, Y2 - 30, str); break;
		case 4: outtextxy (X2 + 10, Y1 + 40, str); break;
	}
}

#include "piece.h"

//��ʱ�洢�ƶ�ǰ�����ӻ������Ϣ��֮��һϵ���ж�����Ȼ���ı�ͼ�񵫻�ı���������ֵ�������������жϳ������ƶ�����ɻָ���������
int buffpx, buffpy, buffa, buffb; //����
static int kbkeep = 1; //���ڹر�������˸�߳�

int pkeep = 0;

//�ܿ�������
unsigned __stdcall pThread (void *pParam)
{
	struct player *pn = (struct player *)pParam;
	char c;   //��ȡ���̰���
	HANDLE kbThreadH;
	unsigned kbThreadID;
	
	kbkeep = 1;
	kbThreadH = (HANDLE)_beginthreadex (NULL, 0, &kbThread, (LPVOID)pn, 0, &kbThreadID);
	CloseHandle (kbThreadH);
	
	while (1)  //���̿��������ƶ�ѭ��
	{
		move = 0;
		while (move != 'w' && move != 's' && move != 'a' && move != 'd')//�ȴ�����ʱ�ж��Ƿ�ر��߳�
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
		if (judgepiece (pn, c) == 1) //��������ָ��һ����Ч�����ѭ�����»�ȡ����
		{	
			//�洢�����ƶ�ǰ�ĸ�������
			buffa = pn->a; buffb = pn->b;
			buffpx = pn->px; buffpy = pn->py;
			movepiece (pn, c);    //���ƶ����꣬�����ƶ�ͼ��
			if (!b[pn->a][pn->b]) //���ƶ���ĸ����ϱ���û�壬���ȥ��ͼ�������ͼ��
			{
				realmovepiece (pn);
				break;  //������Ч֮�����������ѭ��
			}
			else			//���ƶ���ĸ����ϱ���������
			{
				if (judgepiece (pn, c) == 1) //���ж��Ƿ����������壬������
				{
					movepiece (pn, c); //�����ƶ����꣬�����ƶ�ͼ��
					if (!b[pn->a][pn->b]) //���ж��ƶ���ĸ����Ƿ�ռ��
					{
						realmovepiece (pn); //δ��ռ�þ��ƶ�
						break;  //�����ƶ�֮����˳�����ѭ��
					}		
				}
				else if (judgepiece (pn, c) == 2)  //����Ϊ�и��������������
				{
					if (obliquejump (pn, c)) //�ж��ܷ�б��
					{	
						realmovepiece (pn); //�������ƶ�
						break; //�����ƶ�֮����˳�����ѭ��
					}
				}
				//����һ����Ч�����壬�������ع�δ�ƶ�ǰ��������ѭ����ȡ����
				pn->a = buffa; pn->b = buffb;
				pn->px = buffpx; pn->py = buffpy;
			}  
		}
	}
	return 0;
}

//��˸����
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

//�ж����Ӱ�����Ч��: ��������wsad֮һ�����ж������Ƿ��ƶ��󳬳����̻��и����赲
//���ƶ��򷵻�1������������Ϊ0���и�����Ϊ2����Ϊ���������򷵻�0
//�������ӵ����������ж��Ƿ���һ���ᳬ�����̣��������ӱ��ϵĸ���ռ��״̬�ж��Ƿ��и���
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

//���ݰ���ֻ�ƶ����ӵĸ�������
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

//�ж��ܷ�б���������򷵻�1�������ӵĸ��������Ѿ��ƶ��������򷵻�0
//�Ȼ�ȡ�����ж��û�������б����Ȼ���ж��Ƿ��ܳ��������б�������ж��Ƿ��и����赲����һ���Ƿ�������ռ�ݣ�����ͬjudgepiece����
int obliquejump(struct player *pn, char c)
{
	
	char key;
	int sign = 0;
	int keep = 2;
	int w = 0, s = 0, a = 0, d = 0;
	if (c == 'w' || c == 's')
	{ 
		if (judgepiece (pn, 'a') == 1 && b[pn->a - 1][pn->b] == 0)  //��Ϊ��Ч��������keep=0�˳�ѭ��
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
			while (move != 'a' && move != 'd' && move != 'q')//�ȴ�����ʱ�ж��Ƿ�ر��߳�
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
		if (judgepiece (pn, 'w') == 1 && b[pn->a][pn->b - 1] == 0)  //��Ϊ��Ч��������keep=0�˳�ѭ��
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
			while (move != 'w' && move != 's' && move != 'q')//�ȴ�����ʱ�ж��Ƿ�ر��߳�
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

//�����ƶ����ӣ�ɾ����λ�õ����Ӻ͸���ռ��״̬��������λ�õ����ӣ����ռ��״̬
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



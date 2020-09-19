#include "quoridor.h"

/**********************************ȫ�ֱ���****************************/
HWND graph;
HWND console;

int online = IDNO;
int restart = IDYES;    //�������¿�ʼ����
int pieceorbar; //�����жϷ�������1���Ǹ���2,���ڻ��幦��
int undo; //0Ϊ���ɻ��壬1Ϊ���Ի��壬2ΪҪ����

IMAGE piece[8];
char number; //��Ϸ����
float xasp, yasp;
int b[19][19];  //���������жϻ��Ƶĺ��ģ������±�Ϊ���ӵ����꣬����Ϊռ��״̬��0δռ�ã�1ռ�á�
		//һ������ռһ��һ������ռ�����м�һ���ֵΪ1��
		//��9���8�ۣ�0��18Ϊ���Ե�������ӣ��������ж��Ƿ����

struct player p1 = {"", 1, X1 + 4 * 45 + 5, Y2 - 45, 10, X2 - 80, Y2 + 10, 9, 17, 1, &p1.b}; //��ɫ �·�
struct player p2 = {"", 2, X1 + 4 * 45 + 5, Y1 + 5, 10, X1, Y1 - 40, 9, 1, 17, &p2.b};       //��ɫ �Ϸ�
struct player p3 = {"", 3, X1 + 5, Y1 + 4 * 45 + 5, 10, X1 - 100, Y2 - 50, 1, 9, 17, &p3.a}; //��ɫ ��
struct player p4 = {"", 4, X2 - 45, Y1 + 4 * 45 + 5, 10, X2 + 10, Y1 + 20, 17, 9, 1, &p4.a}; //��ɫ �ҷ�
struct player *p[5] = {NULL, &p1, &p2, &p3, &p4};  //�����������������Ϣ����pnָ��
struct player pbuff[5]; //pbuff�������������л��������¿�ʼ��Ϸ��ֵ

struct bans ban; //�����������й������������

int mkeep = 1;//�������ѭ��
int keykeep = 1;
char move;

/***********************************************************************/


//�ж��Ƿ�����ʤ���������򷵻�0�������򷵻���һ��ұ��
int winornext (struct player *pn)
{
	int next;
	if (*pn->nowline != pn->winline) //�ж�ʤ��ʱ��λ��==��׼����ʤ��
	{		
		if (number == 2)  //������������ж���һ���
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
	int n = 1; //���غ��������
	int previous = 1; //��һ�غ�������֣����ڻ���
	struct pack sendpiece;
	struct pack sendbar;
	while (1)
	{
		//����غ���Ϣ
		setcolor (YELLOW);
		setfont (20, 0, "����", 0, 0, 400, false, false, false);
		outtextxy (X1 - 50, Y1 - 130, "���غ��ǣ�");
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
					return 1;     //��play�����а������¿�ʼ����ֱ�����¿�ʼ��Ϸ
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
				if ((n = winornext (p[n])) == 0) //�ж�ʤ���͸ı����
				{
					//�Ƿ����¿�ʼ
					restart = MessageBox(graph, TEXT ("��ҵ��߻�ʤ�� �Ƿ����¿�ʼ��"), TEXT ("��Ϸ����"),MB_YESNO); 
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
				if ((n = winornext (p[n])) == 0) //�ж�ʤ���͸ı����
				{
					//�Ƿ����¿�ʼ
					restart = MessageBox(graph, TEXT ("��ҵ��߻�ʤ�� �Ƿ����¿�ʼ��"), TEXT ("��Ϸ����"),MB_YESNO); 
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
				return 1;     //��play�����а������¿�ʼ����ֱ�����¿�ʼ��Ϸ
			}
			if (undo == 2)
			{
				undo = 0;
				undofunc (p[previous]);
				n = previous; //ʹ�ûغ�������ָ�Ϊ��һ�غ�
				continue;
			}
			previous = n; //��Ϊ�������ж�ʤ����n���Ϊ��һ�غ���ң�����previous�ȼ̳�n
			if ((n = winornext (p[n])) == 0) //�ж�ʤ���͸ı����
			{
				//�Ƿ����¿�ʼ
				restart = MessageBox(graph, TEXT ("��ҵ��߻�ʤ�� �Ƿ����¿�ʼ��"), TEXT ("��Ϸ����"),MB_YESNO); 
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

//һ���غϵĿ��ƺ��������ݲ����ж�����һ����ң������ڵĸ�������Լ����ø��ֺ��������ݲ�ͬ����Զ��仯
//�����ȫ�ֽṹ�������ָ������þ����ڴ�
void myturn (struct player *pn)
{
	int sign = 0;
	MOUSEMSG m;
	HANDLE pThreadH, bThreadH;
	unsigned pThreadID, bThreadID;
	struct pack sendsignal;
	mkeep = 1;
	while (1)  //���ѭ��
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
		if (m.uMsg == WM_LBUTTONDOWN) //�������Ϣ���ǰ���������򷴸���ȡm
		{
			//����갴�����ӷ�Χ��
			if (m.x >= pn->px * xasp && m.x <= (pn->px + 39) * xasp && m.y >= pn->py * yasp && m.y <= (pn->py + 39) * yasp && pkeep == 0)  
			{	
				undo = 0;
				bkeep = 0;
				pkeep = 1;
				pThreadH = (HANDLE)_beginthreadex (NULL, 0, pThread, (LPVOID)pn, 0, &pThreadID);
				CloseHandle (pThreadH);
			}
			//����갴�ڸ����ʾ��Χ��
			else if (m.x >= pn->bx * xasp && m.x <= (pn->bx + 79) * xasp && m.y >= pn->by * yasp && m.y <= (pn->by + 9) *yasp && pn->bars != 0 && bkeep == 0) 
			{
				undo = 0;
				pkeep = 0;
				bkeep = 1;
				bThreadH = (HANDLE)_beginthreadex (NULL, 0, bThread, (LPVOID)pn, 0, &bThreadID);
				CloseHandle (bThreadH);
			}
			//������ڡ����¿�ʼ����Χ��
			else if (m.x >= (X2 - 140) * xasp && m.x <= (X2 - 40) * xasp && m.y >= (Y1 - 130) * yasp && m.y <= (Y1 - 100) * yasp) 
			{
				if (online == IDYES)
				{	
					restart = MessageBox(graph, TEXT ("�Ƿ����¿�ʼ��Ϸ�����ǣ���ȴ��������ͬ��"), TEXT ("���¿�ʼ"),MB_YESNO);
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
							MessageBox(graph, TEXT ("�����˶�ͬ�����¿�ʼ"), TEXT ("���¿�ʼ"),MB_OK);
							break;
						}
						else
						{
							sendsignal.flag = NORESTART;
							mysend ((char *)&sendsignal, sizeof (struct pack));
							MessageBox(graph, TEXT ("���˲�ͬ�����¿�ʼ"), TEXT ("���¿�ʼ"),MB_OK);
						}
					}
				}
				else
				{
					restart = MessageBox(graph, TEXT ("�Ƿ����¿�ʼ��Ϸ��"), TEXT ("���¿�ʼ"),MB_YESNO);
					if (restart == IDYES) 
					{
						break;
					}
				}
			}
			//������ڡ����塱��Χ��������ɻ�
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
		setfillstyle (LIGHTGRAY, SOLID_FILL, NULL);  //�����ƶ�ǰ�洢����Ϣ�ָ����������ͼƬ
		bar (pre->px, pre->py, pre->px + 39, pre->py + 39);
		b[pre->a][pre->b] = 0;
		pre->px = buffpx; pre->py = buffpy;
		pre->a = buffa;   pre->b = buffb;
		putimage (buffpx, buffpy, &piece[pre->name]);
		b[buffa][buffb] = 1;
	}
	else
	{
		b[ban.a1][ban.b1] = 0; //ֱ��ɾȥռ�ú�ͼƬ
		b[ban.a2][ban.b2] = 0;
		b[ban.a3][ban.b3] = 0;
		setfillstyle (WHITE, SOLID_FILL, NULL);
		if (ban.d == -1)
			bar (ban.x, ban.y, ban.x + 84, ban.y + 4);
		else
			bar (ban.x, ban.y, ban.x + 4, ban.y + 84);
		pre->bars += 1; //ǰһ��ҵĸ������ָ�
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

//��������ѭ���������¿�ʼ��Ϸ
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

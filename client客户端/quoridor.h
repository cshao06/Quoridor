#ifndef QUORIDOR_H
#define QUORIDOR_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

#include "initgame.h"
#include "client.h"
#include "piece.h"
#include "bar.h"
#include "clientplay.h"

#pragma comment(lib, "Ws2_32.lib")

#define X0 700 //���������С
#define Y0 750 
#define X1 146 //�������Ͻ�����
#define Y1 226
#define X2 556 //�������½�����
#define Y2 636
#define PI 3.1415926535 //������תͼƬ

struct bans
{
	int x; //�������Ͻ�����
	int y; 
	int a1;//������ռ�����������һ������������
	int b1;
	int a2;//ͬ�ϣ��м�
	int b2;
	int a3;//ͬ�ϣ��һ���
	int b3;
	int d;  //-1��1��
};

struct pack
{
	char flag;
	char sender;
	char buff[BUFFLEN];
	int px, py;
	int a, b;
	struct bans buffban;
};

struct player
{
	char username[20];//�û���
	int name;//����
	int px;   //�������Ͻ�����
	int py;
	int bars;  //ʣ�������
	int bx; //�����ʾ���Ͻ�����
	int by;
	int a;     //�����й����������
	int b;     //�����й����������
	int winline;//�ж�ʤ��ʱ�ı�׼
	int *nowline;//�ж�ʤ��ʱ��λ��
};

extern HWND graph;
extern HWND console;

extern int online;
extern int restart;    //Ϊ��play����������¿�ʼ����
extern int pieceorbar; //�����жϷ�������1���Ǹ���2
extern int undo;//0Ϊ���ɻ��壬1Ϊ���Ի���

extern IMAGE piece[8];
extern char number; //��Ϸ����
extern float xasp, yasp;
extern int b[19][19];  //���������жϻ��Ƶĺ��ģ������±�Ϊ���ӵ����꣬����Ϊռ��״̬��0δռ�ã�1ռ�á�
		//һ������ռһ��һ������ռ�����м�һ���ֵΪ1��
		//��9���8�ۣ�0��18Ϊ���Ե�������ӣ��������ж��Ƿ����

extern struct player p1; //��ɫ �·�
extern struct player p2;       //��ɫ �Ϸ�
extern struct player p3; //��ɫ ��
extern struct player p4; //��ɫ �ҷ�
extern struct player *p[5];  //�����������������Ϣ����pnָ��
extern struct player pbuff[5]; //pbuff�������������л��������¿�ʼ��Ϸ��ֵ

extern struct bans ban; //�����������й������������

extern int mkeep;
extern int keykeep;
extern char move;

int play ();
void myturn (struct player *pn);
int winornext (struct player *pn);
void undofunc (struct player *pre);
unsigned __stdcall keyThread (void *pParam);

#endif //QUORIDOR_H
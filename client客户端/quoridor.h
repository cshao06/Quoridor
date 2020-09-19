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

#define X0 700 //整个画面大小
#define Y0 750 
#define X1 146 //棋盘左上角坐标
#define Y1 226
#define X2 556 //棋盘右下角坐标
#define Y2 636
#define PI 3.1415926535 //用于旋转图片

struct bans
{
	int x; //隔板左上角坐标
	int y; 
	int a1;//隔板所占三格中左或上一个的数组坐标
	int b1;
	int a2;//同上，中间
	int b2;
	int a3;//同上，右或下
	int b3;
	int d;  //-1横1竖
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
	char username[20];//用户名
	int name;//名字
	int px;   //棋子左上角坐标
	int py;
	int bars;  //剩余隔板数
	int bx; //隔板标示左上角坐标
	int by;
	int a;     //格子有关数组的列数
	int b;     //格子有关数组的行数
	int winline;//判断胜利时的标准
	int *nowline;//判断胜利时的位置
};

extern HWND graph;
extern HWND console;

extern int online;
extern int restart;    //为了play函数里的重新开始功能
extern int pieceorbar; //用于判断返回棋子1还是隔板2
extern int undo;//0为不可悔棋，1为可以悔棋

extern IMAGE piece[8];
extern char number; //游戏人数
extern float xasp, yasp;
extern int b[19][19];  //整个程序判断机制的核心，数组下标为格子的坐标，内容为占用状态，0未占用，1占用。
		//一个棋子占一格；一个隔板占三格，中间一格的值为1。
		//共9格加8槽，0和18为最边缘，不放子，但用于判断是否出界

extern struct player p1; //红色 下方
extern struct player p2;       //蓝色 上方
extern struct player p3; //黄色 左方
extern struct player p4; //绿色 右方
extern struct player *p[5];  //各个函数传递玩家信息都用pn指针
extern struct player pbuff[5]; //pbuff用于在主函数中缓存后给重新开始游戏赋值

extern struct bans ban; //隔板的坐标和有关数组的行列数

extern int mkeep;
extern int keykeep;
extern char move;

int play ();
void myturn (struct player *pn);
int winornext (struct player *pn);
void undofunc (struct player *pre);
unsigned __stdcall keyThread (void *pParam);

#endif //QUORIDOR_H
#ifndef CLIENT_H
#define CLIENT_H

#include "quoridor.h"

#define PORT 3579
#define BUFFLEN 256 //用于发送接收数据

extern char clientNo;
//用于发送接收数据


//函数声明
unsigned __stdcall cliThread (void *pParam);
int initclient ();
int initconnect ();
int waitforgame ();
void mysend (char *sendbuff, int len);
void myrecv (char *recvbuff, int len);

//BOOL WINAPI ConsoleHandler (DWORD CEvent);

#endif //CLIENT_H
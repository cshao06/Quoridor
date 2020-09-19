#ifndef CLIENTPLAY_H
#define CLIENTPLAY_H

#include "quoridor.h"

#define MSG		1
#define PIECE		2
#define BAR		3
#define RESTART		4
#define AGREE		5
#define DISAGREE	6
#define YESRESTART	7
#define NORESTART	8
#define READY		9

#define BUFFLEN 256

extern int reply;
extern int ready[5];
extern struct pack recvready;

unsigned __stdcall recvThread (void *pParam);
unsigned __stdcall talkThread (void *pParam);
//unsigned __stdcall sendThread (void *pParam);
void remoteturn (struct player *pn);

#endif //CLIENTPLAY_H
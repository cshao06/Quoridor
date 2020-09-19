#ifndef PIECE_H
#define PIECE_H

#include "quoridor.h"

extern int pkeep;
extern int buffpx, buffpy, buffa, buffb; 

unsigned __stdcall pThread (void *pParam);
unsigned __stdcall kbThread (void *pParam);
int judgepiece (struct player *pn, char key);
void movepiece (struct player *pn, char key);
int obliquejump(struct player *pn, char c);
void realmovepiece (struct player *pn);

#endif
#ifndef BAR_H
#define BAR_H

#include "quoridor.h"

extern int bkeep;
extern int buffx, buffy, buffa1, buffb1, buffa2, buffa3, buffb2, buffb3, buffd;

unsigned __stdcall bThread (void *pParam);
void realmovebar ();
void rotate ();
int judgebar (char key);
void movebar (char key);
int judgeclose ();
void remainbar (struct player *pn);

#endif
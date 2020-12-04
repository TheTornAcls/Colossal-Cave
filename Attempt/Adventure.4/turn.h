#ifndef __TURN_H
#define __TURN_H

void turn();
void describe();
void descitem();

void domove();
void goback();
void copytrv(PTRAV trav1, PTRAV trav2);
void dotrav();

void badmove();
void spcmove(int rdest);
void dwarfend();

void normend();
void score();
void death();
void doobj();

void trobj();
char * probj();
void dwarves();
void dopirate();

int stimer();

#endif 

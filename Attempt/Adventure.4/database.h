#ifndef __DATABASE_H
#define __DATABASE_H

void gettrav(int loc);

int dark();

int here(int item);
int toting(int item);
int forced(int atloc);
int at(int item);

int pct(int x);

void dstroy(int obj);
void move(int obj, int where);
void juggle(int loc);
void carry(int obj, int where);

void drop(int obj, int where);

void rspeak(int msg);
void pspeak(int item, int state);
void desclg(int loc);
void descsh(int loc);

int vocab(char *word,int val);
int binary(char *w, WAC wctable[], int maxwc);

int yes(int msg1, int msg2,int msg3);

void bug(int n);

int put(int obj,int where,int pval);
int dcheck();
int liq();
int liqloc(int loc);

int liq2(int pbottle);

#endif


/*	program ITVERB.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h".		*/


#include <strsafe.h>
#include	<stdio.h>	/* drv = 1.1st file 2.def 3.A	*/
#include	"advent.h"
#include	"advdec.h"

#include "itverb.h"
#include "verb.h"
#include "database.h"
#include "turn.h"

/*
	Routines to process intransitive verbs
    An Intranstive Verb expresses a complete action without the aid of a direct object
*/
void itverb()
{
	switch(g_iVerb) {
	case DROP:
	case SAY:
	case WAVE:
	case CALM:
	case RUB:
	case THROW:
	case FIND:
	case FEED:
	case BREAK:
	case WAKE:
		needobj();
		break;
	case TAKE:
		ivtake();
		break;
	case OPEN:
	case LOCK:
		ivopen();
		break;
	case NOTHING:
		rspeak(54);
		break;
	case ON:
	case OFF:
	case POUR:
		trverb();
		break;
	case WALK:
		actspk(g_iVerb);
		break;
	case KILL:
		ivkill();
		break;
	case EAT:
		iveat();
		break;
	case DRINK:
		ivdrink();
		break;
	case QUIT:
		ivquit();
		break;
	case FILL:
		ivfill();
		break;
	case BLAST:
		vblast();
		break;
	case SCORE:
		score();
		break;
	case FOO:
		ivfoo();
		break;
	case SUSPEND:
		saveflg = 1;
		break;
	case INVENTORY:
		inventory();
		break;
	default:
		printf("This intransitive not implemented yet\n");
	}
}

/*
	CARRY, TAKE etc.
*/
void ivtake()
{
	int anobj,item;

	anobj = 0;
	for(item=1;item<MAXOBJ;++item) {
		if(place[item]==g_iLoc) {
			if(anobj != 0) {
				needobj();
				return;
			}
			anobj = item;
		}
	}
	if(anobj==0|| (dcheck() && dflag>=2)) {
		needobj();
		return;
	}
	g_iObject = anobj;
	vtake();
}

/*
	OPEN, LOCK, UNLOCK
*/
void ivopen()
{
	if(here(CLAM))
		g_iObject=CLAM;
	if(here(OYSTER))
		g_iObject=OYSTER;
	if(at(DOOR))
		g_iObject=DOOR;
	if(at(GRATE))
		g_iObject=GRATE;
	if(here(CHAIN)) {
		if(g_iObject != 0) {
			needobj();
			return;
		}
		g_iObject=CHAIN;
	}
	if(g_iObject==0) {
		rspeak(28);
		return;
	}
	vopen();
}

/*
	ATTACK, KILL etc
*/
void ivkill()
{
	object1 = 0;
	if(dcheck() && dflag >=2)
		g_iObject=DWARF;
	if(here(SNAKE))
		addobj(SNAKE);
	if(at(DRAGON) && prop[DRAGON]==0)
		addobj(DRAGON);
	if(at(TROLL))
		addobj(TROLL);
	if(here(BEAR) && prop[BEAR]==0)
		addobj(BEAR);
	if(object1 != 0) {
		needobj();
		return;
	}
	if(g_iObject != 0) {
		vkill();
		return;
	}
	if(here(BIRD) && g_iVerb!= THROW)
		g_iObject=BIRD;
	if(here(CLAM) || here(OYSTER))
		addobj(CLAM);
	if(object1 != 0) {
		needobj();
		return;
	}
	vkill();
}

/*
	EAT
*/
void iveat()
{
	if(!here(FOOD))
		needobj();
	else {
		g_iObject=FOOD;
		veat();
	}
}

/*
	DRINK
*/
void ivdrink()
{
	if(liqloc(g_iLoc) != WATER &&
	  (liq()!= WATER || !here(BOTTLE)))
		needobj();
	else {
		g_iObject=WATER;
		vdrink();
	}
}

/*
	QUIT
*/
void ivquit()
{
	if(gaveup=yes(22,54,54))
		normend();
}

/*
	FILL
*/
void ivfill()
{
	if(!here(BOTTLE))
		needobj();
	else {
		g_iObject=BOTTLE;
		vfill();
	}
}

/*
	Handle fee fie foe foo...
*/
void ivfoo()
{
	char k;
    int msg;

	k = vocab(word1,3000);
	msg = 42;
	if (foobar != 1-k) {
		if (foobar != 0)
			msg = 151;
		rspeak(msg);
		return;
	}
	foobar = k;
	if (k != 4)
		return;
	foobar = 0;
	if (place[EGGS] == 92 ||
	   (toting(EGGS) && g_iLoc == 92)) {
		rspeak(msg);
		return;
	}
	if (place[EGGS] == 0 && place[TROLL] == 0 &&
	    prop[TROLL] == 0)
		prop[TROLL] = 1;
	if (here(EGGS))
		k = 1;
	else if (g_iLoc == 92)
		k = 0;
	else
		k = 2;
	move(EGGS,92);
	pspeak(EGGS,k);
	return;
}

/*
	read etc...
*/
/*  no room for this...
ivread()
{
	if (here(MAGAZINE))
		g_iObject = MAGAZINE;
	if (here(TABLET))
		g_iObject = g_iObject*100 + TABLET;
	if (here(MESSAGE))
		g_iObject = g_iObject*100 + MESSAGE;
	if (g_iObject > 100 || g_iObject == 0 || dark()) {
		needobj();
		return;
	}
	vread();
}
*/

/*
	INVENTORY 
*/
void inventory()
{
	int msg;
	int i;

	msg = 98;
	for (i=1; i<=MAXOBJ; ++i) {
		if (i==BEAR || !toting(i))
			continue;
		if (msg)
			rspeak(99);
		msg = 0;
		pspeak(i,-1);
	}
	if (toting(BEAR))
		msg = 141;
	if (msg)
		rspeak(msg);
}

/*
	ensure uniqueness as objects are searched
	out for an intransitive verb
*/
void addobj(int obj)
{
	if(object1 != 0)
		return;
	if(g_iObject != 0) {
		object1 = -1;
		return;
	}
	g_iObject = obj;
}

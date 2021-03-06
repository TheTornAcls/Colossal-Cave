
/*  ITVERB.C  no mod for V 1.43  */

#include "advent.h"

/*
        Routines to process intransitive verbs
*/
VOID PASCAL itverb(VOID)
{
    switch (verb)
    {
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
	    actspk(verb);
	    break;
	case KILL:
	    ivkill();
	    break;
#if 0
	case EAT:
	    iveat();
	    break;
#endif
	case DRINK:
	    ivdrink();
	    break;
	case QUIT:
	    ivquit();
	    break;
#if 0
	case FILL:
	    ivfill();
	     break;
#endif
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
	case SAVE:
	    saveadv();
	    describe();
	    descitem();
	    break;
	case RESTORE:
	    restore();
	    describe();
	    descitem();
	    break;
	case BRIEF:
	case VERBOSE:
	    brief_sw = (BRIEF == verb);
	    rspeak(54);
	    break;
	default:
	    printf("This intransitive not implemented yet\n");
    }

    return;
}

/*
        CARRY, TAKE etc.
*/
VOID PASCAL ivtake(VOID)
{
    auto     SHORT	anobj, item;

    anobj = 0;
    for (item = 1; item < MAXOBJ; ++item)
    {
	if (place[item] == loc)
	{
	    if (anobj != 0)
	    {
		needobj();
		return;
	    }
	    anobj = item;
	}
    }
    if (anobj == 0 || (dcheck() && dflag >= 2))
    {
	needobj();
	return;
    }
    object = anobj;
    vtake();
    return;
}

/*
        OPEN, LOCK, UNLOCK
*/
VOID PASCAL ivopen(VOID)
{
    if (here(CLAM))
	object = CLAM;
    if (here(OYSTER))
	object = OYSTER;
    if (at(DOOR))
	object = DOOR;
    if (at(GRATE))
	object = GRATE;
    if (here(CHAIN))
    {
	if (object != 0)
	{
	    needobj();
	    return;
	}
	object = CHAIN;
    }
    if (object == 0)
    {
	rspeak(28);
	return;
    }
    vopen();
    return;
}

/*
        ATTACK, KILL etc
*/
VOID PASCAL ivkill(VOID)
{
    object1 = 0;
    if (dcheck() && dflag >= 2)
	object = DWARF;
    if (here(SNAKE))
	addobj(SNAKE);
    if (at(DRAGON) && prop[DRAGON] == 0)
	addobj(DRAGON);
    if (at(TROLL))
	addobj(TROLL);
    if (here(BEAR) && prop[BEAR] == 0)
	addobj(BEAR);
    if (object1 != 0)
    {
	needobj();
	return;
    }
    if (object != 0)
    {
	vkill();
	return;
    }
    if (here(BIRD) && verb != THROW)
	object = BIRD;
    if (here(CLAM) || here(OYSTER))
	addobj(CLAM);
    if (object1 != 0)
    {
	needobj();
	return;
    }
    vkill();
    return;
}

/*
        EAT
*/
VOID PASCAL iveat(VOID)
{
    if(!here(FOOD))
       needobj();
    else
    {
	object=FOOD;
	veat();
    }

    return;
}

/*
        DRINK
*/
VOID PASCAL ivdrink(VOID)
{
    if (liqloc(loc) != WATER && (liq() != WATER || !here(BOTTLE)))
	needobj();
    else
    {
	object = WATER;
	vdrink();
    }

    return;
}

/*
        QUIT
*/
VOID PASCAL ivquit(VOID)
{
    if (gaveup = yes(22, 54, 54))
	normend();

    return;
}

/*
        FILL
*/
VOID PASCAL ivfill(VOID)
{
    if (!here(BOTTLE))
	needobj();
    else
    {
	object=BOTTLE;
	vfill();
    }

    return;
}

/*
        Handle fee fie foe foo...
*/
VOID PASCAL ivfoo(VOID)
{
    auto     SHORT	 k, msg;

    k	= vocab(word1, 3000);
    msg = 42;
    if (foobar != 1 - k)
    {
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
	(toting(EGGS) && loc == 92))
    {
	rspeak(msg);
	return;
    }

    if (place[EGGS] == 0 && place[TROLL] == 0 && prop[TROLL] == 0)
	prop[TROLL] = 1;

    if (here(EGGS))
	k = 1;
    else
    {
	if (loc == 92)
	    k = 0;
	else
	    k = 2;
    }
    move(EGGS, 92);
    pspeak(EGGS, k);
    return;
}

/*
        read etc...
*/
VOID PASCAL ivread(VOID)
{
    if (here(MAGAZINE))
	object = MAGAZINE;
    if (here(TABLET))
	object = object * 100 + TABLET;
    if (here(MESSAGE))
	object = object*100 + MESSAGE;
    if (object > 100 || object == 0 || dark())
    {
	needobj();
	return;
    }
    vread();
    return;
}

/*
        INVENTORY
*/
VOID PASCAL inventory(VOID)
{
    auto     SHORT	   msg;
    auto     SHORT	   i;

    msg = 98;
    for (i = 1; i <= MAXOBJ; ++i)
    {
	if (i == BEAR || !toting(i))
	    continue;
	if (msg)
	    rspeak(99);
	msg = 0;
	pspeak(i, -1);
    }
    if (toting(BEAR))
	msg = 141;
    if (msg)
	rspeak(msg);

    return;
}

/*
        ensure uniqueness as objects are searched
        out for an intransitive verb
*/
VOID PASCAL addobj(SHORT obj)
{
    if (object1 != 0)
	return;

    if (object != 0)
    {
	object1 = -1;
	return;
    }
    object = obj;
    return;
}

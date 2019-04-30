/*      program ITVERB.C                                        */


#include        <stdio.h>
#include        "advent.h"
#include        "advep.h"
#include        "advdec.h"

/*
        Routines to process intransitive verbs
*/
void itverb(void)
{
    switch (verb) {
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
    case FEE:
    case FIE:
    case FOE:
    case FOO:
    case FUM:
        ivfoo();
        break;
    case SUSPEND:
        saveflg = 1;
        break;
    case READ:
        ivread();
        break;
    case INVENTORY:
        invent();
        break;
    case BRIEF:
        ivbrief();
        break;
    default:
        printf("This intransitive not implemented yet\n");
    }
}

/*
        CARRY, TAKE etc.
*/
void ivtake(void)
{
    int    anobj, item;

    anobj = 0;
    for (item = 1; item < MAXOBJ; ++item) {
        if (place[item] == loc) {
            if (anobj != 0) {
                needobj();
                return;
            }
            anobj = item;
        }
    }
    if (anobj == 0 || (dcheck() && dflag >= 2)) {
        needobj();
        return;
    }
    object = anobj;
    vtake();
}

/*
        OPEN, LOCK, UNLOCK
*/
void ivopen(void)
{
    if (here(CLAM))
        object = CLAM;
    if (here(OYSTER))
        object = OYSTER;
    if (at(DOOR))
        object = DOOR;
    if (at(GRATE))
        object = GRATE;
    if (here(CHAIN)) {
        if (object != 0) {
            needobj();
            return;
        }
        object = CHAIN;
    }
    if (object == 0) {
        rspeak(28);
        return;
    }
    vopen();
}

/*
        ATTACK, KILL etc
*/
void ivkill(void)
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
    if (object1 != 0) {
        needobj();
        return;
    }
    if (object != 0) {
        vkill();
        return;
    }
    if (here(BIRD) && verb != THROW)
        object = BIRD;
    if (here(CLAM) || here(OYSTER))
        addobj(CLAM);
    if (object1 != 0) {
        needobj();
        return;
    }
    vkill();
}

/*
        EAT
*/
void iveat(void)
{
    if (!here(FOOD))
        needobj();
    else {
        object = FOOD;
        veat();
    }
}

/*
        DRINK
*/
void ivdrink(void)
{
    if (liqloc(loc) != WATER &&
        (liq() != WATER || !here(BOTTLE)))
        needobj();
    else {
        object = WATER;
        vdrink();
    }
}

/*
        QUIT
*/
void ivquit(void)
{
    if (gaveup = yes(22, 0, 54))
        normend();
}

/*
        FILL
*/
void ivfill(void)
{
    if (!here(BOTTLE))
        needobj();
    else {
        object = BOTTLE;
        vfill();
    }
}

/*
        Handle fee fie foe foo...
*/
void ivfoo(void)
{
    int    k, msg;
    k = verb - FEE + 1;
    msg = 42;
    if (foobar != 1 - k) {
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
        (toting(EGGS) && loc == 92)) {
        rspeak(msg);
        return;
    }
    if (place[EGGS] == 0 && place[TROLL] == 0 &&
        prop[TROLL] == 0)
        prop[TROLL] = 1;
    if (here(EGGS))
        k = 1;
    else if (loc == 92)
        k = 0;
    else
        k = 2;
    move(EGGS, 92);
    pspeak(EGGS, k, 0);
    return;
}

/*
        read etc...
*/
void ivread(void)
{
    if (here(MAGAZINE))
        object = MAGAZINE;
    if (here(TABLET))
        object = object * 100 + TABLET;
    if (here(MESSAGE))
        object = object * 100 + MESSAGE;
    if (object > 100 || object == 0 || dark()) {
        needobj();
        return;
    }
    vread();
}

/*
        INVENTORY
*/
void invent(void)
{
    int    msg, i;

    msg = 98;
    for (i = 1; i <= MAXOBJ; ++i) {
        if (i == BEAR || !toting(i))
            continue;
        if (msg)
            rspeak(99);
        msg = 0;
        pspeak(i, -1, 0);
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
    if (object1 != 0)
        return;
    if (object != 0) {
        object1 = -1;
        return;
    }
    object = obj;
}
/*
        turn on brief switch
*/
void ivbrief(void)
{
    detail |= 2;
    actspk(verb);
}

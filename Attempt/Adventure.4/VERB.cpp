
/*	program VERB.C						*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h".		*/


#include <strsafe.h>
#include	"stdio.h"	/* drv = 1.1st file 2.def 3.A	*/
#include	"advent.h"
#include	"advdec.h"

#include "verb.h"
#include "database.h"
#include "english.h"
#include "turn.h"
#include "itverb.h"


/*
    Routine to process a transitive verb
    A Transitive Verb must have a direct object to receive the action it describes
*/
void trverb()
{
    switch (g_iVerb)
    {
    case CALM:
    case WALK:
    case QUIT:
    case SCORE:
    case FOO:
    case BRIEF:
    case SUSPEND:
    case HOURS:
    case LOG:
        actspk(g_iVerb);
        break;
    case TAKE:
        vtake();
        break;
    case DROP:
        vdrop();
        break;
    case OPEN:
    case LOCK:
        vopen();
        break;
    case SAY:
        vsay();
        break;
    case NOTHING:
        rspeak(54);
        break;
    case ON:
        von();
        break;
    case OFF:
        voff();
        break;
    case WAVE:
        vwave();
        break;
    case KILL:
        vkill();
        break;
    case POUR:
        vpour();
        break;
    case EAT:
        veat();
        break;
    case DRINK:
        vdrink();
        break;
    case RUB:
        if (g_iObject != LAMP)
            rspeak(76);
        else
            actspk(RUB);
        break;
    case THROW:
        vthrow();
        break;
    case FEED:
        vfeed();
        break;
    case FIND:
    case INVENTORY:
        vfind();
        break;
    case FILL:
        vfill();
        break;
    case READ:
        vread();
        break;
    case BLAST:
        vblast();
        break;
    case BREAK:
        vbreak();
        break;
    case WAKE:
        vwake();
        break;
    default:
        printf("This verb is not implemented yet.\n");
    }
}

/*
    CARRY TAKE etc.
*/
void vtake()
{
    int msg;
    int i;

    if (toting(g_iObject))
    {
        actspk(g_iVerb);
        return;
    }
    /*
       special case objects and fixed objects
    */
    msg = 25;
    if (g_iObject == PLANT && prop[PLANT] <= 0)
        msg = 115;
    if (g_iObject == BEAR && prop[BEAR] == 1)
        msg = 169;
    if (g_iObject == CHAIN && prop[BEAR] != 0)
        msg = 170;
    if (fixed[g_iObject])
    {
        rspeak(msg);
        return;
    }
    /*
       special case for liquids
    */
    if (g_iObject == WATER || g_iObject == OIL)
    {
        if (!here(BOTTLE) || liq() != g_iObject)
        {
            g_iObject = BOTTLE;
            if (toting(BOTTLE)&&prop[BOTTLE] == 1)
            {
                vfill();
                return;
            }
            if (prop[BOTTLE] != 1)
                msg = 105;
            if (!toting(BOTTLE))
                msg = 104;
            rspeak(msg);
            return;
        }
        g_iObject = BOTTLE;
    }
    if (holding >= 7)
    {
        rspeak(92);
        return;
    }
    /*
       special case for bird.
    */
    if (g_iObject == BIRD && prop[BIRD] == 0)
    {
        if (toting(ROD))
        {
            rspeak(26);
            return;
        }
        if (!toting(CAGE))
        {
            rspeak(27);
            return;
        }
        prop[BIRD] = 1;
    }
    if ( (g_iObject == BIRD || g_iObject == CAGE) &&
         prop[BIRD] != 0)
        carry((BIRD+CAGE)-g_iObject,g_iLoc);
    carry(g_iObject,g_iLoc);
    /*
       handle liquid in bottle
    */
    i = liq();
    if (g_iObject == BOTTLE && i != 0)
        place[i] = -1;
    rspeak(54);
}

/*
    DROP etc.
*/
void vdrop()
{
    int i;

    /*
       check for dynamite
    */
    if (toting(ROD2) && g_iObject == ROD && !toting(ROD))
        g_iObject = ROD2;
    if (!toting(g_iObject))
    {
        actspk(g_iVerb);
        return;
    }
    /*
       snake and bird
    */
    if (g_iObject == BIRD && here(SNAKE))
    {
        rspeak(30);
        if (closed)
            dwarfend();
        dstroy(SNAKE);
        prop[SNAKE] = -1;
    }
    /*
       coins and vending machine
    */
    else if (g_iObject == COINS && here(VEND))
    {
        dstroy(COINS);
        drop(BATTERIES,g_iLoc);
        pspeak(BATTERIES,0);
        return;
    }
    /*
       bird and dragon (ouch!!)
    */
    else if (g_iObject == BIRD && at(DRAGON) && prop[DRAGON] == 0)
    {
        rspeak(154);
        dstroy(BIRD);
        prop[BIRD] = 0;
        if (place[SNAKE] != 0)
            ++tally2;
        return;
    }
    /*
       Bear and troll
    */
    if (g_iObject == BEAR && at(TROLL))
    {
        rspeak(163);
        move(TROLL,0);
        move((TROLL+MAXOBJ),0);
        move(TROLL2,117);
        move((TROLL2+MAXOBJ),122);
        juggle(CHASM);
        prop[TROLL] = 2;
    }
    /*
       vase
    */
    else if (g_iObject == VASE)
    {
        if (g_iLoc == 96)
            rspeak(54);
        else
        {
            prop[VASE] = at(PILLOW) ? 0 : 2;
            pspeak(VASE,prop[VASE]+1);
            if (prop[VASE] != 0)
                fixed[VASE] = -1;
        }
    }
    /*
       handle liquid and bottle
    */
    i = liq();
    if (i == g_iObject)
        g_iObject = BOTTLE;
    if (g_iObject == BOTTLE && i != 0)
        place[i] = 0;
    /*
       handle bird and cage
    */
    if (g_iObject == CAGE && prop[BIRD] != 0)
        drop(BIRD,g_iLoc);
    if (g_iObject == BIRD)
        prop[BIRD] = 0;
    drop(g_iObject,g_iLoc);
}

/*
    LOCK, UNLOCK, OPEN, CLOSE etc.
*/
void vopen()
{
    char oyclam;
    int msg;

    switch (g_iObject)
    {
    case CLAM:
    case OYSTER:
        oyclam = (g_iObject == OYSTER ? 1 : 0);
        if (g_iVerb == LOCK)
            msg = 61;
        else if (!toting(TRIDENT))
            msg = 122+oyclam;
        else if (toting(g_iObject))
            msg = 120+oyclam;
        else
        {
            msg = 124+oyclam;
            dstroy(CLAM);
            drop(OYSTER,g_iLoc);
            drop(PEARL,105);
        }
        break;
    case DOOR:
        msg = (prop[DOOR] == 1 ? 54 : 111);
        break;
    case CAGE:
        msg = 32;
        break;
    case KEYS:
        msg = 55;
        break;
    case CHAIN:
        if (!here(KEYS))
            msg = 31;
        else if (g_iVerb == LOCK)
        {
            if (prop[CHAIN] != 0)
                msg = 34;
            else if (g_iLoc != 130)
                msg = 173;
            else
            {
                prop[CHAIN] = 2;
                if (toting(CHAIN))
                    drop(CHAIN,g_iLoc);
                fixed[CHAIN] =  -1;
                msg = 172;
            }
        }
        else
        {
            if (prop[BEAR] == 0)
                msg = 41;
            else if (prop[CHAIN] == 0)
                msg = 37;
            else
            {
                prop[CHAIN] = 0;
                fixed[CHAIN] = 0;
                if (prop[BEAR] != 3)
                    prop[BEAR] = 2;
                fixed[BEAR] = 2-prop[BEAR];
                msg = 171;
            }
        }
        break;
    case GRATE:
        if (!here(KEYS))
            msg = 31;
        else if (closing)
        {
            if (!panic)
            {
                clock2 = 15;
                ++panic;
            }
            msg = 130;
        }
        else
        {
            msg = 34+prop[GRATE];
            prop[GRATE] = (g_iVerb == LOCK ? 0 : 1);
            msg += 2*prop[GRATE];
        }
        break;
    default:
        msg = 33;
    }
    rspeak(msg);
}

/*
    SAY etc.
*/
void vsay()
{
    int wtype,wval;

    analyze(word1,&wtype,&wval);
    printf("Okay.\n%s\n",wval == SAY ? word2 : word1);
}

/*
    ON etc.
*/
void von()
{
    if (!here(LAMP))
        actspk(g_iVerb);
    else if (limit<0)
        rspeak(184);
    else
    {
        prop[LAMP] = 1;
        rspeak(39);
        if (wzdark)
        {
            wzdark = 0;
            describe();
        }
    }
}

/*
    OFF etc.
*/
void voff()
{
    if (!here(LAMP))
        actspk(g_iVerb);
    else
    {
        prop[LAMP] = 0;
        rspeak(40);
    }
}

/*
    WAVE etc.
*/
void vwave()
{
    if (!toting(g_iObject) &&
        (g_iObject != ROD || !toting(ROD2)))
        rspeak(29);
    else if (g_iObject != ROD || !at(FISSURE) ||
             !toting(g_iObject) || closing)
        actspk(g_iVerb);
    else
    {
        prop[FISSURE] = 1-prop[FISSURE];
        pspeak(FISSURE,2-prop[FISSURE]);
    }
}

/*
    ATTACK, KILL etc.
*/
void vkill()
{
    int msg;
    int i;

    switch (g_iObject)
    {
    case BIRD:
        if (closed)
            msg = 137;
        else
        {
            dstroy(BIRD);
            prop[BIRD] = 0;
            if (place[SNAKE] == 19)
                ++tally2;
            msg = 45;
        }
        break;
    case 0:
        msg = 44;
        break;
    case CLAM:
    case OYSTER:
        msg = 150;
        break;
    case SNAKE:
        msg = 46;
        break;
    case DWARF:
        if (closed)
            dwarfend();
        msg = 49;
        break;
    case TROLL:
        msg = 157;
        break;
    case BEAR:
        msg = 165+(prop[BEAR]+1)/2;
        break;
    case DRAGON:
        if (prop[DRAGON] != 0)
        {
            msg = 167;
            break;
        }
        if (!yes(49,0,0))
            break;
        pspeak(DRAGON,1);
        prop[DRAGON] = 2;
        prop[RUG] = 0;
        move((DRAGON+MAXOBJ),-1);
        move((RUG+MAXOBJ),0);
        move(DRAGON,120);
        move(RUG,120);
        for (i = 1;i<MAXOBJ;++i)
            if (place[i] == 119 || place[i] == 121)
                move(i,120);
        newloc = 120;
        return;
    default:
        actspk(g_iVerb);
        return;
    }
    rspeak(msg);
}

/*
    POUR
*/
void vpour()
{
    if (g_iObject == BOTTLE || g_iObject == 0)
        g_iObject = liq();
    if (g_iObject == 0)
    {
        needobj();
        return;
    }
    if (!toting(g_iObject))
    {
        actspk(g_iVerb);
        return;
    }
    if (g_iObject != OIL && g_iObject != WATER)
    {
        rspeak(78);
        return;
    }
    prop[BOTTLE] = 1;
    place[g_iObject] = 0;
    if (at(PLANT))
    {
        if (g_iObject != WATER)
            rspeak(112);
        else
        {
            pspeak(PLANT,prop[PLANT]+1);
            prop[PLANT] = (prop[PLANT]+2)%6;
            prop[PLANT2] = prop[PLANT]/2;
            describe();
        }
    }
    else if (at(DOOR))
    {
        prop[DOOR] = (g_iObject == OIL ? 1 : 0);
        rspeak(113+prop[DOOR]);
    }
    else
        rspeak(77);
}

/*
    EAT
*/
void veat()
{
    int msg;

    switch (g_iObject)
    {
    case FOOD:
        dstroy(FOOD);
        msg = 72;
        break;
    case BIRD:
    case SNAKE:
    case CLAM:
    case OYSTER:
    case DWARF:
    case DRAGON:
    case TROLL:
    case BEAR:
        msg = 71;
        break;
    default:
        actspk(g_iVerb);
        return;
    }
    rspeak(msg);
}

/*
    DRINK
*/
void vdrink()
{
    if (g_iObject != WATER)
        rspeak(110);
    else if (liq() != WATER || !here(BOTTLE))
        actspk(g_iVerb);
    else
    {
        prop[BOTTLE] = 1;
        place[WATER] = 0;
        rspeak(74);
    }
}

/*
    THROW etc.
*/
void vthrow()
{
    int msg;
    int i;

    if (toting(ROD2) && g_iObject == ROD && !toting(ROD))
        g_iObject = ROD2;
    if (!toting(g_iObject))
    {
        actspk(g_iVerb);
        return;
    }
    /*
       treasure to troll
    */
    if (at(TROLL) && g_iObject >= 50 && g_iObject<MAXOBJ)
    {
        rspeak(159);
        drop(g_iObject,0);
        move(TROLL,0);
        move((TROLL+MAXOBJ),0);
        drop(TROLL2,117);
        drop((TROLL2+MAXOBJ),122);
        juggle(CHASM);
        return;
    }
    /*
       feed the bears...
    */
    if (g_iObject == FOOD && here(BEAR))
    {
        g_iObject = BEAR;
        vfeed();
        return;
    }
    /*
       if not axe, same as drop...
    */
    if (g_iObject != AXE)
    {
        vdrop();
        return;
    }
    /*
       AXE is THROWN
    */
    /*
       at a dwarf...
    */
    if (i = dcheck())
    {
        msg = 48;
        if (pct(33))
        {
            dseen[i] = dloc[i] = 0;
            msg = 47;
            ++dkill;
            if (dkill == 1)
                msg = 149;
        }
    }
    /*
       at a dragon...
    */
    else if (at(DRAGON) && prop[DRAGON] == 0)
        msg = 152;
    /*
       at the troll...
    */
    else if (at(TROLL))
        msg = 158;
    /*
       at the bear...
    */
    else if (here(BEAR) && prop[BEAR] == 0)
    {
        rspeak(164);
        drop(AXE,g_iLoc);
        fixed[AXE] = -1;
        prop[AXE] = 1;
        juggle(BEAR);
        return;
    }
    /*
       otherwise it is an attack
    */
    else
    {
        g_iVerb = KILL;
        g_iObject = 0;
        itverb();
        return;
    }
    /*
       handle the left over axe...
    */
    rspeak(msg);
    drop(AXE,g_iLoc);
    describe();
}

/*
    INVENTORY, FIND etc.
*/
void vfind()
{
    int msg;
    if (toting(g_iObject))
        msg = 24;
    else if (closed)
        msg = 138;
    else if (dcheck() && dflag >= 2 && g_iObject == DWARF)
        msg = 94;
    else if (at(g_iObject) ||
             (liq() == g_iObject && here(BOTTLE)) ||
             g_iObject == liqloc(g_iLoc))
        msg = 94;
    else
    {
        actspk(g_iVerb);
        return;
    }
    rspeak(msg);
}

/*
    FILL
*/
void vfill()
{
    int msg;
    int i;

    switch (g_iObject)
    {
    case BOTTLE:
        if (liq() != 0)
            msg = 105;
        else if (liqloc(g_iLoc) == 0)
            msg = 106;
        else
        {
            prop[BOTTLE] = cond[g_iLoc] & WATOIL;
            i = liq();
            if (toting(BOTTLE))
                place[i] = -1;
            msg = (i == OIL ? 108 : 107);
        }
        break;
    case VASE:
        if (liqloc(g_iLoc) == 0)
        {
            msg = 144;
            break;
        }
        if (!toting(VASE))
        {
            msg = 29;
            break;
        }
        rspeak(145);
        vdrop();
        return;
    default:
        msg = 29;
    }
    rspeak(msg);
}

/*
    FEED
*/
void vfeed()
{
    int msg;

    switch (g_iObject)
    {
    case BIRD:
        msg = 100;
        break;
    case DWARF:
        if (!here(FOOD))
        {
            actspk(g_iVerb);
            return;
        }
        ++dflag;
        msg = 103;
        break;
    case BEAR:
        if (!here(FOOD))
        {
            if (prop[BEAR] == 0)
                msg = 102;
            else if (prop[BEAR] == 3)
                msg = 110;
            else
            {
                actspk(g_iVerb);
                return;
            }
            break;
        }
        dstroy(FOOD);
        prop[BEAR] = 1;
        fixed[AXE] = 0;
        prop[AXE] = 0;
        msg = 168;
        break;
    case DRAGON:
        msg = (prop[DRAGON] != 0 ? 110 : 102);
        break;
    case TROLL:
        msg = 182;
        break;
    case SNAKE:
        if (closed || !here(BIRD))
        {
            msg = 102;
            break;
        }
        msg = 101;
        dstroy(BIRD);
        prop[BIRD] = 0;
        ++tally2;
        break;
    default:
        msg = 14;
    }
    rspeak(msg);
}

/*
    READ etc.
*/
void vread()
{
    int msg;

    msg = 0;
    if (dark())
    {
        printf("I see no %s here.\n",probj());
        return;
    }
    switch (g_iObject)
    {
    case MAGAZINE:
        msg = 190;
        break;
    case TABLET:
        msg = 196;
        break;
    case MESSAGE:
        msg = 191;
        break;
    case OYSTER:
        if (!toting(OYSTER) || !closed)
            break;
        yes(192,193,54);
        return;
    default:
        ;
    }
    if (msg)
        rspeak(msg);
    else
        actspk(g_iVerb);
}

/*
    BLAST etc.
*/
void vblast()
{
    if (prop[ROD2] < 0 || !closed)
        actspk(g_iVerb);
    else
    {
        bonus = 133;
        if (g_iLoc == 115)
            bonus = 134;
        if (here(ROD2))
            bonus = 135;
        rspeak(bonus);
        normend();
    }
}

/*
    BREAK etc.
*/
void vbreak()
{
    int msg;
    if (g_iObject == MIRROR)
    {
        msg = 148;
        if (closed)
        {
            rspeak(197);
            dwarfend();
        }
    }
    else if (g_iObject == VASE && prop[VASE] == 0)
    {
        msg = 198;
        if (toting(VASE))
            drop(VASE,g_iLoc);
        prop[VASE] = 2;
        fixed[VASE] = -1;
    }
    else
    {
        actspk(g_iVerb);
        return;
    }
    rspeak(msg);
}

/*
    WAKE etc.
*/
void vwake()
{
    if (g_iObject != DWARF || !closed)
        actspk(g_iVerb);
    else
    {
        rspeak(199);
        dwarfend();
    }
}

/*
    Routine to speak default verb message
*/
void actspk(int verb)
{
    char i;

    if (verb<1 || verb>31)
        bug(39);
    i = actmsg[verb];
    if (i)
        rspeak(i);
}

/*
    Routine to indicate no reasonable
    object for verb found.  Used mostly by
    intransitive verbs.
*/
void needobj()
{
    int wtype,wval;

    analyze(word1,&wtype,&wval);
    printf("%s what?\n",wtype == 2 ? word1 : word2);
}


/*      program TURN.C                                          */


#include        <stdio.h>
#include        <stdlib.h>
#include        <stdbool.h>
#include        <string.h>
#include        "advent.h"
#include        "advep.h"
#include        "advdec.h"

int     totalscore;

/*
    turn
    ----
    Processes a single turn in the game. Handles player movement, checks for special conditions (such as closing the cave, dwarf encounters, forced moves, death, hints), updates game state, and executes player commands.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Updates player location, state, and turn counters
        - May call functions that print messages, update the world, or end the game
        - Handles hints, death, and forced moves
*/
void turn(void)
{
    int    i;
    /*
            if closing, then he can't leave except via
            the main office.
    */
    if (newloc < 9 && newloc != 0 && closing) {
        rspeak(130);
        newloc = loc;
        if (!panic)
            clock2 = 15;
        panic = 1;
    }
    /*
            see if a dwarf has seen him and has come
            from where he wants to go.
    */
    if (newloc != loc && !forced(loc) && (cond[loc] & NOPIRAT) == 0)
        for (i = 1; i < (DWARFMAX - 1); ++i)
            if (odloc[i] == newloc && dseen[i]) {
                newloc = loc;
                rspeak(2);
                break;
            }

    dwarves();      /* & special dwarf(pirate who steals)   */

    /* added by BDS C conversion */
    if (loc != newloc) {
        if (loc == 0) visited[newloc] = (visited[newloc] + 3) & ~3;
        ++turns;
        loc = newloc;

        /* check for death */
        if (loc == 0) {
            death();
            return;
        }

        /* check for forced move */
        if (forced(loc)) {
            describe();
            domove();
            return;
        }

        /* check for wandering in dark */
        if (wzdark && dark() && pct(35)) {
            rspeak(23);
            oldloc2 = loc;
            death();
            return;
        }

        /* describe his situation */
        describe();
        if (!dark())
            ++visited[loc];
    }                               /* if (loc != newloc)   */

    if (closed) {
        if (prop[OYSTER] < 0 && toting(OYSTER))
            pspeak(OYSTER, 1, 0);
        for (i = 1; i <= MAXOBJ; ++i)
            if (toting(i) && prop[i] < 0)
                prop[i] = -1 - prop[i];
    }

    wzdark = dark();
    if (knfloc > 0 && knfloc != loc)
        knfloc = 0;

    if (stimer())   /* as the grains of sand slip by        */
        return;

    if (cond[loc] & hintavail)
        switch (cond[loc] & HINT) {
        case HINTF:
            ++hintloc[HINTAREAF];
            if (hintloc[HINTAREAF] > 20 && visited[8] == 0 /* never in yet */)
                tryhint(56, HINTF, HINTAREAF);
            break;
        case HINTC:
            ++hintloc[HINTAREAC];
            if (hintloc[HINTAREAC] > 3 && prop[GRATE] == 0 && !toting(KEYS))
                tryhint(62, HINTC, HINTAREAC);
            break;
        case HINTB:
            ++hintloc[HINTAREAB];
            if (hintloc[HINTAREAB] > 4 && place[BIRD] == loc && toting(ROD))
                tryhint(18, HINTB, HINTAREAB);
            break;
        case HINTS:
            ++hintloc[HINTAREAS];
            if (hintloc[HINTAREAS] > 5 && place[SNAKE] == loc && !toting(BIRD))
                tryhint(20, HINTS, HINTAREAS);
            break;
        case HINTM:
            ++hintloc[HINTAREAM];
            if (hintloc[HINTAREAM] > 15)
                tryhint(176, HINTM, HINTAREAM);
            break;
        case HINTP:
            ++hintloc[HINTAREAP];
            if (hintloc[HINTAREAP] > 5 && place[EMERALD] != 100)
                tryhint(178, HINTP, HINTAREAP);
            break;
        case HINTW:
            ++hintloc[HINTAREAW];
            if (hintloc[HINTAREAW] > 15)
                tryhint(180, HINTW, HINTAREAW);
        }
    else
    {
        memset(hintloc, 0, sizeof(hintloc[0]) * (MAXHINT + 1));
    }


    while (!english())      /* retrieve player instructions */
        ;

#ifdef DEBUG
    if (dbugflg)
        printf("loc = %d, verb = %d, object = %d, motion = %d\n",
            loc, verb, object, motion);
#endif

    if (motion)             /* execute player instructions  */
        domove();
    else if (object)
        doobj();
    else
        itverb();
}

/*
describe - Routine to describe current location
--------
Describes the current location to the player. This function:
  - Prints a special message if the player is carrying the bear.
  - Prints a message if the location is dark.
  - Otherwise, prints either the short or long description of the location:
      * If the location has been visited before, or if the player has requested more detail, prints the short description (descsh).
      * Otherwise, prints the long description (desclg).
  - If the location is not dark, also describes any visible items (descitem).
  - Occasionally prints a random message in location 33 if the cave is not closing.

    Parameters:
        None (uses global state for location and inventory)

    Side Effects:
        - Prints messages to the player
        - May update visited locations
*/
void describe(void)
{
    if (toting(BEAR))
        rspeak(141);
    if (dark())
        rspeak(16);
    else {
        if ((visited[loc] & 3) || ((detail & testbr) && visited[loc]))
            descsh(loc);
        else
            desclg(loc);
        if (!dark())
            descitem();
    }
    if (loc == 33 && pct(25) && !closing)
        rspeak(8);
}

/*
    descitem
    --------
    Describes visible items at the current location. Handles special cases for certain items and updates tally if items are revealed.

    Parameters:
        None (uses global state for location and items)

    Side Effects:
        - Prints item descriptions
        - Updates tally and item properties
*/
void descitem(void)
{
    int    i, init = 0;
    int             state, prespace;

    for (i = 1; i < MAXOBJ; ++i) {
        if (at(i)) {
            if (i == STEPS && toting(NUGGET))
                continue;
            if (prop[i] < 0) {
                if (closed)
                    continue;
                else {
                    prop[i] = 0;
                    if (i == RUG || i == CHAIN)
                        ++prop[i];
                    --tally;
                }
            }
            if (i == STEPS && loc == fixed[STEPS])
                state = 1;
            else
                state = prop[i];
            if (!init && !(detail & 2)) {
                init = 1;
                prespace = 1;
            }
            else
                prespace = 0;
            pspeak(i, state, prespace);
        }
    }
    if (tally == tally2 && tally != 0 && limit > 35)
        limit = 35;
}

/*
    domove
    ------
    Handles the player's motion request. Calls gettrav to fill the travel array, then processes the motion command (e.g., BACK, LOOK, CAVE, or a regular move).

    Parameters:
        None (uses and updates global state)

    Side Effects:
        - May update player location, print messages, or call other movement functions
*/
void domove(void)
{
    gettrav(loc);
    switch (motion) {
    case NULLX:
        break;
    case BACK:
        goback();
        break;
    case LOOK:
        if (!detail) {
            rspeak(15);
            detail |= 1;
        }
        wzdark = 0;
        visited[loc] = (visited[loc] + 3) & ~3;
        testbr = 0;
        newloc = loc;
        loc = 0;
        break;
    case CAVE:
        if (loc < 8)
            rspeak(57);
        else
            rspeak(58);
        break;
    default:
        oldloc2 = oldloc;
        oldloc = loc;
        dotrav();
    }
}

/*
    goback
    ------
    Handles the player's request to return to the previous location. Searches the travel array for a valid move back, and updates motion and location accordingly.

    Parameters:
        None (uses and updates global state)

    Side Effects:
        - May update player location, print messages, or call dotrav
*/
void goback(void)
{
    int    kk, k2;
    int             want, temp;
    struct trav strav[MAXTRAV];

    if (forced(oldloc))
        want = oldloc2;
    else
        want = oldloc;
    oldloc2 = oldloc;
    oldloc = loc;
    k2 = 0;
    if (want == loc) {
        rspeak(91);
        return;
    }
    copytrv(travel, strav);
    for (kk = 0; travel[kk].tdest != -1; ++kk) {
        if (!travel[kk].tcond && travel[kk].tdest == want) {
            motion = travel[kk].tverb;
            dotrav();
            return;
        }
        if (!travel[kk].tcond) {
            k2 = kk;
            temp = travel[kk].tdest;
            if (temp <= MAXLOC) {
                gettrav(temp);
                if (forced(temp) && travel[0].tdest == want)
                    k2 = temp;
                copytrv(strav, travel);
            }
            else
                k2 = 0;
        }
    }
    if ((k2 > 0) && (k2 < MAXTRAV))
    {
        motion = travel[k2].tverb;
        dotrav();
    }
    else
        rspeak(140);
}

/*
    copytrv
    -------
    Copies one travel array to another.

    Parameters:
        trav1 - Source travel array
        trav2 - Destination travel array

    Side Effects:
        - Modifies the destination travel array
*/
void copytrv(struct trav* trav1, struct trav* trav2)
{
    int    i;

    for (i = 0; i < MAXTRAV; ++i)
        trav2 = trav1;
}

/*
    dotrav
    ------
    Determines the new player location based on the current location and the player's intended motion.
    This function examines the global 'travel' array (which should be filled for the current location)
    and checks each possible travel option to see if the player's motion matches and if any conditions are met.
    If a valid move is found, updates 'newloc' to the destination, or triggers special movement or messages.
    If no valid move is found, calls badmove() to handle the failed attempt.

    Parameters:
        None (uses and updates global variables such as loc, newloc, motion, travel, etc.)

    Side Effects:
        - Updates 'newloc' to the new location if a valid move is found.
        - May call rspeak(), spcmove(), or badmove() to handle special cases or errors.
        - May update other global state depending on the move.
*/
void dotrav(void)
{
    int    kk, hitflag;
    int             mvflag;
    int             rdest, rverb, rcond, robject;
    int             pctt;

    newloc = loc;
    mvflag = hitflag = 0;
    pctt = rrand(0, 99);

    for (kk = 0; travel[kk].tdest >= 0 && !mvflag; ++kk) {
        rdest = travel[kk].tdest;
        rverb = travel[kk].tverb;
        rcond = travel[kk].tcond;
        robject = rcond % 100;

#ifdef DEBUG
        if (dbugflg)
            printf("rdest = %d, rverb = %d, rcond = %d, robject = %d in dotrav\n",
                rdest, rverb, \
                rcond, robject);
#endif
        if ((rverb != 1) && (rverb != motion) && !hitflag)
            continue;
        ++hitflag;
        switch (rcond / 100) {
        case 0:
            if ((rcond == 0) || (pctt < rcond))
                ++mvflag;
#ifdef DEBUG
            if (rcond && dbugflg)
                printf("%% move %d %d\n",
                    pctt, mvflag);
#endif
            break;
        case 1:
            if (robject == 0)
                ++mvflag;
            else if (toting(robject))
                ++mvflag;
            break;
        case 2:
            if (toting(robject) || at(robject))
                ++mvflag;
            break;
        case 3:
        case 4:
        case 5:
        case 7:
            if (prop[robject] != (rcond / 100) - 3)
                ++mvflag;
            break;
        default:
            bug(37);
        }
    }
    if (!mvflag)
        badmove();
    else if (rdest > 500)
        rspeak(rdest - 500);
    else if (rdest > 300)
        spcmove(rdest);
    else {
        newloc = rdest;
#ifdef DEBUG
        if (dbugflg)
            printf("newloc in dotrav = %d\n", newloc);
#endif
        if (newloc == loc) loc = 0;
    }
}

/*
    badmove
    -------
    Handles the case where the player attempts an invalid move. Selects and prints an appropriate error message.

    Parameters:
        None (uses global state for motion and verb)

    Side Effects:
        - Prints an error message to the player
*/
void badmove(void)
{
    int    msg;

    msg = 12;
    if (motion >= 43 && motion <= 50) msg = 9;
    if (motion == 29 || motion == 30) msg = 9;
    if (motion == 7 || motion == 36 || motion == 37) msg = 10;
    if (motion == 11 || motion == 19) msg = 11;
    if (verb == FIND || verb == INVENTORY) msg = 59;
    if (motion == 62 || motion == 65) msg = 42;
    if (motion == 17) msg = 80;
    rspeak(msg);
}

/*
    spcmove
    -------
    Handles very special movement cases (e.g., plover movement, troll bridge). Updates location and game state as needed.

    Parameters:
        rdest - Encoded destination value for special movement

    Side Effects:
        - Updates newloc, may print messages, update inventory, or end the game
*/
void spcmove(int rdest)
{
    switch (rdest - 300) {
    case 1: /* plover movement via alcove */
        if (!holding || (holding == 1 && toting(EMERALD)))
            newloc = (99 + 100) - loc;
        else
            rspeak(117);
        break;
    case 2: /* trying to remove plover, bad route */
        drop(EMERALD, loc);
        rspeak(54);
        break;
    case 3: /* troll bridge */
        if (prop[TROLL] == 1) {
            pspeak(TROLL, 1, 0);
            prop[TROLL] = 0;
            move(TROLL2, 0);
            move((TROLL2 + MAXOBJ), 0);
            move(TROLL, 117);
            move((TROLL + MAXOBJ), 122);
            juggle(CHASM);
            newloc = loc;
        }
        else {
            newloc = (loc == 117 ? 122 : 117);
            if (prop[TROLL] == 0)
                ++prop[TROLL];
            if (!toting(BEAR))
                return;
            rspeak(162);
            prop[CHASM] = 1;
            prop[TROLL] = 2;
            drop(BEAR, newloc);
            fixed[BEAR] = -1;
            prop[BEAR] = 3;
            if (prop[SPICES] < 0)
                ++tally2;
            oldloc2 = newloc;
            death();
        }
        break;
    default:
        bug(38);
    }
}


/*
    dwarfend
    --------
    Handles the player's demise due to dwarves. Prints a message, calls death, and ends the game.

    Parameters:
        msg - Message number to print (if nonzero)

    Side Effects:
        - Prints a message, ends the game
*/
void dwarfend(int msg)
{
    if (msg)
        rspeak(msg);
    death();
    normend();
}

/*
    normend
    -------
    Handles the normal end of the game. Calculates and prints the player's score and rating, then exits.

    Parameters:
        None

    Side Effects:
        - Prints score and rating, exits the program
*/
void normend(void)
{
    static short int limits[] = { 35, 100, 200, 250, 300, 330, 350, 1000 };
    short int* i;
    int j;
    int total;
    total = score();
    for (i = limits, j = 0; *i <= total; i++, j++);
    putchar('\n');
    rspeak(202 + j);
    j = *i - total;
    printf("\nTo achieve the next higher rating, you need %d more point%s.\n",
        j, j > 1 ? "s" : "");
    exit(0);
}

/*
    score
    -----
    Calculates and prints the player's score, including treasures, survival, hints, and bonuses.

    Parameters:
        None

    Returns:
        The total score as an integer

    Side Effects:
        - Prints score breakdown to the player
*/
int score(void)
{
    int t, s;
    s = t = 0;
    {
        int i, k;
        for (i = 50; i <= MAXTRS; ++i) {
            if (i == CHEST)
                k = 14;
            else if (i > CHEST)
                k = 16;
            else
                k = 12;
            if (prop[i] >= 0)
                t += 2;
            if (place[i] == 3 && prop[i] == 0)
                t += k - 2;
        }
    }
    printf("%-22s%4d\n", "Treasures:", s = t);
    t = (MAXDIE - numdie) * 10;
    if (t)
        printf("%-22s%4d\n", "Survival:", t);
    s += t;
    if (!gaveup)
        s += 4;
    t = visited[19] ? 25 : 0;
    if (t)
        printf("%-22s%4d\n", "Getting well in:", t);
    s += t;
    t = closing ? 25 : 0;
    if (t)
        printf("%-22s%4d\n", "Masters section:", t);
    s += t;
    if (closed) {
        if (bonus == 0)
            t = 10;
        else if (bonus == 135)
            t = 25;
        else if (bonus == 134)
            t = 30;
        else if (bonus == 133)
            t = 45;
        printf("%-22s%4d\n", "Bonus:", t);
        s += t;
    }
    if (place[MAGAZINE] == 108)
        s += 1;
    t = -15 * hinttaken;
    if (t) {
        printf("%-22s%4d\n", "Hints & intructions:", t);
        s += t;
    }
    s += 2;
    if (s < 0) s = 0;
    printf("%-22s%4d\n", "Score:", s);
    return s;
}

/*
    death
    -----
    Handles the player's death. Asks if the player wants to continue, resets inventory and location if so, or ends the game if not.

    Parameters:
        None

    Side Effects:
        - Updates inventory, location, and game state; may end the game
*/
void death(void)
{
    int i, j;
    int yea;

    if (!closing) {
        yea = yes(81 + numdie * 2, 82 + numdie * 2, 54);
        if (++numdie >= MAXDIE || !yea)
            normend();
        place[WATER] = 0;
        place[OIL] = 0;
        if (toting(LAMP))
            prop[LAMP] = '\0';
        for (j = 1; j < 101; ++j) {
            i = (char)101 - j;
            if (toting(i))
                drop(i, i == LAMP ? 1 : oldloc2);
        }
        newloc = 3;
        oldloc = loc;
        loc = 0;
        return;
    }
    /*
     closing -- no resurrection...
    */
    rspeak(131);
    ++numdie;
    normend();
}

/*
    doobj
    -----
    Processes an object-related command. Handles special cases for certain objects and calls trobj for transitive actions.

    Parameters:
        None (uses global state for object and location)

    Side Effects:
        - May update inventory, print messages, or call trobj
*/
void doobj(void)
{
    /*
     is object here? if so, transitive
    */
    if (fixed[object] == loc || here(object))
        trobj();
    /*
            did he give grate as destination?
    */
    else if (object == GRATE) {
        if (loc == 1 || loc == 4 || loc == 7) {
            motion = DEPRESSION;
            domove();
        }
        else if (loc > 9 && loc < 15) {
            motion = ENTRANCE;
            domove();
        }
    }
    /*
            is it a dwarf he is after?
    */
    else if (dcheck() && dflag >= 2) {
        object = DWARF;
        trobj();
    }
    /*
     is he trying to get/use a liquid?
    */
    else if ((liq() == object && here(BOTTLE)) ||
        liqloc(loc) == object)
        trobj();
    else if (object == PLANT && at(PLANT2) &&
        prop[PLANT2] == 0) {
        object = PLANT2;
        trobj();
    }
    /*
     is he trying to grab a knife?
    */
    else if (object == KNIFE && knfloc == loc) {
        rspeak(116);
        knfloc = -1;
    }
    /*
     is he trying to get at dynamite?
    */
    else if (object == ROD && here(ROD2)) {
        object = ROD2;
        trobj();
    }
    /*
     is he trying to get at shadow troll?
    */
    else if (object == TROLL && here(TROLL2)) {
        object = TROLL2;
        trobj();
    }
    /*
     is he trying to get at shadow plant?
    */
    else if (object == PLANT && here(PLANT2)) {
        object = PLANT2;
        trobj();
    }
    else
        printf("I see no %s here.\n", probj(object));
}

/*
    trobj
    -----
    Processes an object being referred to. Calls trverb if a verb is present, or prompts the player for clarification.

    Parameters:
        None (uses global state for object and verb)

    Side Effects:
        - May print a prompt or call trverb
*/
void trobj(void)
{
    if (verb)
        trverb();
    else
        printf("What do you want to do with the %s?\n",
            probj(object));
}

/*
    probj
    -----
    Returns the word corresponding to the given object.

    Parameters:
        object - The object number

    Returns:
        Pointer to the object's word (char*)

    Side Effects:
        - None
*/
char* probj(int object)
{
    int     wtype, wval;
    analyze(word1, &wtype, &wval);
    return (wtype == 1 ? word1 : word2);
}
/*
        dwarf stuff.
*/
void dwarves(void)
{
    int    i, j;
    int             k, try, attack, stick, dtotal;
    /*
            see if dwarves allowed here
    */
    if (newloc == 0 || forced(newloc) || cond[newloc] & NOPIRAT)
        return;
    /*
            see if dwarves are active.
    */
    if (!dflag) {
        if (newloc > 15)
            ++dflag;
        return;
    }
    /*
            if first close encounter (of 3rd kind)
            kill 0, 1 or 2
    */
    if (dflag == 1) {
        if (newloc < 15 || pct(95))
            return;
        ++dflag;
        for (i = 1; i < 3; ++i)
            if (pct(50))
                dloc[rrand(1, 5)] = 0;
        for (i = 1; i < (DWARFMAX - 1); ++i) {
            if (dloc[i] == newloc)
                dloc[i] = daltloc;
            odloc[i] = dloc[i];
        }
        rspeak(3);
        drop(AXE, newloc);
        return;
    }
    dtotal = attack = stick = 0;
    for (i = 1; i < DWARFMAX; ++i) {
        if (dloc[i] == 0)
            continue;
        /*
                move a dwarf at random. we don't
                have a matrix around to do it
                as in the original version...
        */
        for (try = 1; try < 20; ++try) {
            j = rrand(15, 120); /* allowed area */
            if (j != odloc[i] && j != dloc[i] &&
                !(i == (DWARFMAX - 1) && (cond[j] & (NOPIRAT == 1))))
                break;
        }
        if (j == 0)
            j = odloc[i];
        odloc[i] = dloc[i];
        dloc[i] = j;
        if ((dseen[i] && newloc >= 15) ||
            dloc[i] == newloc || odloc[i] == newloc)
            dseen[i] = 1;
        else
            dseen[i] = 0;
        if (!dseen[i])
            continue;
        dloc[i] = newloc;
        if (i == 6)
            dopirate();
        else {
            ++dtotal;
            if (odloc[i] == dloc[i]) {
                ++attack;
                if (knfloc >= 0)
                    knfloc = newloc;
                if (rrand(0, 999) < 95 * (dflag - 2))
                    ++stick;
            }
        }
    }
    if (dtotal == 0)
        return;
    if (dtotal > 1)
        printf("There are %d threatening little dwarves in the room with you!\n", dtotal);
    else
        rspeak(4);
    if (attack == 0)
        return;
    if (dflag == 2)
        ++dflag;
    if (attack > 1) {
        printf("%d of them throw knives at you!!\n", attack);
        k = 6;
    }
    else {
        rspeak(5);
        k = 52;
    }
    if (stick <= 1) {
        rspeak(stick + k);
        if (stick == 0)
            return;
    }
    else
        printf("%d of them get you !!!\n", stick);
    oldloc2 = newloc;
    death();
}
/*
        pirate stuff
*/
void dopirate(void)
{
    int    j, k;
    if (newloc == chloc || prop[CHEST] >= 0)
        return;
    k = 0;
    for (j = 50; j <= MAXTRS; ++j)
        if (j != PYRAMID ||
            (newloc != place[PYRAMID] &&
                newloc != place[EMERALD])) {
            if (toting(j))
                goto stealit;
            if (here(j))
                ++k;
        }
    if (tally == tally2 + 1 && k == 0 && place[CHEST] == 0 &&
        here(LAMP) && prop[LAMP] == 1) {
        rspeak(186);
        move(CHEST, chloc);
        move(MESSAGE, chloc2);
        dloc[6] = chloc;
        odloc[6] = chloc;
        dseen[6] = 0;
        return;
    }
    if (odloc[6] != dloc[6] && pct(20)) {
        rspeak(127);
        return;
    }
    return;

stealit:

    rspeak(128);
    if (place[MESSAGE] == 0)
        move(CHEST, chloc);
    move(MESSAGE, chloc2);
    for (j = 50; j <= MAXTRS; ++j) {
        if (j == PYRAMID &&
            (newloc == place[PYRAMID] ||
                newloc == place[EMERALD]))
            continue;
        if (at(j) && fixed[j] == 0)
            carry(j, newloc);
        if (toting(j))
            drop(j, chloc);
    }
    dloc[6] = chloc;
    odloc[6] = chloc;
    dseen[6] = 0;
}
/*
        special time limit stuff...
*/
bool stimer(void)
{
    int    i;
    foobar = foobar > 0 ? -foobar : 0;
    testbr = 2;
    if (tally == 0 && loc >= 15 && loc != 33)
        --clock1;
    if (clock1 == 0) {
        /*
                start closing the cave
        */
        prop[GRATE] = 0;
        prop[FISSURE] = 0;
        for (i = 1; i < DWARFMAX; ++i)
            dseen[i] = 0;
        move(TROLL, 0);
        move((TROLL + MAXOBJ), 0);
        move(TROLL2, 117);
        move((TROLL2 + MAXOBJ), 122);
        juggle(CHASM);
        if (prop[BEAR] != 3)
            dstroy(BEAR);
        prop[CHAIN] = 0;
        fixed[CHAIN] = 0;
        prop[AXE] = 0;
        fixed[AXE] = 0;
        rspeak(129);
        clock1 = -1;
        closing = true;
        return false;
    }
    if (clock1 < 0)
        --clock2;
    if (clock2 == 0) {
        /*
                set up storage room...
                and close the cave...
        */
        prop[BOTTLE] = put(BOTTLE, 115, 1);
        prop[PLANT] = put(PLANT, 115, 0);
        prop[OYSTER] = put(OYSTER, 115, 0);
        prop[LAMP] = put(LAMP, 115, 0);
        prop[ROD] = put(ROD, 115, 0);
        prop[DWARF] = put(DWARF, 115, 0);
        loc = 115;
        oldloc = 115;
        newloc = 115;
        put(GRATE, 116, 0);
        prop[SNAKE] = put(SNAKE, 116, 1);
        prop[BIRD] = put(BIRD, 116, 1);
        prop[CAGE] = put(CAGE, 116, 0);
        prop[ROD2] = put(ROD2, 116, 0);
        prop[PILLOW] = put(PILLOW, 116, 0);
        prop[MIRROR] = put(MIRROR, 115, 0);
        fixed[MIRROR] = 116;
        for (i = 1; i <= MAXOBJ; ++i)
            if (toting(i))
                dstroy(i);
        rspeak(132);
        closed = true;
        loc = 0;
        return true;
    }
    if (prop[LAMP] == 1)
        --limit;
    if (limit <= 30 &&
        here(BATTERIES) && prop[BATTERIES] == 0 &&
        here(LAMP)) {
        rspeak(188);
        prop[BATTERIES] = 1;
        if (toting(BATTERIES))
            drop(BATTERIES, loc);
        limit += 2500;
        lmwarn = 0;
        return false;
    }
    if (limit == 0) {
        --limit;
        prop[LAMP] = 0;
        if (here(LAMP))
            rspeak(184);
        return false;
    }
    if (limit < 0 && loc <= 8) {
        rspeak(185);
        gaveup = 1;
        normend();
    }
    if (limit <= 30) {
        if (lmwarn || !here(LAMP))
            return(0);
        lmwarn = 1;
        i = 187;
        if (place[BATTERIES] == 0)
            i = 183;
        if (prop[BATTERIES] == 1)
            i = 189;
        rspeak(i);
        return false;
    }
    return false;
}

/*
    tryhint
    -------
    Handles hint logic, prompting the player and updating hint state if accepted.

    Parameters:
        imsg - Message number for the hint prompt
        mask - Bitmask for the hint
        i    - Index for the hint location

    Side Effects:
        - May update hint state and print messages
*/
void tryhint(int imsg, int mask, int i)
{
    putchar('\n');
    if (yes(imsg, 0, 54))
        if (yes(87, imsg + 1, 54)) {
            ++hinttaken;
            hintavail &= ~mask;
        }
    hintloc[i] = 0;
}

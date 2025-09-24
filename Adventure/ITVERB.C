/*      program ITVERB.C                                        */

#include        <stdio.h>
#include        <stdbool.h>
#include        "advent.h"
#include        "advep.h"
#include        "advdec.h"

/*
    itverb
    ------
    Processes intransitive verb commands entered by the player (such as TAKE, DROP, EAT, INVENTORY, etc.). Determines the appropriate action or prompts for an object if needed, and dispatches to the correct handler function for each verb.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variable 'verb' to determine the action to take.
        - May update or use global variables such as 'object', 'object1', 'saveflg', and 'gaveup' depending on the verb.
        - May call functions that print messages, update the world, or end the game (e.g., needobj(), rspeak(), normend(), score(), etc.).
        - May update inventory, game state, or trigger other side effects depending on the verb processed.
*/
void itverb(void)
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
    ivtake
    ------
    Handles the intransitive TAKE command. Searches for a unique object at the player's current location that can be taken. If exactly one object is found, sets it as the target object and calls vtake(). If no object or multiple objects are found, prompts the player for clarification.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object' and 'anobj' to determine and set the object to take.
        - May call needobj() to prompt the player if no or multiple objects are found.
        - Calls vtake() to process the take action if a unique object is found.
        - May use or update other global variables such as 'dflag' and the result of dcheck().
*/
void ivtake(void)
{
    int    anobj, item;

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
}

/*
    ivopen
    ------
    Handles intransitive OPEN, LOCK, and UNLOCK commands. Attempts to identify a unique object at the player's location that can be opened, locked, or unlocked (such as CLAM, OYSTER, DOOR, GRATE, or CHAIN). If multiple or no objects are found, prompts the player for clarification. If a unique object is found, sets it as the target and calls vopen().

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variable 'object' to determine and set the object to open/lock/unlock.
        - May call needobj() to prompt the player if no or multiple objects are found.
        - Calls vopen() to process the open/lock/unlock action if a unique object is found.
        - May use or update other global variables depending on the object being manipulated.
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
}

/*
    ivkill
    ------
    Handles intransitive ATTACK, KILL, and similar commands. Attempts to identify a unique target at the player's location that can be attacked. If multiple or no targets are found, prompts the player for clarification. If a unique target is found, sets it as the target and calls vkill().

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object' and 'object1' to determine and set the target to attack.
        - May use or update 'dflag', 'verb', and the result of dcheck().
        - May call addobj() to set possible targets, needobj() to prompt the player, or vkill() to process the attack.
        - May update the game state depending on the attack outcome.
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
}

/*
    iveat
    -----
    Handles the intransitive EAT command. If food is present at the player's location, sets the global 'object' variable to FOOD and calls veat() to process the action. If food is not present, prompts the player for clarification.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variable 'object' to set FOOD as the target if present.
        - Calls needobj() to prompt the player if food is not present.
        - Calls veat() to process the eat action if food is found.
*/
void iveat(void)
{
    if (!here(FOOD))
        needobj();
    else
    {
        object = FOOD;
        veat();
    }
}

/*
    ivdrink
    -------
    Handles the intransitive DRINK command. If water is available at the player's location or in the bottle, sets the global 'object' variable to WATER and calls vdrink() to process the action. If water is not available, prompts the player for clarification.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variable 'object' to set WATER as the target if available.
        - Calls needobj() to prompt the player if water is not available.
        - Calls vdrink() to process the drink action if water is found.
*/
void ivdrink(void)
{
    if (liqloc(loc) != WATER &&
        (liq() != WATER || !here(BOTTLE)))
        needobj();
    else
    {
        object = WATER;
        vdrink();
    }
}

/*
    ivquit
    ------
    Handles the intransitive QUIT command. Prompts the player for confirmation to quit, and if confirmed, sets the global 'gaveup' variable and ends the game by calling normend().

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Updates the global variable 'gaveup' to indicate if the player chose to quit (set to 1 if quitting, 0 otherwise).
        - Calls normend() to end the game if quitting is confirmed.
*/
void ivquit(void)
{
    gaveup = yes(22, 0, 54);
    if (gaveup)
    {
        normend();
    }
}

/*
    ivfill
    ------
    Handles the intransitive FILL command. If the bottle is present at the player's location, sets the global 'object' variable to BOTTLE and calls vfill() to process the action. If the bottle is not present, prompts the player for clarification.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variable 'object' to set BOTTLE as the target if present.
        - Calls needobj() to prompt the player if the bottle is not present.
        - Calls vfill() to process the fill action if the bottle is found.
*/
void ivfill(void)
{
    if (!here(BOTTLE))
        needobj();
    else
    {
        object = BOTTLE;
        vfill();
    }
}

/*
    ivfoo
    -----
    Handles the special FEE FIE FOE FOO FUM sequence. Updates the global 'foobar' variable to track the sequence state and triggers special game events if the correct sequence is entered. May also update the state of the TROLL and EGGS objects, and print messages to the player.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Updates the global variable 'foobar' to track the current state of the magic word sequence.
        - May update 'prop[TROLL]' and move or print messages about EGGS and TROLL depending on the sequence and game state.
        - Calls rspeak(), move(), and pspeak() to update the game state and print messages.
*/
void ivfoo(void)
{
    int    k, msg;
    k = verb - FEE + 1;
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
    ivread
    ------
    Handles the intransitive READ command. Attempts to identify a readable object at the player's location (MAGAZINE, TABLET, MESSAGE). If found, sets it as the target and calls vread(). If no readable object is found or it is too dark, prompts the player for clarification.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variable 'object' to set the readable object as the target.
        - Calls needobj() to prompt the player if no readable object is found or if it is too dark.
        - Calls vread() to process the read action if a readable object is found.
*/
void ivread(void)
{
    if (here(MAGAZINE))
        object = MAGAZINE;
    if (here(TABLET))
        object = object * 100 + TABLET;
    if (here(MESSAGE))
        object = object * 100 + MESSAGE;
    if (object > 100 || object == 0 || dark())
    {
        needobj();
        return;
    }
    vread();
}

/*
    invent
    ------
    Handles the intransitive INVENTORY command. Prints a list of items currently carried by the player, including a special message if the player is carrying the bear.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads the player's inventory by checking global variables and arrays such as 'toting', 'BEAR', and 'MAXOBJ'.
        - Prints messages to the player using rspeak() and pspeak().
        - May print a special message if the player is carrying the bear.
*/
void invent(void)
{
    int    msg, i;

    msg = 98;
    for (i = 1; i <= MAXOBJ; ++i)
    {
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
    addobj
    ------
    Ensures uniqueness when searching for objects for intransitive verbs. If no object is currently set, sets the global 'object' variable to the candidate object. If an object is already set, sets the global 'object1' variable to -1 to indicate ambiguity.

    Parameters:
        obj - The object number to consider as a candidate

    Side Effects:
        - Updates the global variables 'object' and 'object1' to track candidate objects for the verb. 'object' is set if not already set; 'object1' is set to -1 if multiple candidates are found.
*/
void addobj(int obj)
{
    if (object1 != 0)
        return;
    if (object != 0)
    {
        object1 = -1;
        return;
    }
    object = obj;
}

/*
    ivbrief
    -------
    Handles the intransitive BRIEF command. Sets the game to brief mode, which affects how room descriptions are displayed to the player. Updates the global 'detail' variable to enable brief mode and calls actspk() to print a message about the mode change.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Updates the global variable 'detail' by setting its brief mode bit (typically |= 2).
        - Calls actspk() to print a message indicating that brief mode is now active.
*/
void ivbrief(void)
{
    detail |= 2;
    actspk(verb);
}

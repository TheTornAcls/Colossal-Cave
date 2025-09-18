/*      program VERB.C                                          */


#include        <stdio.h>
#include        <stdbool.h>
#include        "advent.h"
#include        "advep.h"
#include        "advdec.h"

/*
    trverb
    ------
    Processes a transitive verb command (a verb that acts on an object) entered by the player. Dispatches to the appropriate handler function for the current verb and object, updating the game state as needed.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads the global variable 'verb' to determine which action to perform.
        - Uses and may update global variables such as 'object', 'loc', and others depending on the verb.
        - Calls various handler functions (e.g., vtake(), vdrop(), vopen(), etc.) that may update inventory, object properties, player state, or print messages.
        - May update the game world, inventory, or end the game depending on the verb processed.
*/
void trverb(void)
{
        switch(verb){
        case CALM:
        case WALK:
        case QUIT:
        case SCORE:
        case FOO:
        case BRIEF:
        case SUSPEND:
        case HOURS:
        case LOG:
                actspk(verb);
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
                if (object != LAMP)
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
    vtake
    -----
    Handles the TAKE (or CARRY) command for a specific object. Attempts to pick up the specified object, updating the player's inventory and the game state as appropriate. Handles special cases for certain objects (e.g., liquids, bird, cage) and inventory limits.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object', 'loc', 'holding', and arrays such as 'prop', 'fixed', 'place'.
        - May update inventory, object properties, and print messages to the player.
        - Calls carry(), drop(), vfill(), rspeak(), pspeak(), and other functions to update the game state.
        - May affect the state of special objects (e.g., BIRD, CAGE, BOTTLE, liquids).
*/
void vtake(void)
{
        int    msg, i;

        if (toting(object)) {
                actspk(verb);
                return;
        }
        /*
         special case objects and fixed objects
        */
        msg = 25;
        if (object == PLANT && prop[PLANT] <= 0)
                msg = 115;
        if (object == BEAR && prop[BEAR] == 1)
                msg = 169;
        if (object == CHAIN && prop[BEAR] != 0)
                msg = 170;
        if (fixed[object]) {
                rspeak(msg);
                return;
        }
        /*
         special case for liquids
        */
        if (object == WATER || object == OIL) {
                if (!here(BOTTLE) || liq() != object) {
                        object = BOTTLE;
                        if (toting(BOTTLE)&&prop[BOTTLE] == 1){
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
                object = BOTTLE;
        }
        if (holding >= 7) {
                rspeak(92);
                return;
        }
        /*
         special case for bird.
        */
        if (object == BIRD && prop[BIRD] == 0) {
                if (toting(ROD)) {
                        rspeak(26);
                        return;
                }
                if (!toting(CAGE)) {
                        rspeak(27);
                        return;
                }
                prop[BIRD] = 1;
        }
        if ( (object == BIRD || object == CAGE) &&
                prop[BIRD] != 0)
                carry((BIRD+CAGE)-object,loc);
        carry(object,loc);
        /*
         handle liquid in bottle
        */
        i = liq();
        if (object == BOTTLE && i != 0)
                place[i] = -1;
        rspeak(54);
}

/*
    vdrop
    -----
    Handles the DROP command for a specific object. Attempts to drop the specified object at the player's current location, updating the player's inventory and the game state as appropriate. Handles special cases for certain objects (e.g., bird, snake, coins, bear, vase, liquids) and updates object properties and locations.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object', 'loc', and arrays such as 'prop', 'fixed', 'place'.
        - May update inventory, object properties, and print messages to the player.
        - Calls drop(), dstroy(), move(), pspeak(), rspeak(), and other functions to update the game state.
        - May affect the state of special objects (e.g., BIRD, SNAKE, COINS, BEAR, VASE, BOTTLE, liquids).
*/
void vdrop(void)
{
        int    i, msg = 54;

        /*
         check for dynamite
        */
        if (toting(ROD2) && object == ROD && !toting(ROD))
                object = ROD2;
        if (!toting(object)){
                actspk(verb);
                return;
        }
        /*
         snake and bird
        */
        if (object == BIRD && here(SNAKE)) {
                rspeak(30); msg = 0;
                if (closed)
                        dwarfend(136);
                dstroy(SNAKE);
                prop[SNAKE] = -1;
        }
        /*
         coins and vending machine
        */
        else if (object == COINS && here(VEND)) {
                dstroy(COINS);
                drop(BATTERIES,loc);
                pspeak(BATTERIES,0,0);
                return;
        }
        /*
         bird and dragon (ouch!!)
        */
        else if (object == BIRD && at(DRAGON) && prop[DRAGON] == 0){
                rspeak(154); msg = 0;
                dstroy(BIRD);
                prop[BIRD] = 0;
                if (place[SNAKE] != 0)
                        ++tally2;
                return;
        }
        /*
         Bear and troll
        */
        if (object == BEAR && at(TROLL)) {
                rspeak(163); msg = 0;
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
        else if (object == VASE) {
                if (loc == 96)
                        rspeak(54);
                else {
                        prop[VASE] = at(PILLOW) ? 0 : 2;
                        pspeak(VASE,prop[VASE]+1,0);
                        if (prop[VASE] != 0)
                                fixed[VASE] = -1;
                }
                msg = 0;
        }
        /*
         handle liquid and bottle
        */
        i = liq();
        if (i == object)
                object = BOTTLE;
        if (object == BOTTLE && i != 0)
                place[i] = 0;
        /*
         handle bird and cage
        */
        if (object == CAGE && prop[BIRD] != 0)
                drop(BIRD,loc);
        if (object == BIRD)
                prop[BIRD] = 0;
        drop(object,loc);
        rspeak (msg);
}

/*
    vopen
    -----
    Handles the OPEN, LOCK, UNLOCK, and CLOSE commands for a specific object. Attempts to perform the requested action on the specified object, updating its state and the game world as appropriate. Handles special cases for certain objects (e.g., CLAM, OYSTER, DOOR, CAGE, KEYS, CHAIN, GRATE) and updates object properties and locations.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object', 'verb', 'loc', and arrays such as 'prop', 'fixed', 'place', 'clock2', 'panic'.
        - May update object properties, locations, and print messages to the player.
        - Calls drop(), dstroy(), move(), pspeak(), rspeak(), and other functions to update the game state.
        - May affect the state of special objects (e.g., CLAM, OYSTER, DOOR, CAGE, KEYS, CHAIN, GRATE).
*/
void vopen(void)
{
        int    msg,oyclam;

        switch(object) {
        case CLAM:
        case OYSTER:
                oyclam = (object == OYSTER ? 1 : 0);
                if (verb == LOCK)
                        msg = 61;
                else if (!toting(TRIDENT))
                        msg = 122+oyclam;
                else if (toting(object))
                        msg = 120+oyclam;
                else {
                        msg = 124+oyclam;
                        dstroy(CLAM);
                        drop(OYSTER,loc);
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
                else if (verb == LOCK) {
                        if (prop[CHAIN] != 0)
                                msg = 34;
                        else if (loc != 130)
                                msg = 173;
                        else {
                                prop[CHAIN] = 2;
                                if (toting(CHAIN))
                                        drop(CHAIN,loc);
                                fixed[CHAIN] = -1;
                                msg = 172;
                        }
                }
                else {
                        if (prop[BEAR] == 400)
                                msg = 41;
                        else if (prop[CHAIN] == 0)
                                msg = 37;
                        else {
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
                else if (closing) {
                        if (!panic) {
                                clock2 = 15;
                                ++panic;
                        }
                        msg = 130;
                }
                else {
                        msg = 34+prop[GRATE];
                        prop[GRATE] = (verb == LOCK ? 0 : 1);
                        msg += 2*prop[GRATE];
                }
                break;
        default:
                msg = 33;
        }
        rspeak(msg);
}

/*
    vsay
    ----
    Handles the SAY command. Prints the word or phrase the player wants to say. Analyzes the input words and prints the appropriate response.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads the global variables 'word1' and 'word2' to determine what the player said.
        - May print a message to the player using printf().
*/
void vsay(void)
{
        int wtype,wval;

        analyze(word1,&wtype,&wval);
        printf("Okay.\n%s\n",wval == SAY ? word2 : word1);
}


/*
    von
    ---
    Handles the ON command, specifically for turning on the lamp in the game. 
    Checks if the lamp is present, if it has power remaining, and updates the lamp's state and the game environment accordingly.
    
    Parameters:
        None (operates on and updates global game state variables)

    Global Variables Used and Modified:
        - LAMP: Constant representing the lamp object.
        - prop[]: Array of object properties; prop[LAMP] is set to 1 to indicate the lamp is on.
        - limit: Integer tracking the remaining lamp life; checked to ensure the lamp can be turned on.
        - wzdark: Boolean indicating if the current location is dark; set to false if the lamp is turned on in darkness.
        - verb: The current verb code, used for default action messages.
        - Functions called: here(), actspk(), rspeak(), describe().
*/
void von(void)
{
    if (!here(LAMP))
        actspk(verb);
    else if (limit<0)
        rspeak(184);
    else {
        prop[LAMP] = 1;
        rspeak(39);
        if (wzdark) {
            wzdark = false;
            describe();
        }
    }
}

/*
    voff
    ----
    Handles the OFF command, specifically for turning off the lamp. Checks if the lamp is present and, if so, updates its state and prints appropriate messages.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variable 'prop' to set the lamp's state to off.
        - Prints messages to the player using rspeak().
*/
void voff(void)
{
        if (!here(LAMP))
                actspk(verb);
        else {
                prop[LAMP] = 0;
                rspeak(40);
        }
}

/*
    vwave
    -----
    Handles the WAVE command for a specific object. Checks if the player is holding the object (typically a rod) and, if appropriate, toggles the state of the fissure. Prints messages to the player based on the action taken.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object', 'prop', and may call pspeak() and rspeak() to print messages.
        - May update the state of the fissure if the correct object is waved at the correct location.
*/
void vwave(void)
{
        if (!toting(object) &&
         (object != ROD || !toting(ROD2)))
                rspeak(29);
        else if (object != ROD || !at(FISSURE) ||
                !toting(object) || closing)
                actspk(verb);
        else {
                prop[FISSURE] = 1-prop[FISSURE];
                pspeak(FISSURE,2-prop[FISSURE],0);
        }
}

/*
    vkill
    ------
    Handles the KILL (or ATTACK) command for a specific object. Attempts to attack or kill the specified object, updating its state and the game world as appropriate. Handles special cases for BIRD, CLAM, OYSTER, SNAKE, DWARF, TROLL, BEAR, DRAGON, and default cases. May end the game if certain conditions are met (e.g., breaking the mirror while the cave is closed).

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object', 'closed', 'prop', 'place', 'tally2', 'newloc', and others.
        - Calls functions such as dstroy(), rspeak(), pspeak(), move(), yes(), actspk(), and dwarfend() to update the game state and print messages.
        - May end the game or move objects depending on the outcome of the attack.
*/
void vkill(void)
{
        int    msg, i;

        switch(object) {
        case BIRD:
                if (closed)
                        msg = 137;
                else {
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
                        dwarfend(136);
                msg = 49;
                break;
        case TROLL:
                msg = 157;
                break;
        case BEAR:
                msg = 165+(prop[BEAR]+1)/2;
                break;
        case DRAGON:
                if (prop[DRAGON] != 0) {
                        msg = 167;
                        break;
                }
                if (!yes(49,0,0))
                        break;
                pspeak(DRAGON,1,0);
                prop[DRAGON] = 2;
                prop[RUG] = 0;
                move((DRAGON+MAXOBJ),-1);
                move((RUG+MAXOBJ),0);
                move(DRAGON,120);
                move(RUG,120);
                for(i = 1;i<MAXOBJ;++i)
                        if (place[i] == 119 || place[i] == 121)
                                move(i,120);
                newloc = 120;
                return;
        default:
                actspk(verb);
                return;
        }
        rspeak(msg);
}

/*
    vpour
    -----
    Handles the POUR command for a specific object. Attempts to pour the contents of the bottle or a specific liquid, updating the game state and object properties as appropriate. Handles special cases for pouring on the plant or door, and prints messages to the player.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object', 'prop', 'place', and others.
        - Calls functions such as liq(), at(), pspeak(), rspeak(), and needobj() to update the game state and print messages.
        - May update the state of the BOTTLE, PLANT, DOOR, and other objects depending on the action.
*/
void vpour(void)
{
        if (object == BOTTLE || object == 0)
                object = liq();
        if (object == 0) {
                needobj();
                return;
        }
        if (!toting(object)) {
                actspk(verb);
                return;
        }
        if (object != OIL && object != WATER) {
                rspeak(78);
                return;
        }
        prop[BOTTLE] = 1;
        place[object] = 0;
        if (at(PLANT)) {
                if (object != WATER)
                        rspeak(112);
                else {
                        pspeak(PLANT,prop[PLANT]+1,0);
                        prop[PLANT] = (prop[PLANT]+2)%6;
                        prop[PLANT2] = prop[PLANT]/2;
                        describe();
                }
        }
        else if (at(DOOR)) {
                prop[DOOR] = (object == OIL ? 1 : 0);
                rspeak(113+prop[DOOR]);
        }
        else
                rspeak(77);
}

/*
    veat
    ----
    Handles the EAT command for a specific object. Attempts to eat the specified object, updating the game state and object properties as appropriate. Handles special cases for FOOD and inedible objects, and prints messages to the player.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variable 'object'.
        - Calls functions such as dstroy(), rspeak(), and actspk() to update the game state and print messages.
        - May update the state of the FOOD object or print a message for inedible objects.
*/
void veat(void)
{
        int    msg;

        switch(object) {
        case FOOD:
                dstroy(FOOD);
                msg = 72;
                break;
        case BIRD: case SNAKE: case CLAM: case OYSTER:
        case DWARF: case DRAGON: case TROLL: case BEAR:
                msg = 71;
                break;
        default:
                actspk(verb);
                return;
        }
        rspeak(msg);
}

/*
    vdrink
    ------
    Handles the DRINK command for a specific object. Attempts to drink water if available, updating the game state and object properties as appropriate. Prints messages to the player based on the outcome.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object', 'prop', and 'place'.
        - Calls functions such as liq(), here(), actspk(), and rspeak() to update the game state and print messages.
        - May update the state of the BOTTLE and WATER objects depending on the action.
*/
void vdrink(void)
{
        if (object != WATER)
                rspeak(110);
        else if (liq() != WATER || !here(BOTTLE))
                actspk(verb);
        else {
                prop[BOTTLE] = 1;
                place[WATER] = 0;
                rspeak(74);
        }
}

/*
        THROW etc.
*/
void vthrow(void)
{
        int    msg, i;

        if (toting(ROD2) && object == ROD && !toting(ROD))
                object = ROD2;
        if (!toting(object)) {
                actspk(verb);
                return;
        }
        /*
         treasure to troll
        */
        if (at(TROLL) && object >= 50 && object<MAXOBJ) {
                rspeak(159);
                drop(object,0);
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
        if (object == FOOD && here(BEAR)) {
                object = BEAR;
                vfeed();
                return;
        }
        /*
         if not axe, same as drop...
        */
        if (object != AXE) {
                vdrop();
                return;
        }
        /*
         AXE is THROWN
        */
        /*
         at a dwarf...
        */
        if (i = dcheck()) {
                msg = 48;
                if (pct(33)) {
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
        else if (here(BEAR) && prop[BEAR] == 0) {
                rspeak(164);
                drop(AXE,loc);
                fixed[AXE] = -1;
                prop[AXE] = 1;
                juggle(BEAR);
                return;
        }
        /*
         otherwise it is an attack
        */
        else {
                verb = KILL;
                object = 0;
                itverb();
                return;
        }
        /*
         handle the left over axe...
        */
        rspeak(msg);
        drop(AXE,loc);
        describe();
}

/*
    vfind
    -----
    Handles the FIND and INVENTORY commands for a specific object, determining if the player is carrying, can see, or otherwise access the object, and prints an appropriate message based on the game state.

    Parameters:
        None (operates on and updates global game state variables; does not take explicit parameters).

    Global Variables Used:
        - object: The object being searched for or inventoried.
        - closed: Indicates whether the cave is closed.
        - dflag: Used to determine if dwarves are active.
        - verb: The current verb code (used by actspk()).
        - DWARF: Constant representing the dwarf object.
        - Other functions and variables such as toting(), at(), liq(), here(), liqloc(), dcheck() are used to check the state and location of objects.

    Side Effects:
        - May print a message to the player using rspeak() or actspk().
        - Does not modify any global variables.
*/
void vfind(void)
{
        int    msg;
        if (toting(object))
                msg = 24;
        else if (closed)
                msg = 138;
        else if (dcheck() && dflag >= 2 && object == DWARF)
                msg = 94;
        else if (at(object) ||
                (liq() == object && here(BOTTLE)) ||
                object == liqloc(loc))
                msg = 94;
        else {
                actspk(verb);
                return;
        }
        rspeak(msg);
}

/*
    vfill
    -----
    Handles the FILL command for a specific object, allowing the player to fill containers such as the bottle or vase with available liquids at the current location. Determines the appropriate message or action based on the object, its state, and the environment.

    Parameters:
        None (operates on and updates global game state variables; does not take explicit parameters).

    Global Variables Used and Modified:
        - object: Determines which object (BOTTLE, VASE, or other) is being filled.
        - loc: The player's current location, used to check for available liquids.
        - prop[]: Updates the properties of BOTTLE and VASE based on the fill action.
        - cond[]: Used to determine the type of liquid available at the current location.
        - place[]: May update the location of the liquid object (OIL or WATER) when filling the bottle.
        - WATOIL, OIL: Used to identify and set the type of liquid.
        - Functions called: liq(), liqloc(), toting(), vdrop(), rspeak().
*/
void vfill(void)
{
        int    msg, i;

        switch(object) {
        case BOTTLE:
                if (liq() != 0)
                        msg = 105;
                else if (liqloc(loc) == 0)
                        msg = 106;
                else {
                        prop[BOTTLE] = cond[loc] & WATOIL;
                        i = liq();
                        if (toting(BOTTLE))
                                place[i] = -1;
                        msg = (i == OIL ? 108 : 107);
                }
                break;
        case VASE:
                if (liqloc(loc) == 0) {
                        msg = 144;
                        break;
                }
                if (!toting(VASE)) {
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
    vfeed
    -----
    Handles the FEED command for a specific object, determining the outcome based on the object type and the current game state.
    This function checks if the player is attempting to feed an object (such as a bird, dwarf, bear, dragon, troll, or snake),
    and updates the game state, object properties, and prints the appropriate message to the player.

    Parameters:
        None (operates on and updates global game state variables; does not take explicit parameters).

    Global Variables Affected:
        - object: Determines which object is being fed and controls the logic flow.
        - prop[]: Updates properties of objects such as BEAR, AXE, BIRD, and DRAGON based on the feeding action.
        - fixed[]: May update the fixed status of AXE when feeding the bear.
        - dflag: Increments if the dwarf is fed.
        - tally2: Increments if the snake is fed a bird.
        - FOOD, BIRD: May be destroyed (removed from the game) if used as food.
        - Calls rspeak() and actspk() to print messages to the player.
*/
void vfeed(void)
{
        int    msg;

        switch(object) {
        case BIRD:
                msg = 100;
                break;
        case DWARF:
                if (!here(FOOD)) {
                        actspk(verb);
                        return;
                }
                ++dflag;
                msg = 103;
                break;
        case BEAR:
                if (!here(FOOD)) {
                        if (prop[BEAR] == 0)
                                msg = 102;
                        else if (prop[BEAR] == 3)
                                msg = 110;
                        else {
                                actspk(verb);
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
                if (closed || !here(BIRD)) {
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
    vread
    -----
    Handles the READ command for a specific object. Determines if the object can be read based on the current game state (e.g., darkness, object type, inventory, and closure status), and prints the appropriate message or hint to the player.

    Parameters:
        None (operates on and updates global game state variables)

    Global Variables Used:
        - object: The current object the verb is acting upon.
        - closed: Indicates whether the cave is closed.
        - hintavail: Bitmask indicating which hints are available.
        - hinttaken: Counter for the number of hints taken.
        - verb: The current verb code (used by actspk()).

    Side Effects:
        - May print messages to the player using printf(), rspeak(), or actspk().
        - May update hinttaken and hintavail if a hint is given for the OYSTER object.
*/
void vread(void)
{
        int    msg;

        msg = 0;
        if (dark()) {
                printf("I see no %s here.\n",probj(object));
                return;
        }
        switch(object) {
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
                if (hintavail&HINTO)
                        if (yes(192,193,54)) {
                            ++hinttaken;
                            hintavail &= ~HINTO;
                        }
                        else;
                else
                    rspeak(194);
                return;
        default:
                ;
        }
        if (msg)
                rspeak(msg);
        else
                actspk(verb);
}

/*
    vblast
    ------
    Executes the BLAST command, checking the state of the second rod and whether the cave is closed; if conditions are met, it delivers a special bonus message and ends the game.

    Parameters:
        None (operates on and updates global game state variables)

    Global Variables Used:
        - prop: Array of object properties, used to check the state of ROD2.
        - closed: Indicates whether the cave is closed.
        - loc: The player's current location, used to determine the bonus message.
        - bonus: Sets the message code to be delivered.
    
    Side Effects:
        - May print a default action message or a special bonus message to the player.
        - Calls normend() to end the game if the BLAST is successful.
*/
void vblast(void)
{
        if (prop[ROD2] < 0 || !closed)
                actspk(verb);
        else {
                bonus = 133;
                if (loc == 115)
                        bonus = 134;
                if (here(ROD2))
                        bonus = 135;
                rspeak(bonus);
                normend();
        }
}

/*
    vbreak
    ------
    Handles the BREAK command for a specific object. Attempts to break the specified object, updating its state and the game world as appropriate. Handles special cases for MIRROR and VASE, updating their properties and locations, and may end the game if the mirror is broken while the cave is closed.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'object', 'closed', and arrays such as 'prop', 'fixed'.
        - Calls drop(), rspeak(), dstroy(), and dwarfend() to update the game state and print messages.
        - May end the game if the mirror is broken while the cave is closed.
        - May update the state of the VASE and MIRROR objects.
*/
void vbreak(void)
{
        int    msg;
        if (object == MIRROR) {
                msg = 148;
                if (closed) {
                        rspeak(197);
                        dwarfend(136);
                }
        }
        else if (object == VASE && prop[VASE] == 0) {
                msg = 198;
                if (toting(VASE))
                        drop(VASE,loc);
                prop[VASE] = 2;
                fixed[VASE] = -1;
        }
        else {
                actspk(verb);
                return;
        }
        rspeak(msg);
}

/*
    vwake
    -----
    Triggers an action based on the state of the object and game closure, either invoking a default action or ending the game if the player tries to wake a dwarf after the cave is closed.

    Parameters:
        None (operates on and updates global game state variables)

    Global Variables Used:
        - object: The current object the verb is acting upon.
        - closed: Indicates whether the cave is closed.
        - verb: The current verb code (used by actspk()).

    Side Effects:
        - Calls actspk(verb) to print a default message if the object is not a dwarf or the cave is not closed.
        - Calls dwarfend(199) to end the game if the player tries to wake a dwarf after the cave is closed.
*/
void vwake(void)
{
        if (object != DWARF || !closed)
                actspk(verb);
        else
                dwarfend(199);
}

/*
    actspk
    ------
    Prints the default message associated with a given verb. Looks up the message for the specified verb and prints it to the player, if available.

    Parameters:
        verb - The verb code (integer) for which to print the default message.

    Side Effects:
        - Reads the global array 'actmsg' to look up the message for the verb.
        - Calls rspeak() to print the message to the player.
        - May call bug() if the verb code is out of range.
*/
void actspk(int verb)
{
        int i;

        if (verb < 1 || verb>32)
        {
            bug(39);
        }
        if ((verb >= 0) && (verb < ACTMSG))
        {
            i = actmsg[verb];
            if (i)
                rspeak(i);
        }
}

/*
    needobj
    -------
    Prints a prompt asking the player to specify an object for the current verb. Analyzes the player's input words to generate an appropriate prompt.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads the global variables 'word1' and 'word2' to determine the player's input.
        - Calls analyze() to process the input words.
        - Prints a prompt to the player using printf().
*/
void needobj(void)
{
        int wtype,wval;

        analyze(word1,&wtype,&wval);
        printf("%s what?\n",wtype == 2 ? word1 : word2);
}

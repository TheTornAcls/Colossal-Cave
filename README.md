# Adventure

This is the real, full-blown adventure program written by Will Crowther
and Don Woods at SAIL in the early seventies.  This Public Domain
version has been translated from BDS C to CII by Jerry Pohl and then
to C Set++ C by John Kennedy.

See ADVENT.TXT for info on how the program was modified and how you can
change the source.

All the .C files are C source code. The .TXT files are data used
by the program ADVENT0.EXE.

To run ADVENTURE simply enter --

```
ADVENTURE.exe
``` 
and you're on your way.

## Before you begin, a few suggestions:

1. Make a map. There are two common ways to handle this. You can
use a piece of butcher paper and a pencil and sketch in all the
rooms as circles or whatever with directions marked for the lines
between rooms. The other way to make a map is to make a word table
with the room names down one side along with numbers you've assigned
each room. Along the top of the table you put N, NW, W, SW, S, SE, E
and so on. (don't forget up and down). Then you can tell at a glance
which directions you have tried and haven't tried.

2. Save the game every half hour or so (unless you have spent a lot
of moves getting nowhere).

3. Take your time. I've seen major gamers spend 6 months working on
this one. They were spending 2 or 3 hours a day and they weren't
beginners!

>>>>> Try everything!


Sometimes you will have to go back to a previous location to be able to do something.  The game parser uses simple two word (verb-noun) commands (after all, this is the first text adventure game. Infocom wasn't to be for a while yet!)

-----------------------------------------------------------------------------
Load the Adventure.sln file into Visual Studio (I used Visual Studio 2019 Community Edition)

Build for you platform (x86/x64/ARM64)
The Adventure.exe will be in (repo)\Adventure\(Configuration)\(Platform)
For example:
```
Adventure\Debug\x64
```

ADVENTURE.EXE is the executable adventure program.
>**NOTE:** You must run Adventure.exe in the directory where it built, because is uses data files located there.

The *.C files are the source files for the adventure program,
and the files it uses.

ADVENT.TXT will tell you what files you must have on a disk in
order to run and to modify the program.

The *.TXT files provide the text you see on the screen when
you're playing adventure.

-----------------------------------------------------------------------------

## CHEAT.TXT --

>>>>  PLEASE TRY TO PLAY THE GAME FIRST WITHOUT READING THIS FILE ! 

But if you really need some help in the very beginning, enter --

TYPE CHEAT.TXT

for adventure clues.

Use Ctrl-S (hold down the control key and then strike the S key)
to start and stop the screen scroll.

The pure adventurers out there will probably hate me for this,
but this short document will get you started if you really
are stuck.  You can use part of this help file to begin with,
and then come back later for more help if you need it.

------------------------------------------------------------------------------

        The best of luck, and may the pirate give up his chest and all
              your nasty dwarves turn into puffs of black smoke!
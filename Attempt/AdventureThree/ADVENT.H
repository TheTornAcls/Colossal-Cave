/*      header ADVENT.H                                         *\
\*      HEADER file for all adventure modules                   */


#define MAXOBJ 100       /* max # of objects in cave                 */
#define MAXWC    295     /* max # of adventure words                 */
#define MAXLOC 140       /* max # of cave locations                  */
#define WORDSIZE 20      /* max # of chars in commands               */
#define MAXMSG 209       /* max # of long location descr             */

#define MAXTRAV (16+1) /* max # of travel directions from loc        */
                         /* +1 for terminator travel[x].tdest=-1 */
#define DWARFMAX 7       /* max # of nasty dwarves                   */
#define MAXDIE 3         /* max # of deaths before close             */
#define MAXTRS 79        /* max # of treasures                       */

/*
        Object definitions
*/
#define KEYS          1
#define LAMP          2
#define GRATE         3
#define CAGE          4
#define ROD           5
#define ROD2          6
#define STEPS         7
#define BIRD          8
#define DOOR          9
#define PILLOW       10
#define SNAKE        11
#define FISSURE      12
#define TABLET       13
#define CLAM         14
#define OYSTER       15
#define MAGAZINE     16
#define DWARF        17
#define KNIFE        18
#define FOOD         19
#define BOTTLE       20
#define WATER        21
#define OIL          22
#define MIRROR       23
#define PLANT        24
#define PLANT2       25
#define AXE          28
#define DRAGON       31
#define CHASM        32
#define TROLL        33
#define TROLL2       34
#define BEAR         35
#define MESSAGE      36
#define VEND         38
#define BATTERIES    39
#define NUGGET       50
#define COINS        54
#define CHEST        55
#define EGGS         56
#define TRIDENT      57
#define VASE         58
#define EMERALD      59
#define PYRAMID      60
#define PEARL        61
#define RUG          62
#define SPICES       63
#define CHAIN        64

/*
        Verb definitions
*/
#define NULLX        21
#define BACK          8
#define LOOK         57
#define CAVE         67
#define ENTRANCE     64
#define DEPRESSION 63

/*
        Action verb definitions
*/
#define TAKE          1
#define DROP          2
#define SAY           3
#define OPEN          4
#define NOTHING       5
#define LOCK          6
#define ON            7
#define OFF           8
#define WAVE          9
#define CALM         10
#define WALK         11
#define KILL         12
#define POUR         13
#define EAT          14
#define DRINK        15
#define RUB          16
#define THROW        17
#define QUIT         18
#define FIND         19
#define INVENTORY    20
#define FEED         21
#define FILL         22
#define BLAST        23
#define SCORE        24
#define BRIEF        26
#define READ         27
#define BREAK        28
#define WAKE         29
#define SUSPEND      30
#define HOURS        31
#define LOG          32
#define FEE          33
#define FIE          34
#define FOE          35
#define FOO          36
#define FUM          37
#define HELP         51

/*
        BIT mapping of "cond" array which indicates location status
*/
#define LIGHT        1
#define WATOIL       2
#define LIQUID       4
#define NOPIRAT      8
#define FORCED      16
#define HINTC       32
#define HINTB       64
#define HINTS      128
#define HINTM      256
#define HINTP      512
#define HINTW     1024
#define HINTF     2048
#define HINTO     4096
#define HINT      8160
#define HINTAREAC    0
#define HINTAREAB    1
#define HINTAREAS    2
#define HINTAREAM    3
#define HINTAREAP    4
#define HINTAREAW    5
#define HINTAREAF    6
#define MAXHINT      6


/*
        Structure definitions
*/
struct  wac {
        char    *aword;
        int     acode;
        };

struct  trav {
        int     tdest;
        int     tverb;
        int     tcond;
        };

/*
        File definitions
*/
#if defined(MVS)
#define SAVEMODE "wb,recfm=f,lrecl=1348,blksize=1348"
#define RESTMODE "rb,recfm=f,lrecl=1348,blksize=1348"
#define ISAM     "DD:ADVTEXT"
#define ISAMMODE "w,recfm=vb"
#define FD1      "DD:ADVTEXT1"
#define FD2      "DD:ADVTEXT2"
#define FD3      "DD:ADVTEXT3"
#define FD4      "DD:ADVTEXT4"
#else
#define SAVEMODE "wb"
#define RESTMODE "rb"
#define ISAM     "advtext.h"
#define ISAMMODE "w"
#define FD1      "advent1.txx"
#define FD2      "advent2.txx"
#define FD3      "advent3.txx"
#define FD4      "advent4.txx"
#endif

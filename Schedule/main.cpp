#include "schedule.h"


int main(int argc, char *argv[])
{
    cSchedule MySchedule;
    INT iOutput = 0;

    //MySchedule.Add(3,4,BUSY);
    //MySchedule.Add(1,2,BUSY);
    //MySchedule.Add(3,4,BUSY);
    //MySchedule.Add(2,5,AWAY);
    //MySchedule.Add(1,3,AWAY);

    TNODE tnTest[5];

    printf("size of TNODE = %d\n",sizeof(TNODE));
    printf("size of tnTest = %d\n",sizeof(tnTest));
    printf("size of *TNODE = %d\n",sizeof(PTNODE));


    // find a better way of initializing this list
    tnTest[0].iStart =3;
    tnTest[0].iEnd =4;
    tnTest[0].iStatus =BUSY;
    tnTest[1].iStart =1;
    tnTest[1].iEnd =3;
    tnTest[1].iStatus =AWAY;
    tnTest[2].iStart =1;
    tnTest[2].iEnd =2;
    tnTest[2].iStatus =BUSY;
    tnTest[3].iStart =2;
    tnTest[3].iEnd =5;
    tnTest[3].iStatus =BUSY;
    tnTest[4].iStart =3;
    tnTest[4].iEnd =4;
    tnTest[4].iStatus =AWAY;

    MySchedule.Input(&tnTest[0], 5);

    MySchedule.Print();

    MySchedule.ReturnSummary(NULL, &iOutput);

    printf("iOutput = %d\n", iOutput);

    return 1;
}


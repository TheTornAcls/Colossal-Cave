#include <stdio.h>
#include <windows.h>

#define LENGTH 30
#define HEIGHT  30

CHAR g_Source[LENGTH][HEIGHT];
CHAR g_Dest[LENGTH][HEIGHT];


void BInitialize();
void BDump();
VOID MyBitBlt(INT iXSrc, INT iYSrc, INT iSrcLength, INT iSrcHeight,
              INT iXDest, INT iYDest);

int main(int argc, char *argv[])
{

    BInitialize();
    BDump();
    MyBitBlt( 0,0, 10, 10,10,10);
    BDump();
    MyBitBlt( -5,0, 10, 10,3,3);
    BDump();
    MyBitBlt( 25,0, 10, 10,20,20);
    BDump();

    return 1;

}

VOID MyBitBlt(INT iXSrc, INT iYSrc, INT iSrcLength, INT iSrcHeight,
              INT iXDest, INT iYDest)
{
    // First adjust for clipping
    if (iXSrc < 0)
    {             \
        printf("iXSrc < 0\n");
        iSrcLength = iSrcLength - (0 - iXSrc);
        iXDest = iXDest + (0-iXSrc);
        iXSrc = 0;

        printf("iXSrc = %d\n", iXSrc);
        printf("iXDest = %d\n", iXDest);
        printf("iSrcLength = %d\n", iSrcLength);

    }
    if (iXSrc > LENGTH)
    {
        printf("iXSrc > LENGTH\n");
          iXSrc = 0;
          iSrcLength = 0;
          iXDest = 0;
    }

    if ( ( (iSrcLength) + iXSrc) > LENGTH)
    {
        printf("iXSrc +iSrcLength > LENGTH\n");
        // in this case you the length plus the source is greater than the total length
        iSrcLength = LENGTH - iXSrc;

    }

    // Copy from Source Surface to Destination Surface
    INT iCountX;
    INT iCountY;

    for (iCountX =0; iCountX< iSrcLength; iCountX++)
    {
        for (iCountY= 0; iCountY < iSrcHeight; iCountY++)
        {
            g_Dest[iXDest + iCountX][iYDest + iCountY] = g_Source[iXSrc+ iCountX][iYSrc+ iCountY];
        }
    }

    return;
}

void BInitialize()
{
    INT iX, iY;

    for (iX =0; iX < LENGTH; iX++)
    {    
        for (iY = 0; iY < HEIGHT; iY++)
        {
            g_Dest[iX][iY] = ' ';
            g_Source[iX][iY] = '@';
        }
    }

    return;
}

void BDump()
{
    INT iX, iY;

    printf("LENGTH = %d\n", LENGTH);
    printf("HEIGHT = %d\n", HEIGHT);

    for (iX =0; iX < LENGTH; iX++)
    {    
        printf("%d", iX % 10);
        for (iY = 0; iY < HEIGHT; iY++)
        {
            printf("%c", g_Dest[iX][iY]);
        } 
        printf("|\n");
    }

    return;
}


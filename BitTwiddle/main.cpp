#include <stdio.h>
#include <windows.h>

UINT Swap(UINT x, INT p, INT n)
{ 
    return (x >> (p+1-n)) & ~((~0) << n);
}

UINT Swap2(UINT x, INT p, INT n)
{ 
    UINT uiFirst = (x >> (p+1-n));
    printf("Swap2::uiFirst = 0x%x\n",uiFirst);
    UINT uiSecond = ~((~0) << n);
    printf("Swap2::uiSecond = 0x%x\n",uiSecond);


    return uiFirst & uiSecond;
}

UINT MoveValues(UINT x, INT p, INT n, UINT uiNew)
{
    if (0>(p+1-n))
    {
        printf("Error\n");
        return 0;
    }
    if (32 == n)
    {
        return x;
    }

    UINT uiMask = Swap(x, p, n);
    UINT uiRemove = Swap(~0,p,n);

    uiRemove = uiRemove << (p+1-n);
    uiMask = uiMask << (p+1-n);

    return (uiNew & ~uiRemove) | uiMask;
}

UINT MoveValues2(UINT x, INT p, INT n, UINT uiNew)
{
    if (0>(p+1-n))
    {
        printf("Error\n");
        return 0;
    }

    if (32 == n)
    {
        return x;
    }
    UINT uiMask = Swap(x, p, n);
    UINT uiRemove = Swap(~0,p,n);

    //printf("MoveValues2::uiMask == 0x%x\n", uiMask);
    //printf("MoveValues2::uiRemove == 0x%x\n", uiRemove);

    //printf("MoveValues2::SwapTest == 0x%x\n",(x >> (p+1-n)) );
    //printf("MoveValues2::Negate   == 0x%x\n",~(~0<<n));

    uiRemove = uiRemove << (p+1-n);
    uiMask = uiMask << (p+1-n);
    //printf("MoveValues2::uiMask == 0x%x\n", uiMask);
    //printf("MoveValues2::uiRemove == 0x%x\n", uiRemove);

    return (uiNew & ~uiRemove) | uiMask;
}

UINT Condense(UINT x, INT p, INT n, UINT uiNew)
{
    if (0>(p+1-n))
    {
        printf("Error\n");
        return 0;
    }
    if (32 == n)
    {
        return x;
    }
    return (uiNew & ~(((~0 >> (p+1-n)) & ~(~0 << n)) << (p+1-n))) | (((x >> (p+1-n)) & ~(~0 << n)) << (p+1-n));
}
UINT Condense2(UINT x, INT p, INT n, UINT uiNew)
{
    if (0>(p+1-n))
    {
        printf("Error\n");
        return 0;
    }
    if (32 == n)
    {
        return x;
    }

    return (uiNew & ~(((~0 >> (p+1-n)) & ~(~0 << n)) << (p+1-n))) | (((x >> (p+1-n)) & ~(~0 << n)) << (p+1-n));
}

UINT Condense3(UINT x, INT p, INT n, UINT uiNew)
{
    if (0>(p+1-n))
    {
        printf("Error\n");
        return 0;
    }
    if (32 == n)
    {
        return x;
    }

    return (uiNew & ~(((~0 >> (p+1-n)) & ~(~0 << n)) << (p+1-n))) | (((x >> (p+1-n)) & ~(~0 << n)) << (p+1-n));
}
int main()
{
    UINT uiSend = 0xDEADBEEF;
    UINT uiRet = Swap(uiSend, 23,8);
    printf("Value of 0x%x uiSend is 0x%x\n", uiSend,uiRet);
    uiRet = Swap(uiSend, 31,32);
    printf("Value of 0x%x uiSend is 0x%x\n", uiSend,uiRet);

    printf("uiRet shifted = %x\n", uiRet << (23+1-8));

    //uiRet = MoveValues(uiSend,19,8,0xFFFFFFFF); 
    uiRet = MoveValues(0xFFFFFFFF,15,8,uiSend); 
    printf("uiRet M is 0x%x after the move\n", uiRet);
    uiRet = Condense(0xFFFFFFFF,15,8,uiSend); 
    printf("uiRet 1 is 0x%x after the move\n", uiRet);
    uiRet = Condense2(0xFFFFFFFF,15,8,uiSend); 
    printf("uiRet 2 is 0x%x after the move\n", uiRet);
    uiRet = Condense3(0xFFFFFFFF,15,8,uiSend); 
    printf("uiRet 3 is 0x%x after the move\n\n", uiRet);

    uiRet = MoveValues(0xFFFFFFFF,8,8,0xDEADBEE0); 
    printf("uiRet M is 0x%x after the move\n", uiRet);
    uiRet = Condense(0xFFFFFFFF,8,8,0xDEADBEE0); 
    printf("uiRet 1 is 0x%x after the move\n", uiRet);
    uiRet = Condense2(0xFFFFFFFF,8,8,0xDEADBEE0); 
    printf("uiRet 2 is 0x%x after the move\n", uiRet);
    uiRet = Condense3(0xFFFFFFFF,8,8,0xDEADBEE0); 
    printf("uiRet 3 is 0x%x after the move\n\n", uiRet);

    uiRet = MoveValues(0x01,1,2,0xf0); 
    printf("uiRet M is %x after the move\n", uiRet);
    uiRet = Condense(0x01,1,2,0xf0); 
    printf("uiRet 1 is %x after the move\n", uiRet);
    uiRet = Condense2(0x01,1,2,0xf0); 
    printf("uiRet 2 is %x after the move\n", uiRet);
    uiRet = Condense3(0x01,1,2,0xf0); 
    printf("uiRet 3 is %x after the move\n\n", uiRet);

    uiRet = MoveValues(0x000FF000,19,8,0x10000001); 
    printf("uiRet M is %x after the move\n", uiRet);
    uiRet = Condense(0x000FF000,19,8,0x10000001); 
    printf("uiRet 1 is %x after the move\n", uiRet);
    uiRet = Condense2(0x000FF000,19,8,0x10000001); 
    printf("uiRet 2 is %x after the move\n", uiRet);
    uiRet = Condense3(0x000FF000,19,8,0x10000001); 
    printf("uiRet 3 is %x after the move\n\n", uiRet);

    uiRet = MoveValues(0xFFFFFFFF,19,8,0x0); 
    printf("uiRet M is %x after the move\n", uiRet);
    uiRet = Condense(0xFFFFFFFF,19,8,0x0); 
    printf("uiRet 1 is %x after the move\n", uiRet);
    uiRet = Condense2(0xFFFFFFFF,19,8,0x0); 
    printf("uiRet 2 is %x after the move\n", uiRet);
    uiRet = Condense3(0xFFFFFFFF,19,8,0x0); 
    printf("uiRet 3 is %x after the move\n\n", uiRet);

    uiRet = MoveValues(0x0,19,8,0xFFFFFFFF); 
    printf("uiRet M is %x after the move\n", uiRet);
    uiRet = Condense(0x0,19,8,0xFFFFFFFF); 
    printf("uiRet 1 is %x after the move\n", uiRet);
    uiRet = Condense2(0x0,19,8,0xFFFFFFFF); 
    printf("uiRet 2 is %x after the move\n", uiRet);
    uiRet = Condense3(0x0,19,8,0xFFFFFFFF); 
    printf("uiRet 3 is %x after the move\n\n", uiRet);

    uiRet = MoveValues(1,0,1,12700); 
    printf("uiRet M is %d after the move\n", uiRet);
    uiRet = Condense(1,0,1,12700); 
    printf("uiRet 1 is %d after the move\n", uiRet);
    uiRet = Condense2(1,0,1,12700); 
    printf("uiRet 2 is %d after the move\n", uiRet);
    uiRet = Condense3(1,0,1,12700); 
    printf("uiRet 3 is %d after the move\n\n", uiRet);

    printf("0xabcd1234,31,32,0\n"); 
    uiRet = MoveValues(0xabcd1234,31,32,0); 
    printf("uiRet M is %x after the move\n", uiRet);
    uiRet = MoveValues2(0xabcd1234,31,32,0); 
    printf("uiRet M2 is %x after the move\n", uiRet);
    uiRet = Condense(0xabcd1234,31,32,0); 
    printf("uiRet 1 is %x after the move\n", uiRet);
    uiRet = Condense2(0xabcd1234,31,32,0); 
    printf("uiRet 2 is %x after the move\n", uiRet);
    uiRet = Condense3(0xabcd1234,31,32,0); 
    printf("uiRet 3 is %x after the move\n\n", uiRet);

    printf("0,31,32,0xabcd1234\n"); 
    uiRet = MoveValues(0,31,32,0xabcd1234); 
    printf("uiRet M is %x after the move\n", uiRet);
    uiRet = MoveValues2(0,31,32,0xabcd1234); 
    printf("uiRet M2 is %x after the move\n", uiRet);
    uiRet = Condense(0,31,32,0xabcd1234); 
    printf("uiRet 1 is %x after the move\n", uiRet);
    uiRet = Condense2(0,31,32,0xabcd1234); 
    printf("uiRet 2 is %x after the move\n", uiRet);
    uiRet = Condense3(0,31,32,0xabcd1234); 
    printf("uiRet 3 is %x after the move\n\n", uiRet);

    uiRet = MoveValues(0xabcd1234,31,4,0); 
    printf("uiRet M is %x after the move\n", uiRet);
    uiRet = MoveValues2(0xabcd1234,31,4,0); 
    printf("uiRet M2 is %x after the move\n", uiRet);
    uiRet = Condense(0xabcd1234,31,4,0); 
    printf("uiRet 1 is %x after the move\n", uiRet);
    uiRet = Condense2(0xabcd1234,31,4,0); 
    printf("uiRet 2 is %x after the move\n", uiRet);
    uiRet = Condense3(0xabcd1234,31,4,0); 
    printf("uiRet 3 is %x after the move\n\n", uiRet);

    uiRet = MoveValues(0xabcd1231,31,31,0); 
    printf("uiRet M is %x after the move\n", uiRet);
    uiRet = MoveValues2(0xabcd1231,31,31,0); 
    printf("uiRet M2 is %x after the move\n", uiRet);
    uiRet = Condense(0xabcd1231,31,31,0); 
    printf("uiRet 1 is %x after the move\n", uiRet);
    uiRet = Condense2(0xabcd1231,31,31,0); 
    printf("uiRet 2 is %x after the move\n", uiRet);
    uiRet = Condense3(0xabcd1231,31,31,0); 
    printf("uiRet 3 is %x after the move\n\n", uiRet);

    uiRet = MoveValues(0xabcd1231,31,0,0); 
    printf("uiRet M is %x after the move\n", uiRet);
    uiRet = MoveValues2(0xabcd1231,31,0,0); 
    printf("uiRet M2 is %x after the move\n", uiRet);
    uiRet = Condense(0xabcd1231,31,0,0); 
    printf("uiRet 1 is %x after the move\n", uiRet);
    uiRet = Condense2(0xabcd1231,31,0,0); 
    printf("uiRet 2 is %x after the move\n", uiRet);
    uiRet = Condense3(0xabcd1231,31,0,0); 
    printf("uiRet 3 is %x after the move\n\n", uiRet);

    UINT uiTest = 0xDEADBEEF;
    printf("Test 0x%x\n",Swap2(uiTest,31,32));
    printf("Test 0x%x\n",Swap2(uiTest,31,31));
    printf("Test 0x%x\n",Swap2(uiTest,30,31));
    printf("Test 0x%x\n",Swap2(uiTest,31,63));
    printf("Test 0x%x\n",Swap2(uiTest,31,4));
    printf("Test 0x%x\n",uiTest << 32);
    printf("Test 0x%x\n",uiTest << 31);

}

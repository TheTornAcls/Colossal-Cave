#include <stdio.h>

struct Point
{
    int x;
    int y;
};

bool Search(Point *Source, unsigned int length, int *ptrMaxX, int *ptrMaxY, int *ptrMinX, int *ptrMinY)
{
    // Check if Source is NULL
    if (Source == NULL)
        return false;

    int maxX = Source[0].x;
    int maxY = Source[0].y;
    int minX = Source[0].x;
    int minY = Source[0].y;

    for (unsigned int count = 1; count < length; count++)
    {
        if (Source[count].x > maxX)
        {
            maxX = Source[count].x;
        }
        if (Source[count].y > maxY)
        {
            maxY = Source[count].y;
        }
        if (Source[count].x < minX)
        {
            minX = Source[count].x;
        }
        if (Source[count].y < minY)
        {
            minY = Source[count].y;
        }
    }
    if (NULL != ptrMaxX)
    {
        *ptrMaxX = maxX;
    }
    if (NULL != ptrMaxY)
    {
        *ptrMaxY = maxY;
    }
    if (NULL != ptrMinX)
    {
        *ptrMinX = minX;
    }
    if (NULL != ptrMinY)
    {
        *ptrMinY = minY;
    }
    return true;
}

int TopMostHeight(Point * Source, unsigned int length)
{
    int maxY;
    Search(Source, length, NULL, &maxY, NULL, NULL);
    return maxY;
}

int BottomMostHeight(Point * Source, unsigned int length)
{
    int minY;
    Search(Source, length, NULL, NULL, NULL, &minY);
    return minY;
}

int LeftMostWidth(Point * Source, unsigned int length)
{
    int minY;
    Search(Source, length, NULL, NULL, &minY, NULL);
    return minY;
}

int RightMostWidth(Point * Source, unsigned int length)
{
    int maxX;
    Search(Source, length, &maxX, NULL, NULL, NULL);
    return maxX;
}

int main(void)
{
    Point testPoint[12] = { {1,2}, {5, 6}, {-1,5}, {8,0} };
    int right = RightMostWidth(testPoint, 4);
    printf("Right Most is %d\n", right);
    right = LeftMostWidth(testPoint, 4);
    printf("Left Most is %d\n", right);
    right = BottomMostHeight(testPoint, 4);
    printf("Bottom Most is %d\n", right);
    int Top = TopMostHeight(testPoint, 4);
    printf("Top Most is %d\n", Top);
    return 0;
}
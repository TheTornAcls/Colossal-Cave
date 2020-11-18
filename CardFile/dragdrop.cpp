#include "cardfile.h"
#include <shellapi.h>
/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

#define	CBPATHMAX	64

char    szPackageClass[] = "Package";

void DoDragDrop(HWND hwnd, HDROP hdrop, BOOL fCard) 
{
    char szDropFile[CBPATHMAX+1];      /* Path */
    int fLink = FALSE;

    /* Retrieve the file name */
    DragQueryFile(hdrop, 0, szDropFile, CBPATHMAX);
    DragFinish(hdrop);

    /* We got dropped on, so bring ourselves to the top */
    BringWindowToTop(hIndexWnd);

    if (!fCard) /* did not get dropped on the card, open the file */
    {
        if (!MaybeSaveFile(FALSE))
            return;
        OpenNewFile(szDropFile);
        return;
    }

    /* 
     * Ctrl+Shift, no Alt   => link the object 
     * no modifiers         => embed the object
     * anything else        => NOP
     */
    if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0) &&
        !(GetKeyState(VK_MENU) < 0))
        fLink = TRUE;
    else if (!(GetKeyState(VK_SHIFT) < 0) && !(GetKeyState(VK_CONTROL) < 0) &&
        !(GetKeyState(VK_MENU) < 0))
        fLink = FALSE;
    else
        return;
       
    if (fOLE && fCard) 
        PicCreateFromFile(szPackageClass, szDropFile, fLink);
}


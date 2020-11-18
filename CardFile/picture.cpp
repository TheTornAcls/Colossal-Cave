#include "cardfile.h"

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

/* This file contains all the picture operations.  It virtualizes the
 * interface so we deal with pictures instead of old Bitmaps and objects.
 *
 * If the picture is a Linked or Embedded object, the OLE client DLL 
 * will be called.  Otherwise, we will process the data as we did for
 * Windows 3.0.
 *
 * This file is in the _FILE segment, which is why 80% of the calls are FAR.
 */

#define OBJNAMEMAX      10          /* Length of IDS_OBJNAME w/o the %d */

BYTE         *vpfbmp;
DWORD        vcbLeftToRead;
DWORD        vcLeft;
FAKEBITMAP   vfbmp;
HCURSOR      hcurOle = NULL;
HWND         hwndError;
LPCARDSTREAM lpStream = NULL;
LPOLECLIENT  lpclient = NULL;
LPOLEOBJECT  lpObjectUndo = NULL;
OBJECTTYPE   otObjectUndo = NULL;
WORD         cWait = 0;
char         szClip[] = "Clipboard";
char         szUndo[] = "Undo";
char         szObjectName[OBJNAMEMAX + 10];
char         szObjFormat[OBJNAMEMAX];

NOEXPORT void  NEAR SetFile(int fh);
DWORD (*pfOldRead)  (LPOLESTREAM, LPSTR, DWORD);
DWORD (*pfNewRead)  (LPOLESTREAM, LPSTR, DWORD);

void PicDelete(PCARD pCard) {
    if (pCard->lpObject)
    {
        if (HIWORD(pCard->lpObject))
        {
            WaitForObject(pCard->lpObject);
            /* OleRelease() because the object is still
             * part of the Card File.
             */
            switch (OleError(OleRelease(pCard->lpObject)))
            {
            case FOLEERROR_NOTGIVEN:
                ErrorMessage(E_FAILED_TO_DELETE_OBJECT);
                break;
#if 0
            case FOLEERROR_GIVEN:
                break;
#endif
            case FOLEERROR_OK:
                InvalidateRect(hEditWnd, NULL, TRUE);
                break;
            }
        }
        else
            DeleteObject((HBITMAP)LOWORD(pCard->lpObject));
    }
    pCard->lpObject = NULL;
}

/* SIDE EFFECT:  Advances the file pointer past the object */
BOOL PicRead( PCARD pCard, 
              HANDLE fh, 
              BOOL fOld) 
{
    int bmSize;
    int cxBitmap;
    int cyBitmap;
    int cEighths;
    OLESTATUS olestat;
    LPOLEOBJECT lpObject = NULL;
    DWORD idObject;

    myread(fh, &bmSize, sizeof(int));

    /* Does the card contain a picture? */
    if (!bmSize)
    {        /* No... */
        pCard->lpObject = NULL;
        return TRUE;        /* Succeeded in reading 0 size object */
    }

    if (fOLE)
    {        /* Reading a new file... */
        unsigned long fhLoc;

        SetFile(fh);

        if (fOld)
        {        /* Set up to read from old format card */
            lpStream->lpstbl->Get = pfOldRead;
            vcbLeftToRead = ((WORD)bmSize);
        }
        else
        {
            myread(fh, &idObject, sizeof(DWORD));
        }

        /* Save current position of file pointer */
        if ((fhLoc = _llseek(fh, 0L, 1)) == -1)
            return FALSE;

        /* Synchronously:  Load the new object */
        wsprintf(szObjectName, szObjFormat, idObject + 1);
        olestat = OleLoadFromStream((LPOLESTREAM)lpStream, szPStdFile,
                                    lpclient, lhcdoc, szObjectName, &lpObject);

        if (olestat == OLE_WAIT_FOR_RELEASE)
        {
            cOleWait++;
            WaitForObject(lpObject);
            olestat = oleloadstat;
        }

        if (OleError(olestat))
        {
            /* Reset file pointer, and try again */
            vcbLeftToRead = ((WORD)bmSize);
            SetFile(fh);
            if (_llseek(fh, fhLoc, 0) == -1)
            {
                return FALSE;
            }

            /* Synchronously:  Read it with the "Static" protocol */
            olestat = OleLoadFromStream((LPOLESTREAM)lpStream, szPStatic,
                                        lpclient, lhcdoc, szObjectName, &lpObject);

            if (olestat == OLE_WAIT_FOR_RELEASE)
            {
                cOleWait++;
                WaitForObject(lpObject);
                olestat = oleloadstat;
            }

            if (OleError(olestat))
            {
                lpStream->lpstbl->Get = pfNewRead;          /* Restore */
                return FALSE;
            }
        }
        pCard->lpObject = lpObject;

        if (fOld)
        {
            /* Side effect:  pCard->rcObject has already been filled in */
            pCard->otObject = STATIC;
            _llseek(fh, vcbLeftToRead, 1);    /* For Video 7 bug */
            lpStream->lpstbl->Get = pfNewRead;  /* Restore so new is fastest */
            pCard->idObject = idObjectMax++;
        }
        else
        {
            int cx;
            int cy;

            /* Read the character size when saving, and scale to the
             * current character size.
             */
            mylread(fh, &cx, sizeof(int));
            mylread(fh, &cy, sizeof(int));
            mylread(fh, &(pCard->rcObject), sizeof(RECT));

            /* Character width differs, scale in the x direction */
            if (cx != CharFixWidth)
            {
                pCard->rcObject.left  = Scale(pCard->rcObject.left, CharFixWidth, cx);
                pCard->rcObject.right = Scale(pCard->rcObject.right, CharFixWidth, cx);
            }

            /* Character height differs, scale in the y direction */
            if (cy != CharFixHeight)
            {
                pCard->rcObject.top    = Scale(pCard->rcObject.top, CharFixHeight, cy);
                pCard->rcObject.bottom = Scale(pCard->rcObject.bottom, CharFixHeight, cy);
            }

            /* Retrieve the object type */
            mylread(fh, &(pCard->otObject), sizeof(OBJECTTYPE));
            pCard->idObject = idObject;
        }
    }
    else
    {
        HANDLE hBits;
        LPSTR  lpBits;

        if (!fOld)
        {         /* No OLE, can't read new objects! */
            pCard->lpObject = NULL;
            ErrorMessage(E_NEW_FILE_NOT_READABLE);
            return FALSE;
        }

        /* Read in phony monochrome BITMAP header */
        myread(fh, &cxBitmap, sizeof(int));
        myread(fh, &cyBitmap, sizeof(int));

        myread(fh, &cEighths, sizeof(int));
        pCard->rcObject.left = (cEighths * CharFixWidth) / 8;
        pCard->rcObject.right = pCard->rcObject.left + cxBitmap - 1;

        myread(fh, &cEighths, sizeof(int));
        pCard->rcObject.top = (cEighths * CharFixHeight) / 8;
        pCard->rcObject.bottom = pCard->rcObject.top + cyBitmap - 1;
        pCard->idObject = idObjectMax++;

        /* Read in the BITMAP bits */
        if (hBits = GlobalAlloc(GHND, (WORD)bmSize))
        {
            if (lpBits = (LPSTR)GlobalLock(hBits))
            {
                mylread(fh, lpBits, bmSize);
                /* Make the selector zero */
                pCard->lpObject = (LPOLEOBJECT) MAKELONG(
                                                        CreateBitmap(cxBitmap, cyBitmap, 1, 1, (LPSTR)lpBits), 0);
                GlobalUnlock(hBits);
            }
            GlobalFree(hBits);
        }
        else              /* Skip past the object */
            _llseek(fh, (unsigned long)bmSize, 1);
    }
    return TRUE;
}

/* If fForceOld is specified, or the OLE library isn't loaded,
 * PicWrite() will write new format files.
 */

/* SIDE EFFECT:  moves pointer to end of written object */
BOOL PicWrite(PCARD pCard, int fh, BOOL fForceOld) {
    HANDLE  hBitmap = NULL;
    int        bmSize;
    int     bmWidth;
    int     bmHeight;
    int     cEighths;
    HANDLE  hBits = NULL;
    LPSTR   lpBits = NULL;

    if (fOLE)
    {
        if (fForceOld)
        {
            /* Convert picture to monochrome bitmap */
            hBitmap = MakeObjectCopy(pCard);
            goto OldWrite;
        }
        else
        {
            /* Write a BOOL so that cardfiles can be compared */
            bmSize = !!pCard->lpObject;

            if (mywrite(fh, &bmSize, sizeof(int)) < sizeof(int))
                goto Disk_Full;

            if (bmSize)
            {
                SetFile(fh);
                if (mylwrite(fh, &(pCard->idObject), sizeof(DWORD)) < sizeof(DWORD))
                    goto Disk_Full;

                if (S_OK != OleSaveToStream(pCard->lpObject, (LPOLESTREAM)lpStream))
                    goto Disk_Full;

                /* Write current character size, rectangle, and object type.
                 */
                if (mylwrite(fh, &CharFixWidth, sizeof(int)) < sizeof(int)
                    || mylwrite(fh, &CharFixHeight, sizeof(int)) < sizeof(int)
                    || mylwrite(fh, &(pCard->rcObject), sizeof(RECT)) < sizeof(RECT)
                    || mylwrite(fh, &(pCard->otObject), 
                                sizeof(OBJECTTYPE)) < sizeof(OBJECTTYPE))
                {
                    goto Disk_Full;
                }
            }
        }
    }
    else
    {            /* Can only write the old format (sigh) */
        OldWrite:
        if (!hBitmap)
            hBitmap = (HBITMAP)LOWORD(pCard->lpObject);

        if (hBitmap)
        {
            /* Calculate the BITMAP dimensions */
            bmWidth     = (pCard->rcObject.right - pCard->rcObject.left) + 1;
            bmHeight    = (pCard->rcObject.bottom - pCard->rcObject.top) + 1;
            bmSize      = (((bmWidth + 0x000f) >> 4) << 1) * bmHeight;

            if ((hBits = GlobalAlloc(GHND, (WORD)bmSize))
                && (lpBits = (LPSTR)GlobalLock(hBits)))
            {
                /* Write out the size, width, height */
                if (mywrite(fh, &bmSize, sizeof(int)) < sizeof(int) || 
                    mywrite(fh, &bmWidth, sizeof(int)) < sizeof(int) ||
                    mywrite(fh, &bmHeight, sizeof(int)) < sizeof(int))
                    goto Disk_Full;

                /* Write out the x and y positions */
                cEighths = (pCard->rcObject.left * 8) / CharFixWidth;
                if (mywrite(fh, &cEighths, sizeof(int)) < sizeof(int))
                    goto Disk_Full;

                cEighths = (pCard->rcObject.top * 8) / CharFixHeight;
                if (mywrite(fh, &cEighths, sizeof(int)) < sizeof(int))
                    goto Disk_Full;

                /* Write out the actual BITMAP bits */
                GetBitmapBits(hBitmap, (unsigned long)bmSize, lpBits);
                if (mylwrite(fh, lpBits, bmSize) < bmSize)
                    goto Disk_Full;
                GlobalUnlock(hBits);
                GlobalFree(hBits);
            }
            else
            {
                Disk_Full:
                if (lpBits)
                    GlobalUnlock(hBits);

                if (hBits)
                    GlobalFree(hBits);
                return FALSE;
            }
        }
        else
        {
            int zero = 0;

            if (mywrite(fh, &zero, sizeof(int)) < sizeof(int))
                goto Disk_Full;
        }
    }
    return TRUE;
}

BOOL PicDraw(PCARD pCard, HDC hDC, BOOL fAtOrigin) {
    BOOL   bSuccess;
    HANDLE hOldObject;
    HDC       hMemoryDC;

    /* If we have an object, call OleDraw() */
    if (HIWORD(pCard->lpObject))
    {
        HRGN    hrgn;
        RECT    rc, rcClip;

        rc = pCard->rcObject;
        if (fAtOrigin)        /* Move rect to (0,0) */
            OffsetRect(&rc, -rc.left, -rc.top);

        /* Force OleDraw to draw within the clipping region */
        if (hrgn = CreateRectRgnIndirect(&rc))
            SelectObject(hDC, hrgn);

/* Bug 11290: Don't draw outside the edit window's client area
 *  11 January 1992         Clark R. Cyr
 */
        GetClientRect(hEditWnd, &rcClip);
        IntersectClipRect(hDC, rcClip.left, rcClip.top,
                          rcClip.right, rcClip.bottom);

        /* Draw the object */
        bSuccess = (S_OK == OleDraw(pCard->lpObject, hDC, (LPRECT)&rc, NULL, NULL));

        /* If we had a clipping region, delete it */
        if (hrgn)
            DeleteObject(hrgn);

    }
    else
    {        /* If we have a BITMAP, BitBlt it into the DC */
        if (pCard->lpObject && (hMemoryDC = CreateCompatibleDC(hDC)))
        {
            int cxBitmap, cyBitmap;

            cxBitmap = (pCard->rcObject.right - pCard->rcObject.left) + 1,
                       cyBitmap = (pCard->rcObject.bottom - pCard->rcObject.top) + 1,

                                  hOldObject = SelectObject(hMemoryDC, (HBITMAP)LOWORD(pCard->lpObject));
            BitBlt(hDC, pCard->rcObject.left, pCard->rcObject.top,
                   cxBitmap, cyBitmap, hMemoryDC, 0, 0, SRCAND);
            SelectObject(hMemoryDC, hOldObject);
            DeleteDC(hMemoryDC);
            bSuccess = TRUE;
        }
    }
    return bSuccess;
}

void PicCutCopy(PCARD pCard, BOOL fCut) 
{
    /* If no object or we can't open the clipboard, fail */
    if (!pCard->lpObject || !OpenClipboard(hIndexWnd))
        return;

    EmptyClipboard();

    Hourglass(TRUE);
    if (HIWORD(pCard->lpObject))
    {      /* Putting an object on the clipboard */
        if (S_OK != OleCopyToClipboard(pCard->lpObject))
            IndexOkError(EINSMEMORY);
    }
    else if (pCard->lpObject)
    {        /* Old BITMAP */
        BITMAP bm;
        HBITMAP hBitmap;

        GetObject((HBITMAP)LOWORD(pCard->lpObject), sizeof(BITMAP), (LPSTR)&bm);
        if (!(hBitmap = MakeBitmapCopy((HBITMAP)LOWORD(pCard->lpObject), &bm)))
            IndexOkError(EINSMEMORY);
        else
            SetClipboardData(CF_BITMAP, hBitmap);
    }
    CloseClipboard();

    if (fCut && pCard->lpObject)
    {         /* Delete the object */
        /* If Undo object exists, delete it */
        DeleteUndoObject();

        /* Instead of deleting then cloning, just save the object */
        lpObjectUndo = pCard->lpObject;
        otObjectUndo = pCard->otObject;
        OleRename(lpObjectUndo, szUndo);
        pCard->lpObject = NULL;

        InvalidateRect(hEditWnd, (LPRECT)&(pCard->rcObject), TRUE);
        CurCardHead.flags |= FDIRTY;
    }
    Hourglass(FALSE);
}

/* 
 * Paste/PasteLink an Object 
 *
 * pCard  - Card where the object should be pasted,
 * fPaste - specifies Paste/PasteLink
 * ClipFormat - Should be NULL for Edit.Paste, Edit.PasteLink
 *            - will contain a specific Clipbrd format when user chooses
 *              Paste/PasteLink in Edit.PasteSpecial dlg.
 * Parameters for various calls:
 * 
 * On Edit.Paste - PicPaste(pCard, TRUE, 0);
 * On Edit.PasteLink - PicPaste(pCard, FALSE, 0);
 * On Paste/PasteLink from PasteSpecial Dlg, 
 *     if (class name chosen from the Objects list)
 *         Similar to Edit.Paste/PasteLink.
 *     else if (specific format chosen)
 *     {
 *         PicPaste(pCard, TRUE, Format); 
 *         (Paste a static object in given format)
 *         OR 
 *         PasteLink(pCard, FALSE, Format);
 */
void PicPaste(
             PCARD pCard, 
             BOOL fPaste,    /* Paste/PasteLink */
             WORD ClipFormat) /* For Paste from PasteSpecial Dlg, may specify a format */
{
    LONG objType;
    LPOLEOBJECT lpObject;
    OBJECTTYPE otObject;
    bool fError;

    if (!OpenClipboard(hIndexWnd))
    {
        return;                /* Couldn't open the clipboard */
    }

    Hourglass(TRUE);
    if (fOLE)
    {
        /* Don't replace the current object unless we're successful */
        wsprintf(szObjectName, szObjFormat, idObjectMax + 1);
        if (fPaste)
        {
            if (ClipFormat) /* Paste a specific format (from PasteSpecial Dlg) */
            {
                fError = OleError(OleCreateFromClip(szPStatic, lpclient, lhcdoc, szObjectName,
                                                    &lpObject, olerender_format, ClipFormat));
            }
            else        /* Paste from Edit.Paste */
            {
                /* Try StdFileEditing protocol */
                fError = OleError(OleCreateFromClip(szPStdFile, lpclient, lhcdoc, szObjectName,
                                                    &lpObject, olerender_draw, 0));
                /* If unsuccessful, try Static protocol */
                if (fError)
                {
                    fError = OleError(OleCreateFromClip(szPStatic, lpclient, lhcdoc, szObjectName,
                                                        &lpObject, olerender_draw, 0));
                }
            }
        }
        else
        {
            /* create a link, in response to Edit.PasteLink or 
             * PasteLink from PasteSpecial dlg 
             */
            if (ClipFormat) /* PasteLink a specific format (from PasteSpecial Dlg) */
            {
                fError = OleError(OleCreateLinkFromClip(szPStdFile, lpclient, lhcdoc, szObjectName,
                                                        &lpObject,  olerender_format, ClipFormat));
            }
            else    /* PasteLink an object */
            {
                fError = OleError(OleCreateLinkFromClip(szPStdFile, lpclient, lhcdoc, szObjectName,
                                                        &lpObject,  olerender_draw, 0));
            }
        }
        if (fError)
        {
            lpObject = NULL;
            if (fError == FOLEERROR_NOTGIVEN)
                ErrorMessage(E_GET_FROM_CLIPBOARD_FAILED);
        }
        else
        {
            /* Figure out what kind of object we have, OleCreateFromLink
             * can create an embedded object */
            OleQueryType(lpObject, &objType);
            switch (objType)
            {
            case OT_EMBEDDED:   otObject = EMBEDDED;    break;
            case OT_LINK:       otObject = LINK;        break;
            default:            otObject = STATIC;      break;
            }
        }
        if (lpObject)
        {
            if (pCard->lpObject)
                PicDelete(pCard);
            pCard->lpObject = lpObject;
            pCard->otObject = otObject;
            pCard->idObject = idObjectMax++;
            DoSetHostNames(CurCard.lpObject, CurCard.otObject);
            InvalidateRect(hEditWnd, NULL, TRUE);
            SetRect(&(pCard->rcObject), 0, 0, 0, 0);
            CurCardHead.flags |= FDIRTY;
        }
    }
    else
    {            /* Create an old object the hard way */
        HBITMAP hBitmap;
        BITMAP bm;

        if (hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP))
        {
            GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
            if (!(hBitmap = MakeBitmapCopy(hBitmap, &bm)))
                IndexOkError(EINSMEMORY);
            else
            {
                if (pCard->lpObject)
                    PicDelete(pCard);

                /* Make the selector zero */
                pCard->lpObject = (LPOLEOBJECT)MAKELONG(hBitmap, 0);
                SetRect(&(pCard->rcObject), 0, 0, bm.bmWidth-1, bm.bmHeight-1);
                InvalidateRect(hEditWnd, (LPRECT)&(pCard->rcObject), TRUE);
                CurCardHead.flags |= FDIRTY;
            }
        }
    }
    CloseClipboard();
    Hourglass(FALSE);
}

HBITMAP MakeObjectCopy(PCARD pCard) {
    HBITMAP hBitmap = NULL;
    HDC hDCDest = NULL;
    HDC hDC;
    int cxBitmap;
    int cyBitmap;
    BOOL fError = TRUE;
    HANDLE hObject;
    BITMAP bm;

    /* LOWORD(lpObject) is NULL only if no bitmap in 3.0 file */
    if (pCard->lpObject == NULL)
        return NULL;

    /* First, try to load a normal BITMAP */
    if (HIWORD(pCard->lpObject))
    {
        if (S_OK != OleGetData(pCard->lpObject, CF_BITMAP, &hBitmap))
            hBitmap = NULL;
    }
    else
    {
        hBitmap = (HBITMAP)LOWORD(pCard->lpObject);
    }

    /* got a bitmap, either this is a bitmap in OLE object OR
       it is a bitmap in 3.0 card file.
       return a copy of the bitmap */
    if (hBitmap)
    {
        GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
        return MakeBitmapCopy(hBitmap, &bm);
    }

    /* Ole Object is not a bitmap. May be a metafile or s'thing else. 
       Draw it into a mono DC and return a handle to this bitmap */
    Hourglass(TRUE);

    /* If we don't succeed, draw the picture into a monochrome DC */
    hDC = GetDC(hIndexWnd);
    hDCDest = CreateCompatibleDC(hDC);
    ReleaseDC(hIndexWnd, hDC);
    if (!hDCDest)
        goto MakeObjectCopyEnd;

    /* Create a new monochrome BITMAP */
    cxBitmap = (pCard->rcObject.right - pCard->rcObject.left);
    cyBitmap = (pCard->rcObject.bottom - pCard->rcObject.top);
    if (!(hBitmap = CreateBitmap(cxBitmap, cyBitmap, 1, 1, (LPSTR)NULL)))
        goto MakeObjectCopyEnd;

    SetWindowOrg(hDCDest, 0, 0);
    SetWindowExt(hDCDest, cxBitmap, cyBitmap);

    /* Draw into the DC */
    if (hObject = SelectObject(hDCDest, hBitmap))
    {
        RECT    rc;

        /* Start by clearing the DC (white's enough 'cause it's monochrome) */
        rc = pCard->rcObject;
        OffsetRect(&rc, -rc.left, -rc.top);
        FillRect(hDCDest, &rc, (HBRUSH)(COLOR_WINDOW+1));

        /* Draw the picture */
        fError = !PicDraw(pCard, hDCDest, TRUE);
        SelectObject(hDCDest, hObject);                 /* Needed? */
    }

    MakeObjectCopyEnd:
    if (fError)
    {
        ErrorMessage(E_BITMAP_COPY_FAILED);
        if (hBitmap)
        {
            DeleteObject(hBitmap);
            hBitmap = NULL;
        }
    }
    if (hDCDest)
        DeleteDC(hDCDest);

    Hourglass(FALSE);
    return hBitmap;
}

NOEXPORT void NEAR SetFile (int fh) {
    lpStream->fh = fh;
    vfbmp.bm.bmType = 0xffff;
}

/******************* OLE STREAM Get, Put, ... routines **********************/

/*
 * This Read is used to make Old Card bitmaps read like the BITMAP
 * object in OLECLI.DLL.  We must return:
 *
 * <type signature>     0x0700
 * <type string>        "BITMAP" (NULL terminated)
 * BITMAP structure
 * <BITMAP bits>
 *
 * So, I fill in "FAKEBITMAP" and read out of it until I'm through;
 * then I just read the bitmap bits directly from the file.
 */

DWORD ReadOldStream(LPCARDSTREAM lpStream, LPSTR lpstr, DWORD cb) {
    int        i, xTmp, yTmp;
    DWORD    cRead = 0L;

    if (vfbmp.bm.bmType == 0xffff)
    {
        vfbmp.ulVersion = 1L;
        vfbmp.ulWhat = OT_STATIC;
        vfbmp.cbName = 7L;
        lstrcpy(vfbmp.szName, "BITMAP");
        vfbmp.res[0] = vfbmp.res[1] = vfbmp.res[2] = 0L;
        vfbmp.bm.bmType = 0;

        /* Read in the card dimensions */
        myread(lpStream->fh, &vfbmp.bm.bmWidth, sizeof(int));
        myread(lpStream->fh, &vfbmp.bm.bmHeight, sizeof(int));
        vfbmp.bm.bmWidthBytes = ((vfbmp.bm.bmWidth + 0x000f) >> 4) << 1;
        vfbmp.bm.bmPlanes = 1;
        vfbmp.bm.bmBitsPixel = 1;
        vfbmp.bm.bmBits = 0L;

        /* Read in the card position */
        myread(lpStream->fh, &xTmp, sizeof(int));
        xTmp = (xTmp * CharFixWidth) / 8;
        myread(lpStream->fh, &yTmp, sizeof(int));
        yTmp = (yTmp * CharFixHeight) / 8;

        /* We don't scale for compatibility's sake */
        SetRect(&(CurCard.rcObject), xTmp, yTmp,
                xTmp + vfbmp.bm.bmWidth - 1, yTmp + vfbmp.bm.bmHeight - 1);
        vcLeft = (DWORD) sizeof(FAKEBITMAP);
        vpfbmp = (BYTE *)&vfbmp;
    }
    if (vcLeft)
    {
        cRead = min(vcLeft, cb);
        for (i = 0; i < (int) cRead; i++)
            *lpstr++ = *vpfbmp++;
        vcLeft -= cRead;
        cb -= cRead;
    }
    if (cb)
    {
        /* Old bitmaps are always under 64K */
        cRead += (DWORD) _lread(lpStream->fh, lpstr + cRead, (int)cb);
        vcbLeftToRead -= cRead;
    }

    return cRead;
}

DWORD ReadStream(LPCARDSTREAM lpStream, LPSTR lpstr, DWORD cb) {
    if (cb < 0x00010000)
        return _lread(lpStream->fh, lpstr, (WORD) cb);
    return lread(lpStream->fh, lpstr, cb);
}

DWORD WriteStream(LPCARDSTREAM lpStream, LPSTR lpstr, DWORD cb) {
    if (cb < 0x00010000)
        return _lwrite(lpStream->fh, lpstr, (WORD) cb);
    return lwrite(lpStream->fh, lpstr, cb);
}

/********************** Swiped from Clipbrd *******************************/
/*
 *  lread() - Good ol' _lread that will handle >64k
 */

#define MAXREAD   ((LONG)  (60L * 1024L))

DWORD lread(int hFile, void FAR *pBuffer, DWORD dwBytes)
{
    BYTE    *hpBuffer = pBuffer;
    DWORD   dwByteCount = dwBytes;

    while (dwByteCount > MAXREAD)
    {
        if (_lread(hFile, hpBuffer, (WORD)MAXREAD) != MAXREAD)
            return 0;

        dwByteCount -= MAXREAD;
        hpBuffer += MAXREAD;
    }
    if (_lread(hFile, hpBuffer, (WORD)dwByteCount) != dwByteCount)
        return 0;

    return dwBytes;
}


/*
 *  lwrite() -  Good ol' _lwrite that will handle >64k
 */

DWORD lwrite(int hFile, void FAR *pBuffer, DWORD dwBytes) 
{
    DWORD   dwByteCount    = dwBytes;
    BYTE    *hpBuffer = pBuffer;

    while (dwByteCount > MAXREAD)
    {
        if (_lwrite(hFile, (LPSTR)hpBuffer, (WORD)MAXREAD) != MAXREAD)
            return 0;
        dwByteCount -= MAXREAD;
        hpBuffer += MAXREAD;
    }

    if (_lwrite(hFile, (LPSTR)hpBuffer, (WORD)dwByteCount) != (WORD)dwByteCount)
        return 0;

    return dwBytes;
}

HBITMAP MakeBitmapCopy(HBITMAP hbmSrc, PBITMAP pBitmap) {
    HBITMAP hBitmap = NULL;
    HDC hDCSrc = NULL;
    HDC hDCDest = NULL;
    HDC hDC;
    BOOL fError = TRUE;
#ifdef DBCS
    WORD bmHeightOrg;
    WORD bmWidthOrg;
#endif

    hDC = GetDC(hIndexWnd);
    hDCSrc = CreateCompatibleDC(hDC); /* get memory dc */
    hDCDest = CreateCompatibleDC(hDC);
    ReleaseDC(hIndexWnd, hDC);
    if (!hDCSrc || !hDCDest)
        goto MakeCopyEnd;

    /* select in passed bitmap */
    if (!SelectObject(hDCSrc, hbmSrc))
        goto MakeCopyEnd;

    /* create new monochrome bitmap */
#ifdef DBCS

    /* If clipdata bitmap is beyond 64K, shrink it down to size */
    bmHeightOrg = pBitmap->bmHeight;
    bmWidthOrg = pBitmap->bmWidth;
    while ((long)mylmul(pBitmap->bmHeight,pBitmap->bmWidthBytes) > 65535L )
    {
        /* Try shrinking 10% each time until it fits with 64k */
        pBitmap->bmHeight    -= (pBitmap->bmHeight / 10);
        pBitmap->bmWidth     -= (pBitmap->bmWidth / 10);
        pBitmap->bmWidthBytes = (pBitmap->bmWidth >> 4)*2;
        if ((pBitmap->bmWidth % 16) != 0)
            pBitmap->bmWidthBytes += 2;
    }
#endif

    if (!(hBitmap = CreateBitmap(pBitmap->bmWidth, pBitmap->bmHeight, 1, 1, (LPSTR) NULL)))
        goto MakeCopyEnd;

    /* Now blt the bitmap contents.  The screen driver in the source will
       "do the right thing" in copying color to black-and-white. */

    if (!SelectObject(hDCDest, hBitmap) ||
#ifdef DBCS
        /* Use StretchBlt for shrinking image. */
        !StretchBlt(hDCDest, 0, 0, pBitmap->bmWidth, pBitmap->bmHeight,
                    hDCSrc, 0, 0, bmWidthOrg, bmHeightOrg, SRCCOPY))
    {
#else
        !BitBlt(hDCDest, 0, 0, pBitmap->bmWidth, pBitmap->bmHeight, hDCSrc, 0, 0, SRCCOPY))
    {
#endif

        DeleteObject(hBitmap);
        hBitmap = NULL;
        goto MakeCopyEnd;
    }
    fError  = FALSE;

    MakeCopyEnd:
    if (fError)
        ErrorMessage(E_BITMAP_COPY_FAILED);
    if (hDCSrc)
        DeleteObject(hDCSrc);
    if (hDCDest)
        DeleteObject(hDCDest);
    return hBitmap;
}

/* OleError() - Disposes of OLE errors.
 *
 * Note:  This function (and ErrorMessage()) both use the window
 *        hwndError as parent when putting up dialogs.
 */
WORD OleError(
             HRESULT olestat) 
{
    switch (olestat)
    {
    case OLE_WAIT_FOR_RELEASE:
        cOleWait++;

    case S_OK:
        return FOLEERROR_OK;

    case OLE_ERROR_STATIC:              /* Only happens w/ dbl click */
        ErrorMessage(W_STATIC_OBJECT);
        break;

    case OLE_ERROR_COMM:
        ErrorMessage(E_FAILED_TO_LAUNCH_SERVER);
        break;

    case OLE_ERROR_REQUEST_NATIVE:
    case OLE_ERROR_REQUEST_PICT:
    case OLE_ERROR_ADVISE_NATIVE:
    case OLE_ERROR_ADVISE_PICT:
    case OLE_ERROR_OPEN:                /* Invalid link? */
    case OLE_ERROR_NAME:
        if (CurCard.otObject == LINK)
        {
            if (hwndError == hIndexWnd)
            {
                if (DialogBox(hIndexInstance, MAKEINTRESOURCE(DTINVALIDLINK),
                              hwndError, lpfnInvalidLink) == IDD_LINK)
                    PostMessage(hIndexWnd, WM_COMMAND, LINKSDIALOG, 0L);
            }
            else
                /* Failed, but already in LinksDlg!! */
                ErrorMessage(E_FAILED_TO_UPDATE_LINK);

            return FALSE;
        }
        break;

    case OLE_BUSY:
        ErrorMessage(E_SERVER_BUSY);
        break;

    default:
        return FOLEERROR_NOTGIVEN;
        break;
    }
    return FOLEERROR_GIVEN;
}

BOOL GetNewLinkName(HWND hwndOwner, PCARD pCard) {
    BOOL    fPath    = FALSE;
    BOOL    fSuccess    = FALSE;
    BOOL    fFile = FALSE;
    DWORD   dwSize    = NULL;
    HANDLE  hData;
    HANDLE  hData2      = NULL;
    HANDLE  hData3    = NULL;
    HWND    hwndOwnSave = NULL;
    LPSTR   lpData2     = NULL;
    LPSTR   lpstrData   = NULL;
    LPSTR   lpstrFile   = NULL;
    LPSTR   lpstrLink   = NULL;
    LPSTR   lpstrPath   = NULL;
    LPSTR   lpstrTemp   = NULL;
    char    szDocDefExt[10];
    char    szDocFile[PATHMAX];
    char    szDocPath[PATHMAX];

    hwndOwnSave = OFN.hwndOwner;

    if (S_OK != OleGetData(pCard->lpObject, vcfLink, &hData) || 
        !(lpstrData = GlobalLock(hData)))
        goto Error;

    lpstrTemp = lpstrData;
    while (*lpstrTemp++)
        continue;
    lpstrPath = lpstrFile = lpstrTemp;
    while (*(lpstrTemp = AnsiNext(lpstrTemp)))
        if (*lpstrTemp == '\\')
            lpstrFile = lpstrTemp + 1;

    lstrcpy(szDocFile, lpstrFile);
    fPath = (*(lpstrFile - 1));
    *(lpstrFile - 1) = '\0';
    lstrcpy(szDocPath, ((lpstrPath != lpstrFile) ? lpstrPath : ""));

    /* Fix bug:  If we have <drive>:\<filename>, the starting directory
     * should be <drive>:\, not just <drive>:.
     */
    if (lstrlen(szDocPath) == 2)
        lstrcat(szDocPath, "\\");

    if (fPath)
        *(lpstrFile - 1) = '\\';

    while (*lpstrFile != '.' && *lpstrFile)
        lpstrFile++;

    /* Make a filter that respects the link's class name */
    OFN.nFilterIndex    = MakeFilterSpec(lpstrData, lpstrFile, szServerFilter);
    lstrcpy(szDocDefExt, (*lpstrFile) ? lpstrFile + 1 : "");
    OFN.lpstrDefExt       = NULL;
    OFN.lpstrCustomFilter = (LPSTR)szCustFilterSpec;
    OFN.lpstrFile       = (LPSTR)szDocFile;
    OFN.lpstrTitle      = (LPSTR)szLinkCaption;
    OFN.lpstrFilter     = (LPSTR)szServerFilter;
    OFN.lpstrInitialDir = (LPSTR)szDocPath;
    OFN.hwndOwner       = hwndOwner;
    OFN.Flags             = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

    LockData(0);
    fFile = GetOpenFileName(&OFN);
    UnlockData(0);
    if (fFile)
    {
        if (!(hData2 = GlobalAlloc(GMEM_DDESHARE | GMEM_ZEROINIT, PATHMAX * 2))
            || !(lpstrLink = lpstrTemp = GlobalLock(hData2)))
            goto Error;

        /* If the user didn't specify an extension, use the default */
        if (OFN.nFileExtension == lstrlen(szDocFile) && OFN.nFilterIndex)
        {
            LPSTR   lpstrFilter = (LPSTR)szServerFilter;

            while (*lpstrFilter && --OFN.nFilterIndex)
            {
                while (*lpstrFilter++) ;
                while (*lpstrFilter++) ;
            }
            if (*lpstrFilter)
            {
                while (*lpstrFilter++) ;
                lpstrFilter++;
                lstrcat(szDocFile, lpstrFilter);
            }
        }

        while (*lpstrTemp++ = *lpstrData++)
            continue;
        lstrcpy(lpstrTemp, szDocFile);
        lpstrTemp += lstrlen(lpstrTemp) + 1;
        lpstrData += lstrlen(lpstrData) + 1;
        while (*lpstrTemp++ = *lpstrData++);
        *lpstrTemp = 0;
        dwSize = (DWORD)(lpstrTemp - lpstrLink + 1);

        /* Unlock the appropriate memory blocks */
        lpstrData = NULL;
        GlobalUnlock(hData);    /* Unlock, because OleSetData() may free it */
        GlobalUnlock(hData2);

        /* Compress the block to minimal size */
        hData3 = GlobalReAlloc(hData2, (DWORD)(lpstrTemp - lpstrLink + 1), 0);
        if (!hData3)
            hData3 = hData2;

        if (!OleError(OleSetData(pCard->lpObject, vcfLink, hData3)))
        {
            WaitForObject(pCard->lpObject);
            fSuccess = (OleStatusCallBack == S_OK);
            if (fSuccess)
            {
                fSuccess = !OleError(OleUpdate(pCard->lpObject));
                if (fSuccess)
                {
                    WaitForObject(pCard->lpObject);
                    if (OleStatusCallBack == S_OK)
                    {
                        CurCardHead.flags |= FDIRTY;    
                        fSuccess = TRUE;
                    }
                    else
                        fSuccess = FALSE;
                }
            }
            if (!fSuccess)
            {
                ErrorMessage(E_FAILED_TO_UPDATE);
                goto Error;
            }
        }
    }

    if (CommDlgExtendedError()) /* Assumes low memory. */
        IndexOkError(EINSMEMORY);

    Error:
    if (!fSuccess)
    {
        if (hData3)
            GlobalFree(hData3);

        if (lpstrData)
            GlobalUnlock(hData);
    }

    OFN.hwndOwner       = hwndOwnSave;
    return fSuccess;
}

void PicSaveUndo(PCARD pCard) {
    WORD fOleErrMsg;

    /* If Undo object exists, delete it */
    DeleteUndoObject();

    /* Clone the current object */
    if (pCard->lpObject)
    {
        if (fOleErrMsg = OleError(OleClone(pCard->lpObject, lpclient,
                                           lhcdoc, szUndo, (LPOLEOBJECT FAR *)&lpObjectUndo)))
        {
            lpObjectUndo = NULL;
            if (fOleErrMsg == FOLEERROR_NOTGIVEN)
                ErrorMessage(W_FAILED_TO_CLONE_UNDO);
        }
        else
            otObjectUndo = pCard->otObject;
    }
}

void ErrorMessage(int id) {
    char buf[300];

    LoadString(hIndexInstance, id, buf, sizeof(buf));
    MessageBox(hwndError, buf, szWarning, MB_OK | MB_ICONEXCLAMATION);
}

/* WaitForObject() - Waits, dispatching messages, until the object is free.
 *
 * If the object is busy, spin in a dispatch loop.
 */
void WaitForObject(LPOLEOBJECT lpObject) {
    if (lpObject)
    {
        while (OleQueryReleaseStatus(lpObject) == OLE_BUSY)
            ProcessMessage(hIndexWnd, hAccel);
    }
}


void Hourglass(BOOL fOn) {
    if (fOn)
    {
        if (!(cWait++))
            hcurOle = SetCursor(hWaitCurs);
    }
    else
    {
        if (!(--cWait) && hcurOle)
        {
            SetCursor(hcurOle);
            hcurOle = NULL;
        }
    }
}

void PicCreateFromFile(LPSTR szPackageClass, LPSTR szDropFile, BOOL fLink) 
{
    LPOLEOBJECT     lpObject;
    OBJECTTYPE      otObject;
    int fError;

    Hourglass(TRUE);

    /* Don't replace the current object unless we're successful */
    wsprintf(szObjectName, szObjFormat, idObjectMax + 1);

    if (fLink)
        fError = OleCreateLinkFromFile(szPStdFile, lpclient, szPackageClass,
                                       szDropFile, NULL, lhcdoc, szObjectName,
                                       &lpObject, olerender_draw, 0);
    else
        fError = OleCreateFromFile(szPStdFile, lpclient, szPackageClass,
                                   szDropFile, lhcdoc, szObjectName,
                                   &lpObject, olerender_draw, 0);
    WaitForObject(lpObject);
    if (fError != OLE_WAIT_FOR_RELEASE)
    {
        lpObject = NULL;
        ErrorMessage(E_DRAG_DROP_FAILED);
    }
    else
    {
        LONG objType;

        /* Figure out what kind of object we have */
        OleQueryType(lpObject, &objType);
        switch (objType)
        {
        case OT_EMBEDDED:   otObject = EMBEDDED;    break;
        case OT_LINK:       otObject = LINK;        break;
        default:            otObject = STATIC;      break;
        }
        DoSetHostNames(lpObject, otObject);
    }

    if (lpObject)
    {
        if (CurCard.lpObject)
            PicDelete(&CurCard);
        CurCard.lpObject = lpObject;
        CurCard.otObject = otObject;
        CurCard.idObject = idObjectMax++;
        InvalidateRect(hEditWnd, NULL, TRUE);
        SetRect(&(CurCard.rcObject), 0, 0, 0, 0);
        CurCardHead.flags |= FDIRTY;
    }
}

BOOL EditingEmbObject(
                     PCARD pCard)
{
    if (pCard->lpObject && pCard->otObject == EMBEDDED && 
        OleQueryOpen(pCard->lpObject) == S_OK) /* embedded object open for editing */
        return TRUE;
    else
        return FALSE;
}

int UpdateEmbObject(
                   PCARD pCard,
                   int Flags)
{
    int Result = IDYES;
    char szMsg[100];

    /* If an embedded object is open for editing, try to update */
    if (EditingEmbObject(&CurCard))
    {
        LoadString(hIndexInstance, IDS_UPDATEEMBOBJECT, szMsg, sizeof(szMsg));
        Result = MessageBox(hIndexWnd, szMsg, szCardfile, Flags);
        if (Result == IDYES)
        {
            switch (OleError(OleUpdate(CurCard.lpObject)))
            {
            case FOLEERROR_NOTGIVEN:
                ErrorMessage(E_FAILED_TO_UPDATE);
                break;
#if 0
            case FOLEERROR_GIVEN:
                break;
#endif
            case FOLEERROR_OK:
                WaitForObject(CurCard.lpObject);
                CurCardHead.flags |= FDIRTY;    
                break;
            }
        }
    }
    return Result;
}

BOOL InsertObjectInProgress(
                           void)
{
    char szMsg[200];

    if (EditingEmbObject(&CurCard) && !fInsertComplete)
    {
        LoadString(hIndexInstance, IDS_RETRYAFTERINSERT, szMsg, sizeof(szMsg));
        MessageBox(hIndexWnd, szMsg, szCardfile, MB_OK);
        return TRUE;
    }
    return FALSE;
}

void DoSetHostNames(
                   LPOLEOBJECT lpObject,
                   OBJECTTYPE otObject)
{
    if (lpObject && otObject == EMBEDDED)
    {
        WaitForObject(lpObject);
        OleError(OleSetHostNames(lpObject, szCardfile,
                                 (*CurIFile) ? CurIFile : szUntitled));
    }
}

/* delete undo object if any */
void DeleteUndoObject(
                     void)
{
    if (!lpObjectUndo)
        return;
    WaitForObject(lpObjectUndo);
    if (lpObjectUndo && OleError(OleDelete(lpObjectUndo)))
    {
        ErrorMessage(E_FAILED_TO_DELETE_OBJECT);
    }
    lpObjectUndo = NULL;
}

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

/* Register.C - This file contains all the Registration Database calls.
 */
#include "cardfile.h"
#include <shellapi.h>

void GetClassId(HWND hwnd, LPSTR lpstrClass) 
{
    LONG dwSize = KEYNAMESIZE;
    char szName[KEYNAMESIZE];

    if (!RegQueryValue(HKEY_CLASSES_ROOT, lpstrClass, (LPSTR)szName, &dwSize))
        SetWindowText(hwnd, szName);
    else
        SetWindowText(hwnd, lpstrClass);
}

int MakeFilterSpec(LPSTR lpstrClass, LPSTR lpstrExt, LPSTR lpstrFilterSpec) 
{
    /* Note:  The registration keys are guaranteed to be ASCII. */
    LONG dwSize;
    char szClass[KEYNAMESIZE];
    char szName[KEYNAMESIZE];
    char szString[KEYNAMESIZE];
    unsigned int i;
    int  idWhich = 0;
    int  idFilterIndex = 0;

    for (i = 0; !RegEnumKey(HKEY_CLASSES_ROOT, i++, szName, KEYNAMESIZE); ) {
        if (*szName == '.'              /* Default Extension... */
            /* ... so, get the class name */
             && (dwSize = KEYNAMESIZE)
             && !RegQueryValue(HKEY_CLASSES_ROOT, szName, szClass, &dwSize)
            /* ... and if the class name matches */
             && !lstrcmpi(lpstrClass, szClass)
            /* ... get the class name string */
             && (dwSize = KEYNAMESIZE)
             && !RegQueryValue(HKEY_CLASSES_ROOT, szClass, szString, &dwSize))
         {
            idWhich++;        /* Which item of the combo box is it? */
            /* If the extension matches, save the filter index */
            if (!lstrcmpi(lpstrExt, szName))
                idFilterIndex = idWhich;

            /* Copy over "<Class Name String> (*<Default Extension>)"
             * e.g. "Server Picture (*.PIC)"
             */
            lstrcpy(lpstrFilterSpec, szString);
            lstrcat(lpstrFilterSpec, " (*");
            lstrcat(lpstrFilterSpec, szName);
            lstrcat(lpstrFilterSpec, ")");
            lpstrFilterSpec += lstrlen(lpstrFilterSpec) + 1;

            /* Copy over "*<Default Extension>" (e.g. "*.PIC") */
            lstrcpy(lpstrFilterSpec, "*");
            lstrcat(lpstrFilterSpec, szName);
            lpstrFilterSpec += lstrlen(lpstrFilterSpec) + 1;
        }
    }

    /* Add another NULL at the end of the spec */
    *lpstrFilterSpec = 0;

    return idFilterIndex;
}



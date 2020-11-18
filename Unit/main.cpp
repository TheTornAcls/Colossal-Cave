#include <stdio.h>
#include <windows.h>

#define	KEYNAMESIZE	300
BOOL ServerNameFromClass(
    char szClass[], 
    char szServer[]);  

int main(int argc, char *argv[])

/* InitClassNamesList() - Fills the list box with possible server names. */
//int InitClassNamesList(
//    HWND hwndList) 
{
    char    szClass[KEYNAMESIZE];
    char    szServer[KEYNAMESIZE];
    HKEY    hkeyRoot;
    int     i;

    if (RegOpenKey(HKEY_CLASSES_ROOT, NULL, &hkeyRoot))
        return FALSE;

    for (i = 0; !RegEnumKey(HKEY_CLASSES_ROOT, i, szClass, KEYNAMESIZE); i++)
    {
        if (ServerNameFromClass(szClass, szServer))
        {
            printf("Server Name %s\n", (LPSTR)szServer);
        }
    }

    RegCloseKey(hkeyRoot);
    return 1;
}

/* for a given classname, it returns the server name if one exists.
 * returns TRUE on success, FALSE if given classname is not a server */
BOOL ServerNameFromClass(
    char szClass[], 
    char szServer[])  
{
    LONG   dwSize = KEYNAMESIZE;
    HKEY    hkeyTemp = NULL;
    char    szExec[KEYNAMESIZE];
    int     Result;

    if (*szClass != '.')           /* Not default extension... */
    {
        /* See if this class really refers to a server */
        wsprintf(szExec, "%s\\protocol\\StdFileEditing\\server", (LPSTR)szClass);
        if (!RegOpenKey(HKEY_CLASSES_ROOT, szExec, &hkeyTemp)) 
        {
            /* get the class name string */
            Result = RegQueryValue(HKEY_CLASSES_ROOT, szClass, szServer, &dwSize); 
            RegCloseKey(hkeyTemp);
            if (Result == 0)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

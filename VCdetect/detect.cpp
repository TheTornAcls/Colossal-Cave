#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>

#include "detect.h"
#define gc_iNumVCKeys  5

#define MAX_ENV_SIZE   4000

#define MAX_ARGS 4

LPCSTR g_VCKeys[gc_iNumVCKeys] = 
{
    TEXT("SOFTWARE\\Microsoft\\VisualStudio\\8.0\\Setup\\VC"),
    TEXT("SOFTWARE\\Microsoft\\VCExpress\\8.0\\Setup\\VC"),
    TEXT("SOFTWARE\\Microsoft\\VisualStudio\\8.0\\Setup\\VS"),
    TEXT("SOFTWARE\\Microsoft\\VCExpress\\8.0\\Setup\\VS"),
    TEXT("SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v1.0")

};

#define g_tcVCValue TEXT("ProductDir")
#define g_tcSDKValue TEXT("InstallationFolder")

#define VCRegLocation  0
#define VSRegLocation  2
#define SDKRegLocation 4

//***************************************************************
//
// PUBLIC
//
//***************************************************************
CDetection::CDetection()
{
    m_lpIncludePath = NULL;
    m_lpLibPath = NULL;
    m_lpVCPath = NULL;
    m_lpVSPath = NULL;
    m_lpSDKPath = NULL;

    // Intialize the file name
    m_lpstrFile = NULL;
}

//***************************************************************
//
// PUBLIC
//
//***************************************************************
CDetection::~CDetection()
{
    // The Include Environment Variable
    if (NULL != m_lpIncludePath)
    {
        free(m_lpIncludePath);
    }
    // The LIB Environment variable
    if (NULL != m_lpLibPath)
    {
        free(m_lpLibPath);
    }
    // VC Path Information
    if (NULL != m_lpVCPath)
    {
        free(m_lpVCPath);
    }
    // VS Path Information
    if (NULL != m_lpVSPath)
    {
        free(m_lpVSPath);
    }
    // Windows SDK Path
    if (NULL != m_lpSDKPath)
    {
        free(m_lpSDKPath);
    }

    // File Name
    if (NULL != m_lpstrFile)
    {
        free(m_lpstrFile);
    }
}

//***************************************************************
//
//  PUBLIC  Intialize
//          Parameters      INT argc, CHAR * argv[]
//
//          Verify the filename.  Delete the specified file.
//          Store the requested filename.  Retrieve the necessary 
//          information from the registry and the environment.
//
//***************************************************************
BOOL CDetection::Initialize(INT argc, CHAR * argv[])
{
    UINT unRet; 
    LPSTR lpFileName;

    // Parse the command-line arguments
    lpFileName = ParseArgs(argc, argv);

    if (NULL == lpFileName)
    {
        // don't have a file name
        return FALSE;
    }

    //  Save the file name and delete the file.
    unRet = DelFile(lpFileName);

    if (ERROR_SUCCESS == unRet)
    {
        // Retrived the required registry entries
        m_dwVCSize  = RetrieveRegistry(g_VCKeys[VCRegLocation], g_tcVCValue, &m_lpVCPath);
        m_dwVSSize  = RetrieveRegistry(g_VCKeys[VSRegLocation], g_tcVCValue, &m_lpVSPath);
        m_dwSDKSize = RetrieveRegistry(g_VCKeys[SDKRegLocation] ,g_tcSDKValue, &m_lpSDKPath );

        // Retrieve the required Environment variables
        m_dwIncludeSize = RetrievePath(TEXT("INCLUDE"),&m_lpIncludePath);
        m_dwLibSize     = RetrievePath(TEXT("LIB"), &m_lpLibPath);
        //need to check return codes
        return TRUE;
    } else
    {
        return FALSE;
    }

} // End Initialize

//***************************************************************
//
//  PUBLIC
//
//***************************************************************
BOOL CDetection::ModifyInfo()
{
    RemoveInclude();
    RemoveLib();

    return TRUE;
} // End of ModifyInfo

//***************************************************************
//
// PUBLIC
//
//***************************************************************
UINT CDetection::WriteBatchFile()
{
    UINT      unRet           = ERROR_SUCCESS;
    DWORD     dwBytesWritten  = 0;
    HANDLE    hFile = NULL;


    // detect if we should write anything
    if ( (NULL == m_lpstrFile ) )
    {
        // Nothing was found and nothing was set.
        // Just Leave
        return  ERROR_NO_DATA;
    }
    if ( (NULL==m_lpVSPath) && (NULL==m_lpVCPath) && (NULL == m_lpSDKPath))
    {
        // Nothing was found and nothing was set.
        // Just Leave
        return  ERROR_NO_DATA;
    }


    // Create/open the file lpFileName
    hFile = CreateFile((LPCSTR)m_lpstrFile,                            // file name
                       GENERIC_WRITE,                                   // access mode
                       0,                                               // share mode
                       NULL,                                            // SD
                       CREATE_ALWAYS,                                   // how to create
                       FILE_ATTRIBUTE_NORMAL,                           // file attributes
                       NULL);                                           // handle to template file
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }


    // No echo
    // TEXT("@ECHO OFF\r\n");
    WriteFileInternal(hFile, TEXT("@ECHO OFF\r\n"));


    if (NULL != m_lpIncludePath)
    {
        // set INCLUDE=m_lpIncludePATH
        WriteFileInternal(hFile, TEXT("SET INCLUDE="));
        WriteFileInternal(hFile, m_lpIncludePath);
        WriteFileInternal(hFile, TEXT("\r\n"));
    }
    if (NULL != m_lpLibPath)
    {
        WriteFileInternal(hFile, TEXT("SET LIB="));
        WriteFileInternal(hFile, m_lpLibPath);
        WriteFileInternal(hFile, TEXT("\r\n"));
        // set LIB=m_lpLibPATH
    }
    if (NULL != m_lpVSPath)
    {
        // set VSINSTALLDIR=m_lpVSPATH
        WriteFileInternal(hFile, TEXT("SET VSINSTALLDIR="));
        WriteFileInternal(hFile, m_lpVSPath);
        WriteFileInternal(hFile, TEXT("\r\n"));
    }
    if (NULL != m_lpVCPath)
    {
        // set VCINSTALLDIR=m_lpVCPATH
        WriteFileInternal(hFile, TEXT("SET VCINSTALLDIR="));
        WriteFileInternal(hFile, m_lpVCPath);
        WriteFileInternal(hFile, TEXT("\r\n"));
    }

    return unRet;
} // End of WriteBatchFile

//***************************************************************
//
//  PRIVATE
//
//***************************************************************
BOOL CDetection::FindAndRemove(LPSTR lpSource, LPSTR lpSub, DWORD dwSize)
{
    LPSTR lpFind = NULL;
    LPSTR lpSearch = lpSource;
    LPSTR lpBuffer;

    if ((NULL == lpSource) || (NULL == lpSub) || (0 == dwSize ) || (MAX_ENV_SIZE < dwSize))
    {
        // OK we have some bad information
        return FALSE;
    }

    // Indexes into the two strings
    LPSTR lpSearchIndex;
    LPSTR lpBufferIndex;


    lpBuffer = (TCHAR*) malloc(dwSize * sizeof(TCHAR));

    if (NULL == lpBuffer)
    {
        // out of memory
        return FALSE;
    }

    //set the indexes
    lpSearchIndex = lpSource;
    lpBufferIndex = lpBuffer;

    lpFind = strstr(lpSearch, lpSub);
    while (NULL != lpFind)
    {
        // Copy the characters into the the temporary buffer up to the found lpSub
        while (lpSearchIndex != lpFind)
        {
            *lpBufferIndex = *lpSearchIndex;
            lpBufferIndex++;
            lpSearchIndex++;
        }

        // OK we have a sub string
        lpSearch = lpFind;

        // WHAT about the NULL character?? 
        while ((';' != *lpSearch) && ('\0' != *lpSearch))
        {
            lpSearch++;
        }

        // If the last character seen is a semi-colon, advance to the next character
        if (';' == *lpSearch)
        {
            // Go to the next character
            lpSearch++;
        }

        // move the Source pointer past the found substring
        lpSearchIndex = lpSearch;

        // Search the "new" string for the next instance of the sub string
        lpFind = strstr(lpSearch, lpSub);

    }
    // Can we put in an small optimization
    // if the lpSearchIndex != lpSource
    while (*lpSearchIndex != '\0')
    {
        *lpBufferIndex = *lpSearchIndex;
        lpBufferIndex++;
        lpSearchIndex++;
    }

    // null terminate the string
    *lpBufferIndex = '\0';

    // Copy back into the original buffer
    strcpy_s(lpSource, dwSize, lpBuffer);

    // Free the temporary buffer
    free(lpBuffer);

    return TRUE;
} // End of FindAndRemove


//***************************************************************
//
// PRIVATE
//
//***************************************************************
DWORD CDetection::RetrieveRegistry(LPCSTR tcVCKey,
                                   LPCSTR tcVCValue,
                                   LPSTR * lpRegVal)
{
    DWORD     dwRet      = 0;
    LONG      lRet       = ERROR_SUCCESS;
    HKEY      hKey       = NULL;
    DWORD     dwType     = 0;
    DWORD     dwBufSize  = 0;
    TCHAR    *pBuf       = NULL;
    WIN32_FILE_ATTRIBUTE_DATA wfaFileInfo;

    // Try to open HKLM\[tcVCKey]
    lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                        tcVCKey,
                        0, 
                        KEY_QUERY_VALUE,
                        &hKey);

    if ((lRet == ERROR_SUCCESS) &&
        (hKey))
    {
        // Got it
        // Get the size of the buffer required to hold the value
        dwType        = REG_SZ;
        lRet = RegQueryValueEx(hKey,            // handle to key
                               tcVCValue,       // value name
                               NULL,            // reserved
                               &dwType,         // type buffer
                               NULL,            // data buffer
                               &dwBufSize);     // size of data buffer

        // Check error code and size of required buffer
        // Check to see if buffer equals 0 or is greater than MAX_PATH then fail
        if ((lRet != ERROR_SUCCESS) ||
            (dwBufSize == 0) ||
            (dwBufSize > MAX_PATH))
        {
            goto cleanup;
        }


        pBuf = (TCHAR *)malloc((dwBufSize + 1) * sizeof(TCHAR));
        if (!pBuf)
        {
            // Error in allocating
            dwBufSize = 0;
            goto cleanup;
        }

        // This is our return value
        dwRet = dwBufSize +1;

        lRet = RegQueryValueEx(hKey,            // handle to key
                               tcVCValue,       // value name
                               NULL,            // reserved
                               &dwType,         // type buffer
                               (LPBYTE)pBuf,    // data buffer
                               &dwBufSize);     // size of data buffer

        if ((lRet != ERROR_SUCCESS) ||
            (dwBufSize == 0))
        {
            dwRet = 0;
            free(pBuf);
            pBuf = NULL;
        } else
        {
            // CHECK exsistance of the path defined by the key
            GetFileAttributesEx(pBuf, GetFileExInfoStandard, &wfaFileInfo);
            if (!(wfaFileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                // the directory passed in is INVALID
                dwRet =0;
                free(pBuf);
                pBuf = NULL;
            }
        }

        // Cleanup and exit
        cleanup:

        // set the output parameter to the created and intialized buffer
        *lpRegVal = pBuf;


        if (hKey)
        {
            RegCloseKey(hKey);
        }
    }


    return dwRet;
} // Endof RetrieveRegistry


//***************************************************************
//
//  PRIVATE
//
//***************************************************************
DWORD CDetection::RetrievePath (LPSTR lpEnvPath, LPSTR * lplpPath)
{

    LPSTR libvar = NULL;
    size_t requiredSize = 0;

    // call getenv_s to obtain the size of the required buffer
    getenv_s( &requiredSize, NULL, 0, lpEnvPath);

    // make sure we get a valid piece of information
    if ((0 != requiredSize) || (MAX_ENV_SIZE > requiredSize))
    {
        // allocate memory for the buffer
        libvar = (TCHAR*) malloc(requiredSize * sizeof(TCHAR));
        if (!libvar)
        {
            // Error allocating memory
            return NULL;
        }

        // Get the value of the LIB environment variable.
        getenv_s( &requiredSize, libvar, requiredSize, lpEnvPath );

        // set the out parameter to the created and intialized buffer
        *lplpPath = libvar;

    } else
    {
        // Make sure we return back NULL and a buffer size of 0
        requiredSize = 0;
    }

    return requiredSize;
} // End of RetrievePath



//***************************************************************
//
// PRIVATE
//
//***************************************************************
BOOL CDetection::RemoveInclude()
{
    FindAndRemove(m_lpIncludePath, m_lpVCPath, m_dwIncludeSize);
    FindAndRemove(m_lpIncludePath, m_lpVSPath, m_dwIncludeSize);
    FindAndRemove(m_lpIncludePath, m_lpSDKPath, m_dwIncludeSize);

    return TRUE;
} // End of RemoveInclude

//***************************************************************
//
//  PRIVATE
//
//***************************************************************
BOOL CDetection::RemoveLib()
{
    FindAndRemove(m_lpLibPath, m_lpVCPath, m_dwLibSize);
    FindAndRemove(m_lpLibPath, m_lpVSPath, m_dwLibSize);
    FindAndRemove(m_lpLibPath, m_lpSDKPath, m_dwLibSize);

    return TRUE;
} // End of RemoveLib


//***************************************************************
//
//  PRIVATE
//
//***************************************************************
UINT CDetection::WriteFileInternal(HANDLE hFileHandle, LPSTR lpString)
{
    UINT uiRet = ERROR_SUCCESS;
    DWORD dwBytesWritten;

    // hFileHandle is NOT null when this method is call.  No need to check

    // Flush to file    
    if (!WriteFile(hFileHandle,                      // handle to file
                   (LPCSTR)lpString,                  // data buffer
                   strlen(lpString)*sizeof(TCHAR),     // number of bytes to write
                   &dwBytesWritten,                    // number of bytes written
                   NULL))                              // overlapped buffer
    {
        uiRet = GetLastError();
    }
    return uiRet;
} // End of WriteFileInternal

//***************************************************************
//
//  PRIVATE
//
//***************************************************************
UINT CDetection::DelFile(LPSTR lpFileName)
{

    // is the name passed in NULL??
    if (NULL == lpFileName)
    {
        return ERROR_NO_DATA;
    }


    // Delete the file lpFileName
    if (!SetFileAttributes((LPCSTR)lpFileName,                 // file name
                           FILE_ATTRIBUTE_NORMAL))             // attributes
    {
        // BUGBUG: Log an error?
    }

    if (!DeleteFile((LPCSTR)lpFileName))
    {
        // BUGBUG: Log an error?
    }

    // save off the name of the file
    INT iLen = strnlen(lpFileName, MAX_PATH);

    if (MAX_PATH > iLen)
    {
        m_lpstrFile = (TCHAR *)malloc( (iLen+1) * sizeof(TCHAR));

        if (NULL != m_lpstrFile)
        {
            // save the string information
            strcpy_s(m_lpstrFile, iLen+1, lpFileName);
            return ERROR_SUCCESS;
        }
    }

    _ASSERT(FALSE);
    return ERROR_NO_DATA;

} // End of DelFile


//***************************************************************
//
//  PRIVATE
//
//***************************************************************
void CDetection::DisplayLogo()
{
    printf("Microsoft (R) VCDetect Executable Version 1.00.00\n");
    printf("Copyright (C) Microsoft Corporation.  All rights reserved.\n"); 
}

//***************************************************************
//
//  PRIVATE
//
//***************************************************************
void CDetection::DisplayUsage()
{
    printf("VCDetect.exe <-nologo> <-h | -?> <filename>\n");
    printf("              -nologo  - run VCDetect silently\n");
    printf("              -h | -?  - prints out this message\n");
    printf("              filename - the location of where VCDetect will save the results\n\n"); 

}

//***************************************************************
//
//  PRIVATE
//
//***************************************************************
LPSTR CDetection::ParseArgs(INT argc, CHAR * argv[])
{
    LPSTR lpFile = NULL;
    BOOL bQuiet = FALSE;
    BOOL bHelp = FALSE;

    // First check to see if we have bogus arguments
    if ((argc > MAX_ARGS) || (1 == argc))
    {
        // Yeah we do. Print out both the logo and the usage
        DisplayLogo();
        DisplayUsage();
    }
    else
    {
        INT iCount;

        // walk through the list of arguments looking for the arguments
        for (iCount = 1; iCount < argc; iCount++)
        {

            // Check the first character for '-' or '/'
            if ('-' == (*argv[iCount] ) || ('/' == *argv[iCount] ))
            {
                if (!_stricmp(argv[iCount]+1, "nologo"))
                {
                    if (bQuiet)
                    {
                        // already set  no logo
                        printf("Already Set nologo\n");
                        bHelp = TRUE;
                        break;
                    }
                    else 
                    {
                        bQuiet = TRUE;
                    }
                } else if (!_stricmp(argv[iCount]+1, "?"))
                {
                    if (bHelp)
                    {
                        // already set  help request
                        printf("Already Set help\n");
                        break;
                    }
                    else 
                    {
                        bHelp = TRUE;
                    }
                }
                else
                {
                    // We have an invalid argument
                    printf("Invalid argument\n");
                    bHelp = TRUE;
                }
            }
            else
            {
                if (NULL != lpFile)
                {
                    // already set filename
                    printf("Already Set filename\n");
                    bHelp = TRUE;
                    break;
                }
                else 
                {
                    lpFile = argv[iCount];
                }
            }
        }  // end of for loop

        // Did the user ask for NOLOGO
        if (!bQuiet)
        {
            // Nope - then print out the banner
            DisplayLogo();
        }

        // did we have a file name OR was help requested
        if ((NULL == lpFile) || (bHelp))
        {
            // Need to print out help - don't allow program to continue
            lpFile=NULL;
            DisplayUsage();
        }

    }  // end of argv argc check

    // return back the name of the file
    return lpFile;
} // End of ParseArgs

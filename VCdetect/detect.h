#ifndef __DETECT_H
#define __DETECT_H

class CDetection
{
public:
    CDetection();
    ~CDetection();
    BOOL Initialize(INT argc, CHAR * argv[]);
    BOOL ModifyInfo();
    UINT WriteBatchFile();
private:
    DWORD RetrievePath(LPSTR, LPSTR *);

    DWORD RetrieveRegistry(LPCSTR tcVCKey,
                           LPCSTR tcVCValue,
                           LPSTR * lpRegVal);

    BOOL RemoveInclude();
    BOOL RemoveLib();

    BOOL FindAndRemove(LPSTR, LPSTR, DWORD dwSize);
    UINT WriteFileInternal(HANDLE,LPSTR);
    UINT DelFile(LPSTR);

    // Parse the command line arguments
    LPSTR ParseArgs(int argc, char * argv[]);

    void DisplayLogo();
    void DisplayUsage();

    // Some member variables
    LPSTR m_lpIncludePath;
    LPSTR m_lpLibPath;

    DWORD m_dwIncludeSize;
    DWORD m_dwLibSize;

    // Retrieved from the registry
    LPSTR m_lpVCPath;
    LPSTR m_lpVSPath;
    LPSTR m_lpSDKPath;

    DWORD m_dwVCSize;
    DWORD m_dwVSSize;
    DWORD m_dwSDKSize;

    // Name of needed file
    LPSTR m_lpstrFile;

};

#endif // __DETECT_H

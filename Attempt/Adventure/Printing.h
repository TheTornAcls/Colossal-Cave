#ifndef __PRINTING_H
#define __PRINTING_H


class cPrinting
{
private:
    HANDLE m_hHandle;
    WORD   m_wOldColorAttrs;
    //WORD   m_wBackGround;
public:
    cPrinting();
    ~cPrinting();
    void ConOutputError(LPSTR, ...);
    void ConOutputChar(CHAR);
    void ConOutput(LPSTR, ...);
};
  
#endif

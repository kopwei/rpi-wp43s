#ifndef _WP43S_H_
#define _WP43S_H_

class CEPaperDisplay;

class CRPiCW43sCalculator
{
public:
    CRPiCW43sCalculator();
    ~CRPiCW43sCalculator();
    void Start();
private:
    static void* StartMonitorKeyboard(void*);
    static void* StartMonitorCalcProcess(void*);
    CEPaperDisplay* pDisplay;
};

#endif

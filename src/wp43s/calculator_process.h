#ifndef _WP43S_CALCULATOR_PROCESS_H_
#define _WP43S_CALCULATOR_PROCESS_H_

#include <string>

const std::string WP43S_PROCESS_NAME = "wp43s";

class CCaculatorProcess
{
public:
    CCaculatorProcess();
    ~CCaculatorProcess();
    void StartOrFindProcess();
    static const bool IsCalculatorProcessRunning();
private:
    static const int FindProcessByName(const std::string procName=WP43S_PROCESS_NAME);
    const int StartProcess();
    int iprocessId;
    std::string calculatorBinName;
};
#endif

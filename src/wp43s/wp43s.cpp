#include "wp43s.h"
#include "display.h"

CRPiCW43sCalculator::CRPiCW43sCalculator()
{
    pDisplay = CEPaperDisplay::GetInstance();
}

CRPiCW43sCalculator::~CRPiCW43sCalculator()
{
    if(pDisplay)
    {
        delete pDisplay;
    }
    pDisplay = nullptr;
}

void CRPiCW43sCalculator::Start()
{
    pDisplay->InitDisplay();
    pDisplay->GrabScreenAndShowFull();
}
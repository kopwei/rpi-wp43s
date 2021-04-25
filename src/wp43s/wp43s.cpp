#include "wp43s.h"
#include "display.h"
#include "keyboard.h"
#include "calculator_process.h"
#include <iostream>
#include <pthread.h>

// Change the value here. Run command "xinput" to find actual number
// We only monitor one keyboard here
const std::string KEYBOARD_DEVICE_NAME = "LingYao ShangHai Thumb Keyboard";

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

void* CRPiCW43sCalculator::StartMonitorKeyboard(void* display)
{
    CKeyboardHandler keyboard;
    keyboard.RegisterObserver(static_cast<CEPaperDisplay*>(display));
    keyboard.StartMonitorEvent(KEYBOARD_DEVICE_NAME);
    return NULL;
}

void* CRPiCW43sCalculator::StartMonitorCalcProcess(void*)
{
    CCaculatorProcess wp43s_process;
    wp43s_process.StartOrFindProcess();
    return NULL;
}

void CRPiCW43sCalculator::Start()
{
    pthread_t threads[2];
    int calculatorRc = pthread_create(&threads[0], NULL, StartMonitorCalcProcess, NULL);
    if (calculatorRc)
    {
        std::cout << "Error: unable to start wp43s process, " << calculatorRc << std::endl;
        exit(-1);
    }
    pDisplay->InitDisplay();
    pDisplay->GrabScreenAndShowFull();
    int keyboardRc = pthread_create(&threads[1], NULL, StartMonitorKeyboard, static_cast<void *>(pDisplay));
    if (keyboardRc)
    {
         std::cout << "Error:unable to create keyboard thread," << keyboardRc << std::endl;
         exit(-1);
    }
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pDisplay->ShowSleepPicture();
    pDisplay->Sleep();
}
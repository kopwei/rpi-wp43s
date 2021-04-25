#include "wp43s.h"
#include "display.h"
#include "keyboard.h"
#include <iostream>
#include <pthread.h>

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
    keyboard.StartMonitorEvent();
}

void CRPiCW43sCalculator::Start()
{
    pDisplay->InitDisplay();
    pDisplay->GrabScreenAndShowFull();
    pthread_t threads[2];
    int keyboardRc = pthread_create(&threads[0], NULL, StartMonitorKeyboard, static_cast<void *>(pDisplay));
    if (keyboardRc) {
         std::cout << "Error:unable to create keyboard thread," << keyboardRc << std::endl;
         exit(-1);
    }
    void* keyboardMonitorRet;
    pthread_join(threads[0], &keyboardMonitorRet);
}
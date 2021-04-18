#include <iostream>
#include "EPD_3in7.h"
#include "GUI_Paint.h"

using namespace std;

void test_feature(){
    cout << "Testing ePaper Feature" << endl;
    if(DEV_Module_Init()!=0){
        return;
    }
    EPD_3IN7_4Gray_Init();
    EPD_3IN7_4Gray_Clear();
    DEV_Delay_ms(500);
    Paint_Clear(WHITE);
}
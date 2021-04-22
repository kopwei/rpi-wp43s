#include <iostream>
#include "EPD_3in7.h"
#include "GUI_Paint.h"
//#include <QWidget>
//#include <QDesktopWidget>
//#include <QApplication>
//#include <QPixmap>
#include "screencapturor.h"

using namespace std;


/*void shootScreen(string fileName)
 {
    QPixmap originalPixmap;
	originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId(), 880, 480, 800 , 480);//x, y, width, height
	originalPixmap.save(fileName.c_str());
 }*/

void test_feature(int argc, char *argv[]){
    cout << "Testing ePaper Feature" << endl;
    if(DEV_Module_Init()!=0){
        return;
    }
    /*EPD_3IN7_4Gray_Init();
    EPD_3IN7_4Gray_Clear();
    DEV_Delay_ms(500);
    Paint_Clear(WHITE);
    QApplication MyScreenshot(argc,argv);
    shootScreen("test.png");*/
    //CScreenCapturer(880, 480, 800 , 480);
    CScreenCapturer capturor(0, 0,800, 480);
    cv::Mat img;


	return;
}
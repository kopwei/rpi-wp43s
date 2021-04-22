#include <iostream>
#include <string>
#include "EasyBMP.h"
#include "EPD_3in7.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "screencapturor.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;


/*void shootScreen(string fileName)
 {
    QPixmap originalPixmap;
	originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId(), 880, 480, 800 , 480);//x, y, width, height
	originalPixmap.save(fileName.c_str());
 }*/
const uint SCREEN_WIDTH = 800;
const uint SCREEN_HEIGHT = 480;
const string tmp_path("/tmp/tmp_dump.bmp");

void test_feature(int argc, char *argv[]){
    cout << "Testing ePaper Feature" << endl;
    if(DEV_Module_Init()!=0){
        return;
    }
    EPD_3IN7_4Gray_Init();
    EPD_3IN7_4Gray_Clear();
    DEV_Delay_ms(500);

    CScreenCapturer capturor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    cv::Mat original_colorMat, scaled_colorMat, greyMat;
    capturor(original_colorMat);
    cv::Size newSize(EPD_3IN7_HEIGHT, EPD_3IN7_WIDTH);
    cv::resize(original_colorMat, scaled_colorMat, newSize, 0, 0);
    //cv::GaussianBlur(scaled_colorMat, scaled_colorMat, cv::Size(7,7), 1.5, 1.5);
    cv::cvtColor(scaled_colorMat, greyMat, cv::COLOR_RGBA2GRAY);
    cv::imwrite(tmp_path, greyMat);
    BMP bitConvBmp;
    bitConvBmp.ReadFromFile(tmp_path.c_str());
    bitConvBmp.SetBitDepth(4);
    bitConvBmp.WriteToFile(tmp_path.c_str());


    UBYTE *canvas;
    UWORD Imagesize = ((EPD_3IN7_WIDTH % 4 == 0)? (EPD_3IN7_WIDTH / 4 ): (EPD_3IN7_WIDTH / 4 + 1)) * EPD_3IN7_HEIGHT;
    if((canvas = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return;
    }
    Paint_SelectImage(canvas);
    Paint_Clear(WHITE);
    GUI_ReadBmp_4Gray(tmp_path.c_str(), 0, 0);
    EPD_3IN7_4Gray_Display(canvas);
    EPD_3IN7_Sleep();
    // cv::waitKey(0);

	return;
}
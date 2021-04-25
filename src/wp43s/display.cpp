#include "screencapturor.h"
#include "EPD_3in7.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "EasyBMP.h"
#include <string>
#include <experimental/filesystem>
#include "display.h"
#include "calculator_process.h"
#include "pic_compare.h"

using namespace std;


const uint LCD_SCREEN_WIDTH = 800;
const uint LCD_SCREEN_HEIGHT = 480;
const uint EPAPER_SCREEN_WIDTH = EPD_3IN7_HEIGHT;
const uint EPAPER_SCREEN_HEIGHT = EPD_3IN7_WIDTH;

const string tmp_path("/tmp/tmp_dump.bmp");
const string old_dump_path("/tmp/old_tmp_dump.bmp");
const string diff_pic_path("/tmp/diff_pic.bmp");


CEPaperDisplay* CEPaperDisplay::epaper_display_ = nullptr;

CEPaperDisplay* CEPaperDisplay::GetInstance()
{
    if (!epaper_display_)
    {
        epaper_display_ = new CEPaperDisplay;
    }
    return epaper_display_;
}

CEPaperDisplay::CEPaperDisplay()
{
    this->capturor = new CScreenCapturer(0, 0, LCD_SCREEN_WIDTH, LCD_SCREEN_HEIGHT);
    this->bitConvBmp = new BMP();
    UWORD Imagesize = ((EPD_3IN7_WIDTH % 4 == 0)? (EPD_3IN7_WIDTH / 4 ): (EPD_3IN7_WIDTH / 4 + 1)) * EPD_3IN7_HEIGHT;
    if((canvas = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return;
    }
}

CEPaperDisplay::~CEPaperDisplay()
{
    if(this->capturor)
    {
        delete this->capturor;
    }
    this->capturor = nullptr;
    if(this->bitConvBmp)
    {
        delete this->bitConvBmp;
    }
    this->bitConvBmp = nullptr;
    if(canvas)
    {
        free(canvas);
    }
    canvas = NULL;
}

void CEPaperDisplay::InitDisplay()
{
    cout << "Initalizing Display" << endl;
    if(DEV_Module_Init()!=0){
        return;
    }
    EPD_3IN7_4Gray_Init();
    EPD_3IN7_4Gray_Clear();
    DEV_Delay_ms(500);
    EPD_3IN7_1Gray_Init();
    EPD_3IN7_1Gray_Clear();
    DEV_Delay_ms(500);
}

void CEPaperDisplay::GrabScreenAndShowFull()
{
    CaptureScreenInGray();
    Convert8bitGreyTo1bit();
    Paint_NewImage(canvas, EPD_3IN7_WIDTH, EPD_3IN7_HEIGHT, 270, WHITE);
    Paint_SetScale(2);
    Paint_SelectImage(canvas);
    Paint_Clear(WHITE);
    GUI_ReadBmp(tmp_path.c_str(), 0, 0);
    EPD_3IN7_1Gray_Display(canvas);
}

void CEPaperDisplay::GrabScreenAndShowDiff()
{
    CaptureScreenInGray();
    Convert8bitGreyTo1bit();
    DiffArea diff_area = CBMPComparator::GetDiffArea(old_dump_path.c_str(), tmp_path.c_str());
    if(diff_area.Exists())
    {
        CBMPComparator::SaveDiffAreaToFile(tmp_path, diff_area, diff_pic_path);
        Paint_SelectImage(canvas);
        Paint_Clear(WHITE);
        GUI_ReadBmp(tmp_path.c_str(), diff_area.X_MIN, diff_area.Y_MIN);
        EPD_3IN7_1Gray_Display_Part(canvas, diff_area.X_MIN, diff_area.Y_MIN, diff_area.X_MAX, diff_area.Y_MAX);
    }
}

void CEPaperDisplay::CaptureScreenInGray()
{
    MoveExistingScreeshotIfExists();
    cv::Mat original_colorMat, scaled_colorMat, greyMat;
    capturor->CaptureToImg(original_colorMat);
    cv::Size newSize(EPAPER_SCREEN_WIDTH, EPAPER_SCREEN_HEIGHT);
    cv::resize(original_colorMat, scaled_colorMat, newSize, 0, 0);
    cv::cvtColor(scaled_colorMat, greyMat, cv::COLOR_RGBA2GRAY);
    cv::rotate(greyMat, greyMat, cv::ROTATE_180);
    cv::imwrite(tmp_path, greyMat);
}

void CEPaperDisplay::MoveExistingScreeshotIfExists()
{
    if (experimental::filesystem::exists(tmp_path))
    {
        rename(tmp_path.c_str(), old_dump_path.c_str());
    }
}

void CEPaperDisplay::Convert8bitGreyTo1bit()
{
    bitConvBmp->ReadFromFile(tmp_path.c_str());
    bitConvBmp->SetBitDepth(1);
    bitConvBmp->WriteToFile(tmp_path.c_str());
}

void CEPaperDisplay::HandleKeyboardEvent(const KeyboardEvent event)
{
    if (!CCaculatorProcess::IsCalculatorProcessRunning())
    {
        return;
    }
    if(KeyReleaseEvent == event)
    {
        GrabScreenAndShowFull();
        //# Debug purpose
        DiffArea diff_area = CBMPComparator::GetDiffArea(old_dump_path.c_str(), tmp_path.c_str());
        if(diff_area.Exists())
        {
            CBMPComparator::SaveDiffAreaToFile(tmp_path, diff_area, diff_pic_path);
        }
    }
}

void CEPaperDisplay::ShowSleepPicture()
{
    EPD_3IN7_4Gray_Init();
    printf("Clear...\r\n");
    EPD_3IN7_4Gray_Clear();
    //GUI_ReadBmp_4Gray("../../picture/3in7_4gray2.bmp", 0, 0);
    //EPD_3IN7_4Gray_Display(canvas);
}

void CEPaperDisplay::Sleep()
{
    EPD_3IN7_Sleep();
}

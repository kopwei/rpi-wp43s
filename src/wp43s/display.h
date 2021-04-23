#ifndef _WP43S_DISPLAY_H_
#define _WP43S_DISPLAY_H_

#include "DEV_Config.h"

class CScreenCapturer;

class CEPaperDisplay
{
public:
    static CEPaperDisplay* GetInstance();

    ~CEPaperDisplay();
    void InitDisplay();
    void GrabScreenAndShowFull();
    void GrabScreenAndShowDiff();
    void Sleep();

private:
    CEPaperDisplay();
    CEPaperDisplay(CEPaperDisplay const&);
    CEPaperDisplay& operator=(CEPaperDisplay const&);
    CScreenCapturer* capturor;
    static CEPaperDisplay* epaper_display_;

    void CaptureScreenInGray();
    void Convert8bitGreyTo1bit();
    UBYTE* canvas;
};

#endif
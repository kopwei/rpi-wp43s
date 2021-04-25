#ifndef _WP43S_DISPLAY_H_
#define _WP43S_DISPLAY_H_

#include "DEV_Config.h"
#include "observer.h"

class CScreenCapturer;
class BMP;

class CEPaperDisplay : public CKeyboardObserver
{
public:
    static CEPaperDisplay* GetInstance();

    ~CEPaperDisplay();
    void InitDisplay();
    void GrabScreenAndShowFull();
    void GrabScreenAndShowDiff();
    void ShowSleepPicture();
    void Sleep();
    void HandleKeyboardEvent(const KeyboardEvent event);

private:
    CEPaperDisplay();
    CEPaperDisplay(CEPaperDisplay const&);
    CEPaperDisplay& operator=(CEPaperDisplay const&);
    CScreenCapturer* capturor;
    static CEPaperDisplay* epaper_display_;

    void CaptureScreenInGray();
    void Convert8bitGreyTo1bit();
    UBYTE* canvas;
    BMP* bitConvBmp;
};

#endif
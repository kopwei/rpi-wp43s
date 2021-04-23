#ifndef _SCREEN_CAPTURER_
#define _SCREEN_CAPTURER_


#include <opencv2/opencv.hpp>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <cstdint>

class CScreenCapturer{
public:
    CScreenCapturer(uint x, uint y, uint width, uint height);

    void CaptureToImg(cv::Mat& cv_img);

    void operator() (cv::Mat& cv_img);

    ~CScreenCapturer();
private:
    Display* display;
    Window root;
    XWindowAttributes window_attributes;
    Screen* screen;
    XImage* ximg;
    XShmSegmentInfo shminfo;

    int x, y, width, height;

    bool init;
};


#endif

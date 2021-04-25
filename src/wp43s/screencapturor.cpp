#include "screencapturor.h"
#include <cstring>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include <iostream>

using namespace std;


CScreenCapturer::CScreenCapturer(uint x, uint y, uint width, uint height):
               x(x), y(y), width(width), height(height)
{

    display = XOpenDisplay(nullptr);
    root = DefaultRootWindow(display);

    XGetWindowAttributes(display, root, &window_attributes);
    screen = window_attributes.screen;
    ximg = XShmCreateImage(display, DefaultVisualOfScreen(screen), DefaultDepthOfScreen(screen), ZPixmap, NULL, &shminfo, width, height);

    shminfo.shmid = shmget(IPC_PRIVATE, ximg->bytes_per_line * ximg->height, IPC_CREAT|0777);
    shminfo.shmaddr = ximg->data = (char*)shmat(shminfo.shmid, 0, 0);
    shminfo.readOnly = false;
    if(shminfo.shmid < 0)
        cout<<"Fatal shminfo error!"<<endl;
    Status s1 = XShmAttach(display, &shminfo);
    //printf("XShmAttach() %s\n", s1 ? "success!" : "failure!");

    init = true;
}

CScreenCapturer::~CScreenCapturer(){
    if(!init)
        XDestroyImage(ximg);

    XShmDetach(display, &shminfo);
    shmdt(shminfo.shmaddr);
    XCloseDisplay(display);
}

void CScreenCapturer::operator() (cv::Mat& cv_img){
    if(init)
        init = false;

    XShmGetImage(display, root, ximg, 0, 0, 0x00ffffff);
    cv_img = cv::Mat(height, width, CV_8UC4, ximg->data);
}

void CScreenCapturer::CaptureToImg(cv::Mat& cv_img)
{
    if(init)
        init = false;

    XShmGetImage(display, root, ximg, 0, 0, 0x00ffffff);
    cv_img = cv::Mat(height, width, CV_8UC4, ximg->data);
}

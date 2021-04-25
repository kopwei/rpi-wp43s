#ifndef _WP43S_KEYBOARD_H_
#define _WP43S_KEYBOARD_H_

#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>

#ifdef HAVE_XI2
#include <X11/extensions/XInput2.h>
#endif

#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "observer.h"

extern int xi_opcode; /* xinput extension op code */

class CKeyboardHandler
{
public:
    CKeyboardHandler();
    ~CKeyboardHandler(){}

    int StartMonitorEvent(const std::string keyboard_name_or_id);
    void RegisterObserver(CKeyboardObserver* observer);
    void RegisterExitKeyEvent();

    XDeviceInfo* find_device_info(const char *name, Bool only_extended);
    int xinput_version();
    #if HAVE_XI2
    XIDeviceInfo* xi2_find_device_info(char *name);
    #endif
private:
    CKeyboardHandler(const CKeyboardHandler&);
    CKeyboardHandler& operator=(const CKeyboardHandler&);
    void NotifyObservers(const KeyboardEvent Event);
    int LoopEventUntilTerminate(const char* deviceId);
    void _loop_until_terminate();
    bool NeedTerminate();
    static int register_events(Display *display,
        XDeviceInfo *info, const char *dev_name, Bool handle_proximity);

    Display* display;
    std::string keyboard_name;
    std::vector<CKeyboardObserver*> observers;

};

#endif

#ifndef _WP43S_OBSERVER_H_
#define _WP43S_OBSERVER_H_

#include "X11/Xlib.h"

enum KeyboardEvent
{
    UnkownKeyEvent = 0,
    KeyPressEvent,
    KeyReleaseEvent
};

class CKeyboardObserver
{
public:
    virtual void HandleKeyboardEvent(const KeyboardEvent event) = 0;
    virtual ~CKeyboardObserver(){}
};

#endif

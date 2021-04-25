#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "keyboard.h"
#include <ctype.h>
#include <string.h>
#include "calculator_process.h"

using namespace std;

int xi_opcode;

const int INVALID_EVENT_TYPE = -1;

static int motion_type = INVALID_EVENT_TYPE;
static int button_press_type = INVALID_EVENT_TYPE;
static int button_release_type = INVALID_EVENT_TYPE;
static int key_press_type = INVALID_EVENT_TYPE;
static int key_release_type = INVALID_EVENT_TYPE;
static int proximity_in_type = INVALID_EVENT_TYPE;
static int proximity_out_type = INVALID_EVENT_TYPE;


CKeyboardHandler::CKeyboardHandler()
{
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        cout << "Unable to connect to X server" << endl;
        exit(1);
    }
    observers = vector<CKeyboardObserver*>();
}

int CKeyboardHandler::StartMonitorEvent(const string keyboard_name_or_id)
{
    int event, error;
    if(!XQueryExtension(display, "XInputExtension", &xi_opcode, &event, &error))
    {
        cout << "X Input extension not available." << endl;
        exit(1);
    }
    // Проверяется версия расширения, она не должна быть нулевой
    if(!xinput_version())
    {
        cout << INAME << " extension not available" << endl;
        exit(1);
    }
    LoopEventUntilTerminate(keyboard_name_or_id.c_str());
    XSync(display, False);
    XCloseDisplay(display);
    return 0;
}

bool CKeyboardHandler::NeedTerminate()
{
    bool isRunning =CCaculatorProcess::IsCalculatorProcessRunning();
    cout << "Calculator running: " << isRunning << endl;
    return !isRunning;
}

int CKeyboardHandler::LoopEventUntilTerminate(const char* deviceId)
{
    XDeviceInfo *info;

    Bool handle_proximity = True;

    info = find_device_info(deviceId, True);

    if(!info)
    {
      cout << "unable to find device" << deviceId << endl;
      exit(1);
    }
    else
    {
      if(register_events(display, info, deviceId, handle_proximity))
         _loop_until_terminate();
      else
      {
        fprintf(stderr, "no event registered...\n");
        exit(1);
      }
    }

    return 0;
}

XDeviceInfo* CKeyboardHandler::find_device_info(const char *name, Bool only_extended)
{
    Bool is_id = True;
    int len = strlen(name);
    for(int loop = 0; loop<len; loop++)
    {
        if (!isdigit(name[loop]))
        {
            is_id = False;
            break;
        }
    }
    XID id = (XID) - 1;
    if (is_id)
    {
        id = atoi(name);
    }
    int num_devices;
    XDeviceInfo* devices = XListInputDevices(display, &num_devices);
    XDeviceInfo* found = NULL;
    Atom atomTypeKeyboard = XInternAtom(display, XI_KEYBOARD, True);
    for(int loop = 0; loop < num_devices; loop++)
    {
        if ((!only_extended || (devices[loop].use >= IsXExtensionDevice)) &&
            (atomTypeKeyboard == devices[loop].type) &&
            ((!is_id && strcmp(devices[loop].name, name) == 0) ||
            (is_id && devices[loop].id == id)))
        {
            if (found)
            {
                fprintf(stderr,
                        "Warning: There are multiple devices named '%s'.\n"
                        "To ensure the correct one is selected, please use "
                        "the device ID instead.\n\n", name);
                return NULL;
            }
            else
            {
                found = &devices[loop];
            }
        }
    }
    return found;
}

#if HAVE_XI2
XIDeviceInfo* CKeyboardHandler::xi2_find_device_info(char *name)
{
    
    Bool is_id = True;
    for(unsigned int j = 0; j < strlen(name); j++) {
        if (!isdigit(name[j])) {
            is_id = False;
            break;
        }
    }
    int id = -1;
    if (is_id) {
	  id = atoi(name);
    }

    int ndevices;
    XIDeviceInfo info = XIQueryDevice(display, XIAllDevices, &ndevices);
    for(int i = 0; i < ndevices; i++)
    {
        if ((is_id && info[i].deviceid == id) ||
                (!is_id && strcmp(info[i].name, name) == 0))
        {
			XIDeviceInfoX* ret = new XIDeviceInfoX(info[i]);
			XIFreeDeviceInfo(info);
			return ret;
        }
    }

    XIFreeDeviceInfo(info);
    return NULL;
}
#endif

int CKeyboardHandler::xinput_version()
{
    static int vers = -1;

    if (vers != -1)
        return vers;

    XExtensionVersion    *version;
    version = XGetExtensionVersion(display, INAME);

    if (version && (version != (XExtensionVersion*) NoSuchExtension))
    {
        vers = version->major_version;
        XFree(version);
    }

#if HAVE_XI2
    /* Announce our supported version so the server treats us correctly. */
    if (vers >= XI_2_Major)
    {
        const char *forced_version;
        int maj = 2,
            min = 0;

#if HAVE_XI22
        min = 2;
#elif HAVE_XI21
        min = 1;
#endif

        forced_version = getenv("XINPUT_XI2_VERSION");
        if (forced_version) {
            if (sscanf(forced_version, "%d.%d", &maj, &min) != 2) {
                fprintf(stderr, "Invalid format of XINPUT_XI2_VERSION "
                                "environment variable. Need major.minor\n");
                exit(1);
            }
            printf("Overriding XI2 version to: %d.%d\n", maj, min);
        }

        XIQueryVersion(display, &maj, &min);
    }
#endif

    return vers;
}

int CKeyboardHandler::register_events(
    Display *display, XDeviceInfo *info, const char *dev_name, Bool handle_proximity)
{
    int            number = 0;    /* number of events registered */
    XEventClass        event_list[7];
    int            i;
    XDevice        *device;
    Window        root_win;
    unsigned long    screen;
    XInputClassInfo    *ip;

    screen = DefaultScreen(display);
    root_win = RootWindow(display, screen);

    device = XOpenDevice(display, info->id);

    if (!device)
    {
        cout << "unable to open device "<< dev_name << endl;
        return 0;
    }

    if (device->num_classes > 0)
    {
        for (ip = device->classes, i=0; i<info->num_classes; ip++, i++)
        {
            switch (ip->input_class)
            {
            case KeyClass:
                DeviceKeyPress(device, key_press_type, event_list[number]); number++;
                DeviceKeyRelease(device, key_release_type, event_list[number]); number++;
                break;

            case ButtonClass:
                DeviceButtonPress(device, button_press_type, event_list[number]); number++;
                DeviceButtonRelease(device, button_release_type, event_list[number]); number++;
                break;

            case ValuatorClass:
                DeviceMotionNotify(device, motion_type, event_list[number]); number++;
                if (handle_proximity)
                {
                    ProximityIn(device, proximity_in_type, event_list[number]); number++;
                    ProximityOut(device, proximity_out_type, event_list[number]); number++;
                }
                break;

            default:
                cout << "unknown class" << endl;
                break;
            }
        }

        if (XSelectExtensionEvent(display, root_win, event_list, number))
        {
            cout << "error selecting extended events" << endl;
            return 0;
        }
    }
    return number;
}

void CKeyboardHandler::_loop_until_terminate()
{
    XEvent        Event;

    setvbuf(stdout, NULL, _IOLBF, 0);
    do
    {
        XNextEvent(display, &Event);

        if (Event.type == motion_type)
        {
            int    loop;
            XDeviceMotionEvent *motion = (XDeviceMotionEvent *) &Event;

            cout << "motion ";

            for(loop=0; loop<motion->axes_count; loop++)
            {
                cout << "a[" << motion->first_axis + loop << "]=" << motion->axis_data[loop];
            }
            cout << endl;
        } 
        else if ((Event.type == button_press_type) ||
            (Event.type == button_release_type))
        {
            int    loop;
            XDeviceButtonEvent *button = (XDeviceButtonEvent *) &Event;

            cout << "button " << ((Event.type == button_release_type) ? "release " : "press  ") <<
                button->button;

            for(loop=0; loop<button->axes_count; loop++)
            {
                cout << "a[" << button->first_axis + loop << "]=" << button->axis_data[loop];
            }
            cout << endl;
        }
        else if ((Event.type == key_press_type) ||
            (Event.type == key_release_type)) 
        {
            KeyboardEvent kEvent = (Event.type == key_release_type) ? KeyReleaseEvent : KeyPressEvent;
            NotifyObservers(kEvent);
            XDeviceKeyEvent *key = (XDeviceKeyEvent *) &Event;

            cout << "key " << ((Event.type == key_release_type) ? "release " : "press  ") << key->keycode;

            for(int loop=0; loop<key->axes_count; loop++)
            {
                cout << "a[" << key->first_axis + loop << "]= " << key->axis_data[loop];
            }
            cout << endl;
        } 
        else if ((Event.type == proximity_out_type) ||
            (Event.type == proximity_in_type))
        {
            int    loop;
            XProximityNotifyEvent *prox = (XProximityNotifyEvent *) &Event;

            cout << "proximity " << ((Event.type == proximity_in_type) ? "in " : "out");

            for(loop=0; loop<prox->axes_count; loop++)
            {
                cout << "a[" << prox->first_axis + loop << "]=" << prox->axis_data[loop];
            }
            cout << endl;
        }
        else
        {
            cout << "what's that " << Event.type << endl;
        }
    } while(!this->NeedTerminate());
}

void CKeyboardHandler::RegisterObserver(CKeyboardObserver* observer)
{
    observers.push_back(observer);
}

void CKeyboardHandler::NotifyObservers(const KeyboardEvent event)
{
    for(vector<CKeyboardObserver*>::const_iterator it = observers.begin(); it != observers.end(); ++it)
    {
        (*it)->HandleKeyboardEvent(event);
    }
}

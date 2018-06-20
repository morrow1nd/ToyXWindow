#include "ToyXWindow/details/WinBaseXWindowImpl.h"

#include <windowsx.h>
#include <Dbt.h>

#include <initguid.h>
DEFINE_GUID(GUID_DEVINTERFACE_HID, 0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30);


namespace ToyXWindow
{


ToyUtility::int16 WinBaseXWindowImpl::m_PublicKeys[512];
ToyUtility::int16 WinBaseXWindowImpl::m_NativeKeys[(int)KeyType::__End];


// Window callback function (handles window messages)
//
static LRESULT CALLBACK WinBaseXWindomImpl_WndProc(HWND hWnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam)
{
    WinWindow* window = (WinWindow*)GetPropA(hWnd, WinWindow::HWNDGetPropName.c_str());

    if (!window)
    {
        // This is the message handling for the hidden helper window

        // TODOM: handle these messages
        switch (uMsg)
        {
        case WM_DEVICECHANGE:
        {
            if (wParam == DBT_DEVNODES_CHANGED)
            {
                //_glfwInputMonitorChange();
                return TRUE;
            }
            else if (wParam == DBT_DEVICEARRIVAL)
            {
                DEV_BROADCAST_HDR* dbh = (DEV_BROADCAST_HDR*)lParam;
                if (dbh)
                {
                    //if (dbh->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                    //    _glfwDetectJoystickConnectionWin32();
                }
            }
            else if (wParam == DBT_DEVICEREMOVECOMPLETE)
            {
                DEV_BROADCAST_HDR* dbh = (DEV_BROADCAST_HDR*)lParam;
                if (dbh)
                {
                    //if (dbh->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                    //    _glfwDetectJoystickDisconnectionWin32();
                }
            }

            break;
        }
        }

        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    switch (uMsg)
    {
    case WM_SETFOCUS:
    {
        window->OnRecvMsg_Focus(true);

        return 0;
    }

    case WM_KILLFOCUS:
    {
        window->OnRecvMsg_Focus(false);

        return 0;
    }

    case WM_SYSCOMMAND:
    {
        switch (wParam & 0xfff0)
        {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
        {
            // TODOM: monitor power saver
        }
        }
        break;
    }

    case WM_CLOSE:
    {
        window->OnRecvMsg_ShouldClose();
        return 0;
    }

    case WM_CHAR:
    case WM_SYSCHAR:
    case WM_UNICHAR:
    {
        // TODOM: input char
        break;
    }

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        const KeyType key = WinBaseXWindowImpl::_TranslateKey(wParam, lParam);
        const int scancode = (lParam >> 16) & 0x1ff;
        const KeyAction action = ((lParam >> 31) & 1) ? KeyAction::Release : KeyAction::Press;
        const int mods = WinBaseXWindowImpl::GetKeyModifiers();

        if (key == KeyType::__Invalid)
            break;

        if (action == KeyAction::Release && wParam == VK_SHIFT)
        {
            // Release both Shift keys on Shift up event, as only one event
            // is sent even if both keys are released
            window->OnRecvMsg_Key(KeyType::LeftShift, scancode, action, mods);
            window->OnRecvMsg_Key(KeyType::RightShift, scancode, action, mods);
        }
        else if (wParam == VK_SNAPSHOT)
        {
            // Key down is not reported for the Print Screen key
            window->OnRecvMsg_Key(key, scancode, KeyAction::Press, mods);
            window->OnRecvMsg_Key(key, scancode, KeyAction::Release, mods);
        }
        else
            window->OnRecvMsg_Key(key, scancode, action, mods);

        break;
    }

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    {
        MouseButtonType button;
        KeyAction action;

        if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)
            button = MouseButtonType::Left;
        else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP)
            button = MouseButtonType::Right;
        else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP)
            button = MouseButtonType::Middle;
        else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
            button = MouseButtonType::_4;
        else
            button = MouseButtonType::_5;

        if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN ||
            uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN)
        {
            action = KeyAction::Press;
            SetCapture(hWnd);
        }
        else
        {
            action = KeyAction::Release;
            ReleaseCapture();
        }

        window->OnRecvMsg_MouseButton(button, action, WinBaseXWindowImpl::GetKeyModifiers());

        if (uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONUP)
            return TRUE;

        return 0;
    }

    case WM_MOUSEMOVE:
    {
        const int x = GET_X_LPARAM(lParam);
        const int y = GET_Y_LPARAM(lParam);

        //if (window->cursorMode == GLFW_CURSOR_DISABLED)
        //{
        //    const int dx = x - window->win32.lastCursorPosX;
        //    const int dy = y - window->win32.lastCursorPosY;

        //    if (_glfw.win32.disabledCursorWindow != window)
        //        break;

        //    _glfwInputCursorPos(window,
        //        window->virtualCursorPosX + dx,
        //        window->virtualCursorPosY + dy);
        //}
        //else
            window->OnRecvMsg_CursorPos((float)x, (float)y);

        window->m_LastCursorPosX = x;
        window->m_LastCursorPosY = y;

        if (!window->m_CursorTracked)
        {
            TRACKMOUSEEVENT tme;
            ZeroMemory(&tme, sizeof(tme));
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = window->GetHWND();
            TrackMouseEvent(&tme);

            window->m_CursorTracked = 1;
            window->OnRecvMsg_CursorEnterLeave(true);
        }

        return 0;
    }

    case WM_MOUSELEAVE:
    {
        window->m_CursorTracked = 0;
        window->OnRecvMsg_CursorEnterLeave(false);
        return 0;
    }

    case WM_MOUSEWHEEL:
    {
        ScrollDesc desc;
        desc.IsMouse = true;
        desc.Scroll.MouseScroll.XOffset = 0;
        desc.Scroll.MouseScroll.YOffset = (int32) ((SHORT)HIWORD(wParam) / (double)WHEEL_DELTA);
        window->OnRecvMsg_Scroll(desc);
        return 0;
    }

    case WM_MOUSEHWHEEL:
    {
        // This message is only sent on Windows Vista and later
        // NOTE: The X-axis is inverted for consistency with OS X and X11.
        ScrollDesc desc;
        desc.IsMouse = true;
        desc.Scroll.MouseScroll.XOffset = (int32) (-((SHORT)HIWORD(wParam) / (double)WHEEL_DELTA));
        desc.Scroll.MouseScroll.YOffset = 0;
        window->OnRecvMsg_Scroll(desc);
        return 0;
    }

    case WM_ENTERSIZEMOVE:
    case WM_ENTERMENULOOP:
    {
        // TODOH
        //if (window->cursorMode == GLFW_CURSOR_DISABLED)
        //    window->SetCursorMode(CursorMode::Normal);

        break;
    }

    case WM_EXITSIZEMOVE:
    case WM_EXITMENULOOP:
    {
        // TODOH
        //if (window->cursorMode == GLFW_CURSOR_DISABLED)
        //    _glfwPlatformSetCursorMode(window, GLFW_CURSOR_DISABLED);

        break;
    }


    case WM_SIZE:
    {
        const int iconified =
            !window->m_Iconified && wParam == SIZE_MINIMIZED;
        const int restored =
            window->m_Iconified &&
            (wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED);

        // TODOM
        //if (_glfw.win32.disabledCursorWindow == window)
        //    updateClipRect(window);

        if (iconified)
            window->OnRecvMsg_Iconify(true);
        else if (restored)
            window->OnRecvMsg_Iconify(false);

        window->OnRecvMsg_FrameBufferSize(LOWORD(lParam), HIWORD(lParam));
        window->OnRecvMsg_WindowSize(LOWORD(lParam), HIWORD(lParam));

        if (iconified)
        {
            window->m_Iconified = 1;
            // TODOH
            //if (window->monitor)
            //    releaseMonitor(window);
        }
        else if (restored)
        {
            window->m_Iconified = 0;
            // TODOH
            //if (window->monitor)
            //    acquireMonitor(window);
        }

        return 0;
    }


    case WM_MOVE:
    {
        // TODOH
        //if (_glfw.win32.disabledCursorWindow == window)
        //    updateClipRect(window);

        // NOTE: This cannot use LOWORD/HIWORD recommended by MSDN, as
        // those macros do not handle negative window positions correctly
        window->OnRecvMsg_Pos(
            GET_X_LPARAM(lParam),
            GET_Y_LPARAM(lParam));

        return 0;
    }

    case WM_SIZING:
    {
        if (window->m_AspectRatioLimit.Numerator() == XWINDOW_DONT_CARE ||
            window->m_AspectRatioLimit.Denominator() == XWINDOW_DONT_CARE)
        {
            break;
        }

        // TODOM: impl this function
        // applyAspectRatio(window, (int)wParam, (RECT*)lParam);
        return TRUE;
    }

    case WM_GETMINMAXINFO:
    {
        //int xoff, yoff;
        //MINMAXINFO* mmi = (MINMAXINFO*)lParam;
        // TODOM
        //if (window->monitor)
        //    break;

        //getFullWindowSize(getWindowStyle(window), getWindowExStyle(window),
        //    0, 0, &xoff, &yoff);

        //if (window->minwidth != GLFW_DONT_CARE &&
        //    window->minheight != GLFW_DONT_CARE)
        //{
        //    mmi->ptMinTrackSize.x = window->minwidth + xoff;
        //    mmi->ptMinTrackSize.y = window->minheight + yoff;
        //}

        //if (window->maxwidth != GLFW_DONT_CARE &&
        //    window->maxheight != GLFW_DONT_CARE)
        //{
        //    mmi->ptMaxTrackSize.x = window->maxwidth + xoff;
        //    mmi->ptMaxTrackSize.y = window->maxheight + yoff;
        //}

        return 0;
    }

    case WM_PAINT:
    {
        window->OnRecvMsg_WindowNeedRefresh();
        break;
    }

    case WM_ERASEBKGND:
    {
        return TRUE;
    }

    case WM_SETCURSOR:
    {
        // TODOM

        //if (LOWORD(lParam) == HTCLIENT)
        //{
        //    updateCursorImage(window);
        //    return TRUE;
        //}

        break;
    }

    case WM_DPICHANGED:
    {
        RECT* rect = (RECT*)lParam;
        SetWindowPos(window->GetHWND(),
            HWND_TOP,
            rect->left,
            rect->top,
            rect->right - rect->left,
            rect->bottom - rect->top,
            SWP_NOACTIVATE | SWP_NOZORDER);

        break;
    }

    case WM_DROPFILES:
    {
        HDROP drop = (HDROP)wParam;
        POINT pt;
        int i;

        const int count = DragQueryFileW(drop, 0xffffffff, NULL, 0);
        char** paths = (char**)calloc(count, sizeof(char*));

        // Move the mouse to the position of the drop
        DragQueryPoint(drop, &pt);
        window->OnRecvMsg_CursorPos((float)pt.x, (float)pt.y);

        for (i = 0; i < count; i++)
        {
            const UINT length = DragQueryFileW(drop, i, NULL, 0);
            char* buffer = (char*)calloc(length + 1, sizeof(char));

            DragQueryFile(drop, i, buffer, length + 1);
            paths[i] = (buffer);

            free(buffer);
        }

        window->OnRecvMsg_Drop(count, (const char**)paths);

        for (i = 0; i < count; i++)
            free(paths[i]);
        free(paths);

        DragFinish(drop);
        return 0;
    }

    } // end of switch(uMsg)

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


ToyXResult WinBaseXWindowImpl::StartUp(const XWINDOW_API_STARTUP_DESC & desc)
{
    m_ProgramInstance = GetModuleHandle(NULL);

    _CreateKeycodeTables();

    if(_RegisterWindowsWindowClass() == false)
        return ToyXResult::Error_Win_RegisterWindowsWindowClass_Fail;

    // Creates a dummy window for behind-the-scenes work
    TOYXRESULT_RETURN_IF_FAIL(_CreateHelperWindow());

    // TODOM: see glfw:win32_init.c:L396

    return ToyXResult::Success;
}

void WinBaseXWindowImpl::ShutDown()
{
    _DestroyHelperWindow();
    _UnRegisterWindowsWindowClass();
}

void WinBaseXWindowImpl::GetPlatformDependentData(PlatformDependentData & data)
{
    data._.Windows.ProgramInstance = m_ProgramInstance;
    data._.Windows.MainWindowHWND = m_MainWindow ? m_MainWindow->GetHWND() : 0;
}

void WinBaseXWindowImpl::PollEvents()
{
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            // TODOH: handle WM_QUIT

            //// Treat WM_QUIT as a close on all windows
            //// While GLFW does not itself post WM_QUIT, other processes may post
            //// it to this one, for example Task Manager

            //window = _glfw.windowListHead;
            //while (window)
            //{
            //    _glfwInputWindowCloseRequest(window);
            //    window = window->next;
            //}
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // TODOH: more, see glfw:win32_window.c:L1350
}

void WinBaseXWindowImpl::WaitEvents()
{
    WaitMessage();

    PollEvents();
}

void WinBaseXWindowImpl::WaitEventsTimeout(float time)
{
    MsgWaitForMultipleObjects(0, NULL, FALSE, (DWORD)(time * 1e3), QS_ALLEVENTS);

    PollEvents();
}

void WinBaseXWindowImpl::PostEmptyEvent()
{
    if (m_MainWindow)
    {
        PostMessage(m_MainWindow->GetHWND(), WM_NULL, 0, 0);
    }
}

void WinBaseXWindowImpl::SetMainWindow(ToyUtility::SPtr<IWindow> window)
{
    m_MainWindow = std::dynamic_pointer_cast<WinWindow>(window);
}

bool WinBaseXWindowImpl::_RegisterWindowsWindowClass()
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WinBaseXWindomImpl_WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetProgramInstance();
    wcex.hIcon = 0; // TODOL: icon
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = WindowsWindowClassName.c_str();
    wcex.hIconSm = 0; // TODOL: small icon
    if (!RegisterClassEx(&wcex))
        return false;

    return true;
}

void WinBaseXWindowImpl::_UnRegisterWindowsWindowClass()
{
    UnregisterClass(WindowsWindowClassName.c_str(), GetProgramInstance());
}

ToyXResult WinBaseXWindowImpl::_CreateHelperWindow()
{
    m_HelperWindowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
        WindowsWindowClassName.c_str(),
        "ToyXWindow helper window",
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        0, 0, 1, 1,
        HWND_MESSAGE, NULL,
        GetProgramInstance(),
        NULL);
    if (!m_HelperWindowHandle)
    {
        return ToyXResult::Error_Win_CreateHelperWindow_Fail;
    }

    // HACK: The first call to ShowWindow is ignored if the parent process
    //       passed along a STARTUPINFO, so clear that flag with a no-op call
    ShowWindow(m_HelperWindowHandle, SW_HIDE);

    // Register for HID device notifications
    {
        DEV_BROADCAST_DEVICEINTERFACE dbi;
        ZeroMemory(&dbi, sizeof(dbi));
        dbi.dbcc_size = sizeof(dbi);
        dbi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        dbi.dbcc_classguid = GUID_DEVINTERFACE_HID;

        RegisterDeviceNotification(m_HelperWindowHandle,
            (DEV_BROADCAST_HDR*)&dbi,
            DEVICE_NOTIFY_WINDOW_HANDLE);
    }

    return ToyXResult::Success;
}

void WinBaseXWindowImpl::_DestroyHelperWindow()
{
    DestroyWindow(m_HelperWindowHandle);
}

void WinBaseXWindowImpl::_CreateKeycodeTables()
{
    memset(m_PublicKeys, -1, sizeof(m_PublicKeys));
    memset(m_NativeKeys, -1, sizeof(m_NativeKeys));

    m_PublicKeys[0x00B] = (ToyUtility::int16)KeyType::_0;
    m_PublicKeys[0x002] = (ToyUtility::int16)KeyType::_1;
    m_PublicKeys[0x003] = (ToyUtility::int16)KeyType::_2;
    m_PublicKeys[0x004] = (ToyUtility::int16)KeyType::_3;
    m_PublicKeys[0x005] = (ToyUtility::int16)KeyType::_4;
    m_PublicKeys[0x006] = (ToyUtility::int16)KeyType::_5;
    m_PublicKeys[0x007] = (ToyUtility::int16)KeyType::_6;
    m_PublicKeys[0x008] = (ToyUtility::int16)KeyType::_7;
    m_PublicKeys[0x009] = (ToyUtility::int16)KeyType::_8;
    m_PublicKeys[0x00A] = (ToyUtility::int16)KeyType::_9;
    m_PublicKeys[0x01E] = (ToyUtility::int16)KeyType::A;
    m_PublicKeys[0x030] = (ToyUtility::int16)KeyType::B;
    m_PublicKeys[0x02E] = (ToyUtility::int16)KeyType::C;
    m_PublicKeys[0x020] = (ToyUtility::int16)KeyType::D;
    m_PublicKeys[0x012] = (ToyUtility::int16)KeyType::E;
    m_PublicKeys[0x021] = (ToyUtility::int16)KeyType::F;
    m_PublicKeys[0x022] = (ToyUtility::int16)KeyType::G;
    m_PublicKeys[0x023] = (ToyUtility::int16)KeyType::H;
    m_PublicKeys[0x017] = (ToyUtility::int16)KeyType::I;
    m_PublicKeys[0x024] = (ToyUtility::int16)KeyType::J;
    m_PublicKeys[0x025] = (ToyUtility::int16)KeyType::K;
    m_PublicKeys[0x026] = (ToyUtility::int16)KeyType::L;
    m_PublicKeys[0x032] = (ToyUtility::int16)KeyType::M;
    m_PublicKeys[0x031] = (ToyUtility::int16)KeyType::N;
    m_PublicKeys[0x018] = (ToyUtility::int16)KeyType::O;
    m_PublicKeys[0x019] = (ToyUtility::int16)KeyType::P;
    m_PublicKeys[0x010] = (ToyUtility::int16)KeyType::Q;
    m_PublicKeys[0x013] = (ToyUtility::int16)KeyType::R;
    m_PublicKeys[0x01F] = (ToyUtility::int16)KeyType::S;
    m_PublicKeys[0x014] = (ToyUtility::int16)KeyType::T;
    m_PublicKeys[0x016] = (ToyUtility::int16)KeyType::U;
    m_PublicKeys[0x02F] = (ToyUtility::int16)KeyType::V;
    m_PublicKeys[0x011] = (ToyUtility::int16)KeyType::W;
    m_PublicKeys[0x02D] = (ToyUtility::int16)KeyType::X;
    m_PublicKeys[0x015] = (ToyUtility::int16)KeyType::Y;
    m_PublicKeys[0x02C] = (ToyUtility::int16)KeyType::Z;

    m_PublicKeys[0x028] = (ToyUtility::int16)KeyType::Apostrophe;
    m_PublicKeys[0x02B] = (ToyUtility::int16)KeyType::BackSlash;
    m_PublicKeys[0x033] = (ToyUtility::int16)KeyType::Comma;
    m_PublicKeys[0x00D] = (ToyUtility::int16)KeyType::Equal;
    m_PublicKeys[0x029] = (ToyUtility::int16)KeyType::GraveAccent;
    m_PublicKeys[0x01A] = (ToyUtility::int16)KeyType::LeftBracket;
    m_PublicKeys[0x00C] = (ToyUtility::int16)KeyType::Minus;
    m_PublicKeys[0x034] = (ToyUtility::int16)KeyType::Period;
    m_PublicKeys[0x01B] = (ToyUtility::int16)KeyType::RightBracket;
    m_PublicKeys[0x027] = (ToyUtility::int16)KeyType::Semicolon;
    m_PublicKeys[0x035] = (ToyUtility::int16)KeyType::Slash;
    //m_PublicKeys[0x056] = (ToyUtility::int16)KeyType::WORLD_2;

    m_PublicKeys[0x00E] = (ToyUtility::int16)KeyType::Backspace;
    m_PublicKeys[0x153] = (ToyUtility::int16)KeyType::Delete;
    m_PublicKeys[0x14F] = (ToyUtility::int16)KeyType::End;
    m_PublicKeys[0x01C] = (ToyUtility::int16)KeyType::Enter;
    m_PublicKeys[0x001] = (ToyUtility::int16)KeyType::Escape;
    m_PublicKeys[0x147] = (ToyUtility::int16)KeyType::Home;
    m_PublicKeys[0x152] = (ToyUtility::int16)KeyType::Insert;
    m_PublicKeys[0x15D] = (ToyUtility::int16)KeyType::Menu;
    m_PublicKeys[0x151] = (ToyUtility::int16)KeyType::PageDown;
    m_PublicKeys[0x149] = (ToyUtility::int16)KeyType::PageUp;
    m_PublicKeys[0x045] = (ToyUtility::int16)KeyType::Pause;
    m_PublicKeys[0x146] = (ToyUtility::int16)KeyType::Pause;
    m_PublicKeys[0x039] = (ToyUtility::int16)KeyType::Space;
    m_PublicKeys[0x00F] = (ToyUtility::int16)KeyType::Tab;
    m_PublicKeys[0x03A] = (ToyUtility::int16)KeyType::CapsLock;
    m_PublicKeys[0x145] = (ToyUtility::int16)KeyType::NumberLock;
    m_PublicKeys[0x046] = (ToyUtility::int16)KeyType::ScrollLock;
    m_PublicKeys[0x03B] = (ToyUtility::int16)KeyType::F1;
    m_PublicKeys[0x03C] = (ToyUtility::int16)KeyType::F2;
    m_PublicKeys[0x03D] = (ToyUtility::int16)KeyType::F3;
    m_PublicKeys[0x03E] = (ToyUtility::int16)KeyType::F4;
    m_PublicKeys[0x03F] = (ToyUtility::int16)KeyType::F5;
    m_PublicKeys[0x040] = (ToyUtility::int16)KeyType::F6;
    m_PublicKeys[0x041] = (ToyUtility::int16)KeyType::F7;
    m_PublicKeys[0x042] = (ToyUtility::int16)KeyType::F8;
    m_PublicKeys[0x043] = (ToyUtility::int16)KeyType::F9;
    m_PublicKeys[0x044] = (ToyUtility::int16)KeyType::F10;
    m_PublicKeys[0x057] = (ToyUtility::int16)KeyType::F11;
    m_PublicKeys[0x058] = (ToyUtility::int16)KeyType::F12;
    m_PublicKeys[0x064] = (ToyUtility::int16)KeyType::F13;
    m_PublicKeys[0x065] = (ToyUtility::int16)KeyType::F14;
    m_PublicKeys[0x066] = (ToyUtility::int16)KeyType::F15;
    m_PublicKeys[0x067] = (ToyUtility::int16)KeyType::F16;
    m_PublicKeys[0x068] = (ToyUtility::int16)KeyType::F17;
    m_PublicKeys[0x069] = (ToyUtility::int16)KeyType::F18;
    m_PublicKeys[0x06A] = (ToyUtility::int16)KeyType::F19;
    m_PublicKeys[0x06B] = (ToyUtility::int16)KeyType::F20;
    m_PublicKeys[0x06C] = (ToyUtility::int16)KeyType::F21;
    m_PublicKeys[0x06D] = (ToyUtility::int16)KeyType::F22;
    m_PublicKeys[0x06E] = (ToyUtility::int16)KeyType::F23;
    m_PublicKeys[0x076] = (ToyUtility::int16)KeyType::F24;
    m_PublicKeys[0x038] = (ToyUtility::int16)KeyType::LeftAlt;
    m_PublicKeys[0x01D] = (ToyUtility::int16)KeyType::LeftControl;
    m_PublicKeys[0x02A] = (ToyUtility::int16)KeyType::LeftShift;
    m_PublicKeys[0x15B] = (ToyUtility::int16)KeyType::LeftSuper;
    m_PublicKeys[0x137] = (ToyUtility::int16)KeyType::PrintScreen;
    m_PublicKeys[0x138] = (ToyUtility::int16)KeyType::RightAlt;
    m_PublicKeys[0x11D] = (ToyUtility::int16)KeyType::RightControl;
    m_PublicKeys[0x036] = (ToyUtility::int16)KeyType::RightShift;
    m_PublicKeys[0x15C] = (ToyUtility::int16)KeyType::RightSuper;
    m_PublicKeys[0x150] = (ToyUtility::int16)KeyType::Down;
    m_PublicKeys[0x14B] = (ToyUtility::int16)KeyType::Left;
    m_PublicKeys[0x14D] = (ToyUtility::int16)KeyType::Right;
    m_PublicKeys[0x148] = (ToyUtility::int16)KeyType::Up;

    m_PublicKeys[0x052] = (ToyUtility::int16)KeyType::Keypad_0;
    m_PublicKeys[0x04F] = (ToyUtility::int16)KeyType::Keypad_1;
    m_PublicKeys[0x050] = (ToyUtility::int16)KeyType::Keypad_2;
    m_PublicKeys[0x051] = (ToyUtility::int16)KeyType::Keypad_3;
    m_PublicKeys[0x04B] = (ToyUtility::int16)KeyType::Keypad_4;
    m_PublicKeys[0x04C] = (ToyUtility::int16)KeyType::Keypad_5;
    m_PublicKeys[0x04D] = (ToyUtility::int16)KeyType::Keypad_6;
    m_PublicKeys[0x047] = (ToyUtility::int16)KeyType::Keypad_7;
    m_PublicKeys[0x048] = (ToyUtility::int16)KeyType::Keypad_8;
    m_PublicKeys[0x049] = (ToyUtility::int16)KeyType::Keypad_9;
    m_PublicKeys[0x04E] = (ToyUtility::int16)KeyType::Keypad_Add;
    m_PublicKeys[0x053] = (ToyUtility::int16)KeyType::Keypad_Decimal;
    m_PublicKeys[0x135] = (ToyUtility::int16)KeyType::Keypad_Divide;
    m_PublicKeys[0x11C] = (ToyUtility::int16)KeyType::Keypad_Enter;
    m_PublicKeys[0x037] = (ToyUtility::int16)KeyType::Keypad_Multiply;
    m_PublicKeys[0x04A] = (ToyUtility::int16)KeyType::Keypad_Subtact;

    for (int scancode = 0; scancode < 512; scancode++)
    {
        if (m_PublicKeys[scancode] > 0)
            m_NativeKeys[m_PublicKeys[scancode]] = scancode;
    }
}

KeyType WinBaseXWindowImpl::_TranslateKey(WPARAM wParam, LPARAM lParam)
{
    if (wParam == VK_CONTROL)
    {
        // The CTRL keys require special handling

        MSG next;
        DWORD time;

        // Is this an extended key (i.e. right key)?
        if (lParam & 0x01000000)
            return KeyType::RightControl;

        // Here is a trick: "Alt Gr" sends LCTRL, then RALT. We only
        // want the RALT message, so we try to see if the next message
        // is a RALT message. In that case, this is a false LCTRL!
        time = GetMessageTime();

        if (PeekMessageW(&next, NULL, 0, 0, PM_NOREMOVE))
        {
            if (next.message == WM_KEYDOWN ||
                next.message == WM_SYSKEYDOWN ||
                next.message == WM_KEYUP ||
                next.message == WM_SYSKEYUP)
            {
                if (next.wParam == VK_MENU &&
                    (next.lParam & 0x01000000) &&
                    next.time == time)
                {
                    // Next message is a RALT down message, which
                    // means that this is not a proper LCTRL message
                    return KeyType::__Invalid;
                }
            }
        }

        return KeyType::LeftControl;
    }

    if (wParam == VK_PROCESSKEY)
    {
        // IME notifies that keys have been filtered by setting the virtual
        // key-code to VK_PROCESSKEY
        return KeyType::__Invalid;
    }

    // TODOH: why use lParam, rather than Window VK wParam ?
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms646280(v=vs.85).aspx
    return (KeyType)m_PublicKeys[HIWORD(lParam) & 0x1FF];
}

KeyType WinBaseXWindowImpl::NativeKeyToPublicKey(int32 scancode)
{
    throw "unachieved function called"; // TODOL
}

int32 WinBaseXWindowImpl::PublicKeyToNativeKey(KeyType key)
{
    throw "unachieved function called"; // TODOL
}

int WinBaseXWindowImpl::GetKeyModifiers()
{
    int mods = 0;

    if (GetKeyState(VK_SHIFT) & (1 << 31))
        mods |= (int)ModifierKeyType::Shift;
    if (GetKeyState(VK_CONTROL) & (1 << 31))
        mods |= (int)ModifierKeyType::Control;
    if (GetKeyState(VK_MENU) & (1 << 31))
        mods |= (int)ModifierKeyType::Alt;
    if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & (1 << 31))
        mods |= (int)ModifierKeyType::Super;

    return mods;
}


} // end of namespace ToyXWindow
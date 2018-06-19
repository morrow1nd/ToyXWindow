#include "ToyXWindow/details/WinWindow.h"


namespace ToyXWindow
{


// TODOM: see glfw:win32_window.c:L1199
#define getWindowStyle_     WS_CLIPSIBLINGS | WS_CLIPCHILDREN
#define getWindowExStyle_   WS_EX_APPWINDOW


ToyUtility::String WinWindow::HWNDGetPropName = "ToyXWindow";


void WinWindow::SetClassName(const ToyUtility::String & className)
{
    m_ClassName = className;
}

ToyXResult WinWindow::Create(const WINDOW_DESC& desc)
{
    TOYXRESULT_RETURN_IF_FAIL(BaseWindow::Create(desc));

    m_Desc = desc;

    // Create window
    RECT rc = {desc.WindowRect.x, desc.WindowRect.y, (LONG)desc.WindowRect.width, (LONG)desc.WindowRect.height};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    m_HWND = CreateWindowA(
        m_ClassName.c_str(),
        desc.Title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        NULL,
        NULL,
        m_ProgramInstance,
        NULL);
    if (!m_HWND)
    {
        // auto error = GetLastError();
        return ToyXResult::Error_Win_CreateWindowA_Fail;
    }

    SetPropA(m_HWND, HWNDGetPropName.c_str(), this);

    m_HWNDCreated = true;

    Show();

    TOYXRESULT_RETURN_IF_FAIL(InitContext()); // TODOH: if failed, release m_HWND

    return ToyXResult::Success;
}

void WinWindow::Destory()
{
    // TODOH DestoryWindow
}

void WinWindow::GetWindowSize(uint32 * width, uint32 * height)
{
    RECT area;
    GetClientRect(m_HWND, &area);

    if (width)
        *width = area.right;
    if (height)
        *height = area.bottom;
}

void WinWindow::SetWindowSize(uint32 width, uint32 height)
{
    if (IsWindowed())
    {
        RECT rect = {0, 0, (LONG)width, (LONG)height};
        AdjustWindowRectEx(&rect, getWindowStyle_,
            FALSE, getWindowExStyle_);
        SetWindowPos(m_HWND, HWND_TOP,
            0, 0, rect.right - rect.left, rect.bottom - rect.top,
            SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
    }
}

void WinWindow::GetWindowFrameSize(uint32 * left, uint32 * top, uint32 * right, uint32 * bottom)
{
    RECT rect;
    uint32 width, height;

    GetWindowSize(&width, &height);
    SetRect(&rect, 0, 0, width, height);
    AdjustWindowRectEx(&rect, getWindowStyle_,
        FALSE, getWindowExStyle_);

    if (left)
        *left = -rect.left;
    if (top)
        *top = -rect.top;
    if (right)
        *right = rect.right - width;
    if (bottom)
        *bottom = rect.bottom - height;
}

void WinWindow::GetFrameBufferSize(uint32 * width, uint32 * height)
{
    // TODOH
}

void WinWindow::SetWindowSizeLimits(uint32 minWidth, uint32 minHeight, uint32 maxWidth, uint32 maxHeight)
{
    // TODOH
}

void WinWindow::SetAspectRatio(uint32 width, uint32 height)
{
    // TODOH
}

void WinWindow::SetPos(int32 xpos, int32 ypos)
{
    RECT rect = {xpos, ypos, xpos, ypos};
    AdjustWindowRectEx(&rect, getWindowStyle_,
        FALSE, getWindowExStyle_);
    SetWindowPos(m_HWND, NULL, rect.left, rect.top, 0, 0,
        SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}

void WinWindow::GetPos(int32 * xpos, int32 * ypos)
{
    POINT pos = {0, 0};
    ClientToScreen(m_HWND, &pos);

    if (xpos)
        *xpos = pos.x;
    if (ypos)
        *ypos = pos.y;
}

void WinWindow::SetTitle(const ToyUtility::String & title)
{
    BaseWindow::SetTitle(title);

    SetWindowText(m_HWND, title.c_str());
    //SetWindowTextW(m_HWND, title.c_str()); // TODOM
}

void WinWindow::Iconify()
{
    ShowWindow(m_HWND, SW_MINIMIZE);
}

void WinWindow::Restore()
{
    ShowWindow(m_HWND, SW_RESTORE);
}

bool WinWindow::IsIconified()
{
    return IsIconic(m_HWND);
}

void WinWindow::Hide()
{
    ShowWindow(m_HWND, SW_HIDE);
}

void WinWindow::Show()
{
    ShowWindow(m_HWND, SW_SHOW);
}

bool WinWindow::IsVisible()
{
    return IsWindowVisible(m_HWND);
}

void WinWindow::Focus()
{
    BringWindowToTop(m_HWND);
    SetForegroundWindow(m_HWND);
    SetFocus(m_HWND);
}

bool WinWindow::IsFocused()
{
    return m_HWND == GetActiveWindow();
}

ButtonState WinWindow::GetKey(KeyType key)
{
    return ButtonState();
}

const ToyUtility::String & WinWindow::GetKeyName(KeyType key) const
{
    //WCHAR name[16];

    //if (key != GLFW_KEY_UNKNOWN)
    //    scancode = _glfw.win32.nativeKeys[key];

    //if (!_glfwIsPrintable(_glfw.win32.publicKeys[scancode]))
    //    return NULL;

    //if (!GetKeyNameTextW(scancode << 16, name, sizeof(name) / sizeof(WCHAR)))
    //    return NULL;

    //if (!WideCharToMultiByte(CP_UTF8, 0, name, -1,
    //    _glfw.win32.keyName,
    //    sizeof(_glfw.win32.keyName),
    //    NULL, NULL))
    //{
    //    return NULL;
    //}

    //return _glfw.win32.keyName;
    // TODOM
    throw "unachieved";
}

void WinWindow::GetCursorPos(float * xpos, float * ypos)
{
    POINT pos;

    if (::GetCursorPos(&pos))
    {
        ScreenToClient(m_HWND, &pos);

        if (xpos)
            *xpos = (float)pos.x;
        if (ypos)
            *ypos = (float)pos.y;
    }
}

void WinWindow::SetCursorPos(float xpos, float ypos)
{
    POINT pos = {(int)xpos, (int)ypos};

    // TODOM: ?
    // Store the new position so it can be recognized later
    //window->win32.lastCursorPosX = pos.x;
    //window->win32.lastCursorPosY = pos.y;

    ClientToScreen(m_HWND, &pos);
    SetCursorPos((float)pos.x, (float)pos.y);

    OnRecvMsg_CursorPos(xpos, ypos); // TODOH: need this? Or do the system emit win event to WndProc?
}

void WinWindow::GetCursorPosDelta(float * xdelta, float * ydelta)
{
}

void WinWindow::SetCursorMode(CursorMode mode)
{
    // TODOH

    //if (mode == CursorMode::Disabled)
    //{
    //    _glfw.win32.disabledCursorWindow = window;
    //    _glfwPlatformGetCursorPos(window,
    //        &_glfw.win32.restoreCursorPosX,
    //        &_glfw.win32.restoreCursorPosY);
    //    centerCursor(window);
    //    updateClipRect(window);
    //}
    //else if (_glfw.win32.disabledCursorWindow == window)
    //{
    //    _glfw.win32.disabledCursorWindow = NULL;
    //    updateClipRect(NULL);
    //    _glfwPlatformSetCursorPos(window,
    //        _glfw.win32.restoreCursorPosX,
    //        _glfw.win32.restoreCursorPosY);
    //}

    //if (cursorInClientArea(window))
    //    updateCursorImage(window);
}

CursorMode WinWindow::GetCursorMode()
{
    // TODOH
    return CursorMode::Normal;
}

ButtonState WinWindow::GetMouseButton(MouseButtonType type)
{
    // TODOH
    return ButtonState();
}

const ToyUtility::String & WinWindow::GetClipboardString() const
{
    // TODOM
    throw "unachieved";
}

void WinWindow::SetClipboardString(const ToyUtility::String & text)
{
    // TODOM
    throw "unachieved";
}


} // end of namespace ToyXWindow
#include "ToyXWindow/details/WinWindow.h"


namespace ToyXWindow
{

void WinWindow::SetClassName(const ToyUtility::String & className)
{
    m_ClassName = className;
}

bool WinWindow::Create(const WINDOW_DESC& desc)
{
    m_Desc = desc;

    // Create window
    RECT rc = {desc.WindowRect.x, desc.WindowRect.y, desc.WindowRect.width, desc.WindowRect.height};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    m_HWND = CreateWindow(m_ClassName.c_str(), desc.Title.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, GetModuleHandle(NULL),
        NULL);
    if (!m_HWND)
        return false;

    SetProp(m_HWND, "ToyXW", this);

    Show();

    // TODOH: create context: dx11, wgl

    return true;
}

void WinWindow::Destory()
{
    // TODOH
}

void WinWindow::GetWindowSize(uint32 * width, uint32 * height)
{

}

void WinWindow::SetWindowSize(uint32 width, uint32 height)
{
}

void WinWindow::GetWindowFrameSize(uint32 * left, uint32 * top, uint32 * right, uint32 * bottom)
{
}

void WinWindow::GetFrameBufferSize(uint32 * width, uint32 * height)
{
}

void WinWindow::SetWindowSizeLimits(uint32 minWidth, uint32 minHeight, uint32 maxWidth, uint32 maxHeight)
{
}

void WinWindow::SetAspectRatio(uint32 width, uint32 height)
{
}

void WinWindow::SetPos(int32 xpos, int32 ypos)
{
}

void WinWindow::GetPos(int32 * xpos, int32 * pos)
{
}

const ToyUtility::String & WinWindow::GetTitle() const
{
    // TODO: insert return statement here
}

void WinWindow::SetTitle(const ToyUtility::String & title)
{
    SetWindowText(m_HWND, title.c_str());
    //SetWindowTextW(m_HWND, title.c_str());
}

void WinWindow::Iconify()
{
}

void WinWindow::Restore()
{
}

bool WinWindow::IsIconified()
{
    return false;
}

void WinWindow::Hide()
{
}

void WinWindow::Show()
{
}

bool WinWindow::IsVisible()
{
    return false;
}

void WinWindow::Focus()
{
}

bool WinWindow::IsFocused()
{
    return false;
}

void WinWindow::PresentBackBuffer(uint32 syncInterval)
{
}

ButtonState WinWindow::GetKey(KeyType key)
{
    return ButtonState();
}

const ToyUtility::String & WinWindow::GetKeyName(KeyType key) const
{
    // TODO: insert return statement here
}

void WinWindow::GetCursorPos(float * xpos, float * ypos)
{
}

void WinWindow::GetCursorPosDelta(float * xdelta, float * ydelta)
{
}

void WinWindow::SetCursorMode(CursorMode mode)
{
}

CursorMode WinWindow::GetCursorMode()
{
    return CursorMode();
}

ButtonState WinWindow::GetMouseButton(MouseButtonType type)
{
    return ButtonState();
}

const ToyUtility::String & WinWindow::GetClipboardString() const
{
    // TODO: insert return statement here
}

void WinWindow::SetClipboardString(const ToyUtility::String & text)
{
}


} // end of namespace ToyXWindow
#include "ToyXWindow/PlatformXWindow.h"

#include <Windows.h>
#include "ToyXWindow/details/WinWindow.h"


namespace ToyXWindow
{


// Window callback function (handles window messages)
//
static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam)
{
    WinWindow* window = (WinWindow*)GetProp(hWnd, "ToyXW");

    if(window == nullptr)
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    switch (uMsg)
    {
    case WM_SETFOCUS:
    {
        window->FocusEmit(true);

        return 0;
    }

    case WM_KILLFOCUS:
    {
        window->FocusEmit(false);

        return 0;
    }

    case WM_SYSCOMMAND:
    {
        switch (wParam & 0xfff0)
        {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
        {
            // TODOM
        }
        }
    }
    
    case WM_CLOSE:
    {
        window->ShouldCloseEmit();
        return 0;
    }

    case WM_CHAR:
    case WM_SYSCHAR:
    case WM_UNICHAR:
    {
        // TODOM: input char
    }

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        const int key = translateKey(wParam, lParam);
        const int scancode = (lParam >> 16) & 0x1ff;
        const int action = ((lParam >> 31) & 1) ? KeyAction::Release : KeyAction::Press;
        const int mods = getKeyMods();

        if (key == _GLFW_KEY_INVALID)
            break;

        if (action == GLFW_RELEASE && wParam == VK_SHIFT)
        {
            // Release both Shift keys on Shift up event, as only one event
            // is sent even if both keys are released
            _glfwInputKey(window, GLFW_KEY_LEFT_SHIFT, scancode, action, mods);
            _glfwInputKey(window, GLFW_KEY_RIGHT_SHIFT, scancode, action, mods);
        }
        else if (wParam == VK_SNAPSHOT)
        {
            // Key down is not reported for the Print Screen key
            _glfwInputKey(window, key, scancode, GLFW_PRESS, mods);
            _glfwInputKey(window, key, scancode, GLFW_RELEASE, mods);
        }
        else
            _glfwInputKey(window, key, scancode, action, mods);

        break;
    }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

bool _RegristerWindowClass()
{
    ToyUtility::String className = PlatformXWindow::XWindowClassName;

    // TODOH: register class only once

    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = windowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.hIcon = 0; // TODOL: icon
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = className.c_str();
    wcex.hIconSm = 0; // TODOL: small icon
    if (!RegisterClassEx(&wcex))
        return false;

    return true;
}


ToyUtility::String PlatformXWindow::XWindowClassName = "ToyXWindowClassName";


bool PlatformXWindow::_PlatformStartUp()
{
    if(_RegristerWindowClass() == false)
        return false;



    return true;
}

void PlatformXWindow::ShutDown()
{
}

void PlatformXWindow::PollEvents()
{
}

void PlatformXWindow::WaitEvents()
{
}

void PlatformXWindow::WaitEventsTimeout(float time)
{
}

void PlatformXWindow::PostEmptyEvent()
{
}

ToyUtility::SPtr<IAdapter> PlatformXWindow::GetDefaultAdapter()
{
    return ToyUtility::SPtr<IAdapter>();
}

const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& PlatformXWindow::GetAdapters() const
{
    // TODO: insert return statement here
}

#ifdef CreateWindow
# undef CreateWindow
#endif

ToyUtility::SPtr<IWindow> PlatformXWindow::CreateWindow(const WINDOW_DESC& desc)
{
    ToyUtility::SPtr<WinWindow> window = ToyUtility::SPtr<WinWindow>(new WinWindow());

    window->SetClassName(XWindowClassName);

    if(window->Create(desc))
        return window;

    return ToyUtility::SPtr<IWindow>();
}


} // end of namespace ToyXWindow
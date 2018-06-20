#include <iostream>
#include "ToyXWindow/XWindowAPI.h"

#include <Windows.h> // Sleep
#undef CreateWindow

using namespace ToyXWindow;


int main()
{
    auto& api = XWindowAPI::Instance();

    XWINDOW_API_STARTUP_DESC desc;
    desc.WindowContextNativeApi = WindowContextNativeApiType::Win_DX11;

    auto res = api.StartUp(desc);

    if (res != ToyXResult::Success)
    {
        std::cout << "XWindowAPI StartUp fail: " << (int)res << std::endl;
        return -1;
    }

    WINDOW_DESC wd;
    wd.Title = "One Window";
    wd.Windowed = true;
    wd.WindowRect = ToyUtility::Rect2I(0, 0, 800, 600);
    wd.BufferCount = 1;
    wd.BufferDesc.Height = 600;
    wd.BufferDesc.Width = 800;
    wd.BufferDesc.RefreshRate = ToyUtility::Rational(1, 60);
    wd.SampleDesc = SampleDesc::NoMultiSampling;
    auto window = api.CreateWindow(wd);
    if (window == nullptr)
    {
        std::cout << "Create window fail" << std::endl;
        return -1;
    }

    window->SetMouseButtonCallback([] (IWindow* w, MouseButtonFuncArguList)
    {
        float x,y;
        w->GetCursorPos(&x, &y);
        std::cout << "mouse button: " << x << "," << y << std::endl;
    });
    
    while (window->ShouldClose() == false)
    {
        Sleep(33);

        window->PresentBackBuffer(0);
        api.PollEvents();
    }

    api.ShutDown();

    return 0;
}
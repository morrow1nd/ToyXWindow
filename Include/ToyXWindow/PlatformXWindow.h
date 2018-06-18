#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyUtility/DesignPattern/ISingleton.h"
#include "ToyUtility/Memory/SmartPtr.h"
#include "ToyUtility/Container/List.h"
#include "ToyXWindow/CommonType.h"
#include "ToyXWindow/IWindow.h"
#include "ToyXWindow/IAdapter.h"


namespace ToyXWindow
{


class PlatformXWindow
{
public:
    static ToyUtility::String XWindowClassName;


private:
    XWINDOW_API_STARTUP_DESC m_Desc;

    ToyUtility::SPtr<IWindow> m_MainWindow;


public:
    // Common Impl
    bool StartUp(const XWINDOW_API_STARTUP_DESC& desc);
    void SetMainWindow(ToyUtility::SPtr<IWindow> window);


    // Platform-specify Impl
    bool _PlatformStartUp();
    void ShutDown();
    void PollEvents();
    void WaitEvents();
    void WaitEventsTimeout(float time);
    void PostEmptyEvent();
    ToyUtility::SPtr<IAdapter> GetDefaultAdapter();
    const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& GetAdapters() const;
    ToyUtility::SPtr<IWindow> CreateWindow(const WINDOW_DESC& desc);
};


} // end of namespace ToyXWindow
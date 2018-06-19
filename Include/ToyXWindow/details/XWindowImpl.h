#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyUtility/Memory/SmartPtr.h"
#include "ToyXWindow/CommonType.h"
#include "ToyXWindow/IAdapter.h"
#include "ToyXWindow/IWindow.h"

#ifdef CreateWindow
# undef CreateWindow
#endif


namespace ToyXWindow
{


class XWindowImpl
{
public:
    static ToyUtility::SPtr<XWindowImpl> FactoryCreate(WindowContextNativeApiType type);


public:
    virtual ToyXResult StartUp(const XWINDOW_API_STARTUP_DESC& desc) = 0;
    virtual void ShutDown() = 0;

    virtual void PollEvents() = 0;
    virtual void WaitEvents() = 0;
    virtual void WaitEventsTimeout(float time) = 0;
    virtual void PostEmptyEvent() = 0;

    virtual ToyUtility::SPtr<IAdapter> GetDefaultAdapter() = 0;
    virtual const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& GetAdapters() const = 0;
    virtual ToyUtility::SPtr<IWindow> CreateWindow(const WINDOW_DESC& desc) = 0;
    virtual void SetMainWindow(ToyUtility::SPtr<IWindow> window) = 0;
};


} // end of namespace ToyXWindow
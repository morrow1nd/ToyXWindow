#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyXWindow/details/WinBaseXWindowImpl.h"
#include "ToyXWindow/details/WinWGLCommonType.h"
#include "ToyXWindow/details/WinWGLWindow.h"


namespace ToyXWindow
{


class WinWGLXWindowImpl : public WinBaseXWindowImpl
{
public:
    WinWGLXWindowImpl()
    {}


private:
    WinWGLContext                       m_Context;
    ToyUtility::SPtr<WinWGLWindow>      m_MainWindow;


private:
    ToyXResult _InitializeWGL();
    void _TerminateWGL();

    bool _WGLExtensionSupported(const char* extension);
    bool _StringInExtensionString(const char* string, const char* extensions);


public:
    virtual ToyXResult StartUp(const XWINDOW_API_STARTUP_DESC & desc) override;
    virtual void ShutDown() override;
    virtual ToyUtility::SPtr<IAdapter> GetDefaultAdapter() override;
    virtual const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& GetAdapters() const override;
    virtual ToyUtility::SPtr<IWindow> CreateWindow(const WINDOW_DESC & desc) override;
    virtual void SetMainWindow(ToyUtility::SPtr<IWindow> window) override;
};


} // end of namespace ToyXWindow
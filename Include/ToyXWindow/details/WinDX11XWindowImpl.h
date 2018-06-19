#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyXWindow/details/WinBaseXWindowImpl.h"
#include "ToyXWindow/details/WinDX11CommonType.h"
#include "ToyXWindow/details/WinDX11Window.h"


namespace ToyXWindow
{


class WinDX11XWindowImpl : public WinBaseXWindowImpl
{
public:
    WinDX11XWindowImpl()
        :
        m_MainWindow(nullptr)
    {}


private:
    WinDX11Context m_Context;
    ToyUtility::SPtr<WinDX11Window> m_MainWindow;


private:
    ToyXResult _CreateDXGIFactory();
    void _DestoryDXGIFactory();
    ToyXResult _CreateD3DDevice();
    void _DestoryD3DDevice();


public:
    virtual ToyXResult StartUp(const XWINDOW_API_STARTUP_DESC & desc) override;
    virtual void ShutDown() override;
    virtual ToyUtility::SPtr<IAdapter> GetDefaultAdapter() override;
    virtual const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& GetAdapters() const override;
    virtual ToyUtility::SPtr<IWindow> CreateWindow(const WINDOW_DESC & desc) override;
    virtual void SetMainWindow(ToyUtility::SPtr<IWindow> window) override;
};


} // end of namespace ToyXWindow
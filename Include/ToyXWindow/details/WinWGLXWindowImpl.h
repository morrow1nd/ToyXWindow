#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyXWindow/details/WinBaseXWindowImpl.h"


namespace ToyXWindow
{


class WinWGLXWindowImpl : public WinBaseXWindowImpl
{
public:
    virtual void ShutDown() override;
    virtual ToyUtility::SPtr<IAdapter> GetDefaultAdapter() override;
    virtual const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& GetAdapters() const override;
    virtual ToyUtility::SPtr<IWindow> CreateWindow(const WINDOW_DESC & desc) override;
};


} // end of namespace ToyXWindow
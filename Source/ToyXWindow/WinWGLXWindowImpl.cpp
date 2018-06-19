#include "ToyXWindow/details/WinWGLXWindowImpl.h"


namespace ToyXWindow
{




void WinWGLXWindowImpl::ShutDown()
{
}

ToyUtility::SPtr<IAdapter> WinWGLXWindowImpl::GetDefaultAdapter()
{
    return ToyUtility::SPtr<IAdapter>();
}

const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& WinWGLXWindowImpl::GetAdapters() const
{
    // TODO: insert return statement here
static    ToyUtility::List<ToyUtility::SPtr<IAdapter>> a;
return a;
}

ToyUtility::SPtr<IWindow> WinWGLXWindowImpl::CreateWindow(const WINDOW_DESC & desc)
{
    return ToyUtility::SPtr<IWindow>();
}

} // end of namespace ToyXWindow
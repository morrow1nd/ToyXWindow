#include "ToyXWindow/XWindowAPI.h"


namespace ToyXWindow
{


void XWindowAPI::StartUp(const XWINDOW_API_STARTUP_DESC& desc)
{

}

void XWindowAPI::ShutDown()
{
}

void XWindowAPI::PollEvents()
{
}

void XWindowAPI::WaitEvents()
{
}

void XWindowAPI::WaitEventsTimeout(float time)
{
}

void XWindowAPI::PostEmptyEvent()
{
}

ToyUtility::SPtr<IAdapter> XWindowAPI::GetDefaultAdapter()
{
    return ToyUtility::SPtr<IAdapter>();
}

const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& XWindowAPI::GetAdapters() const
{
    static ToyUtility::List<ToyUtility::SPtr<IAdapter>> a;
    return a;
}

ToyUtility::SPtr<IWindow> XWindowAPI::CreateWindow()
{
    return ToyUtility::SPtr<IWindow>();
}


} // end of namespace ToyXWindow
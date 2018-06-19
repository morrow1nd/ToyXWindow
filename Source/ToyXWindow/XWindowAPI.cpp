#include "ToyXWindow/XWindowAPI.h"


namespace ToyXWindow
{


ToyXResult XWindowAPI::StartUp(const XWINDOW_API_STARTUP_DESC& desc)
{
    m_PlatformImpl = XWindowImpl::FactoryCreate(desc.WindowContextNativeApi);

    if (m_PlatformImpl)
    {
        return m_PlatformImpl->StartUp(desc);
    }

    return ToyXResult::Error_XWindowImpl_FactoryCreate_Fail;
}

void XWindowAPI::ShutDown()
{
    m_PlatformImpl->ShutDown();
}

void XWindowAPI::PollEvents()
{
    m_PlatformImpl->PollEvents();
}

void XWindowAPI::WaitEvents()
{
    m_PlatformImpl->WaitEvents();
}

void XWindowAPI::WaitEventsTimeout(float time)
{
    m_PlatformImpl->WaitEventsTimeout(time);
}

void XWindowAPI::PostEmptyEvent()
{
    m_PlatformImpl->PostEmptyEvent();
}

ToyUtility::SPtr<IAdapter> XWindowAPI::GetDefaultAdapter()
{
    return m_PlatformImpl->GetDefaultAdapter();
}

const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& XWindowAPI::GetAdapters() const
{
    return m_PlatformImpl->GetAdapters();
}

ToyUtility::SPtr<IWindow> XWindowAPI::CreateWindow(const WINDOW_DESC& desc)
{
    return m_PlatformImpl->CreateWindow(desc);
}

void XWindowAPI::SetMainWindow(ToyUtility::SPtr<IWindow> window)
{
    m_PlatformImpl->SetMainWindow(window);
}


} // end of namespace ToyXWindow
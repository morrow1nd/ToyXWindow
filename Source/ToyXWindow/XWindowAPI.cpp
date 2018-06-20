#include "ToyXWindow/XWindowAPI.h"


namespace ToyXWindow
{


XWindowAPI::XWindowAPI()
    :
    m_StartUpSucc(false)
{
    // Nothing
}

ToyXResult XWindowAPI::StartUp(const XWINDOW_API_STARTUP_DESC& desc)
{
    m_PlatformImpl = XWindowImpl::FactoryCreate(desc.WindowContextNativeApi);

    if (m_PlatformImpl)
    {
        auto res = m_PlatformImpl->StartUp(desc);

        if (res == ToyXResult::Success)
        {
            m_StartUpSucc = true;
        }

        return res;
    }

    return ToyXResult::Error_XWindowImpl_FactoryCreate_Fail;
}

void XWindowAPI::ShutDown()
{
    if (!m_StartUpSucc)
        return;

    m_PlatformImpl->ShutDown();

    m_StartUpSucc = false;
}

void XWindowAPI::GetPlatformDependentData(PlatformDependentData & data)
{
    if(!m_StartUpSucc)
        return;

    m_PlatformImpl->GetPlatformDependentData(data);
}

void XWindowAPI::PollEvents()
{
    if (!m_StartUpSucc)
        return;

    m_PlatformImpl->PollEvents();
}

void XWindowAPI::WaitEvents()
{
    if (!m_StartUpSucc)
        return;

    m_PlatformImpl->WaitEvents();
}

void XWindowAPI::WaitEventsTimeout(float time)
{
    if (!m_StartUpSucc)
        return;

    m_PlatformImpl->WaitEventsTimeout(time);
}

void XWindowAPI::PostEmptyEvent()
{
    if (!m_StartUpSucc)
        return;

    m_PlatformImpl->PostEmptyEvent();
}

ToyUtility::SPtr<IAdapter> XWindowAPI::GetDefaultAdapter()
{
    if (!m_StartUpSucc)
        return ToyUtility::SPtr<IAdapter>();

    return m_PlatformImpl->GetDefaultAdapter();
}

const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& XWindowAPI::GetAdapters() const
{
    if (!m_StartUpSucc)
    {
        static ToyUtility::List<ToyUtility::SPtr<IAdapter>> empty;
        return empty;
    }

    return m_PlatformImpl->GetAdapters();
}

ToyUtility::SPtr<IWindow> XWindowAPI::CreateWindow(const WINDOW_DESC& desc)
{
    if (!m_StartUpSucc)
        return ToyUtility::SPtr<IWindow>();

    return m_PlatformImpl->CreateWindow(desc);
}

void XWindowAPI::SetMainWindow(ToyUtility::SPtr<IWindow> window)
{
    if (!m_StartUpSucc)
        return;

    m_PlatformImpl->SetMainWindow(window);
}


} // end of namespace ToyXWindow
#include "ToyXWindow/details/WinDX11XWindowImpl.h"


namespace ToyXWindow
{

ToyXResult WinDX11XWindowImpl::_CreateDXGIFactory()
{
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_Context.Factory);
    if (FAILED(hr))
    {
        return ToyXResult::Error_WinDX11_CreateDXGIFactory_Fail;
    }

    return ToyXResult::Success;
}

void WinDX11XWindowImpl::_DestoryDXGIFactory()
{
    if (m_Context.Factory)
    {
        m_Context.Factory->Release();
        m_Context.Factory = nullptr;
    }
}

ToyXResult WinDX11XWindowImpl::_CreateD3DDevice()
{
    HRESULT hr;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG  // TODOL: add to startup param
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    //IDXGIAdapter* defaultAdapter = dynamic_cast<WinDXAdapter*>(m_Adapters.front().get())->GetDXGIAdapter();
    hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
        D3D11_SDK_VERSION, &m_Context.Device, &m_Context.FeatureLevel, &m_Context.DeviceContext);
    if (FAILED(hr))
    {
        return ToyXResult::Error_WinDX11_D3D11CreateDevice_Fail;
    }

    return ToyXResult::Success;
}

void WinDX11XWindowImpl::_DestoryD3DDevice()
{
    if (m_Context.DeviceContext)
    {
        m_Context.DeviceContext->ClearState();
    }

    if (m_Context.DeviceContext)
    {
        m_Context.DeviceContext->Release();
        m_Context.DeviceContext = nullptr;
    }

    if (m_Context.Device)
    {
        m_Context.Device->Release();
        m_Context.Device = nullptr;
    }
}

ToyXResult WinDX11XWindowImpl::StartUp(const XWINDOW_API_STARTUP_DESC & desc)
{
    TOYXRESULT_RETURN_IF_FAIL(WinBaseXWindowImpl::StartUp(desc));

    m_Context.ProgramInstance = GetProgramInstance();

    TOYXRESULT_RETURN_IF_FAIL(_CreateDXGIFactory());
    TOYXRESULT_RETURN_IF_FAIL(_CreateD3DDevice());

    return ToyXResult::Success;
}

void WinDX11XWindowImpl::ShutDown()
{
    _DestoryD3DDevice();
    _DestoryDXGIFactory();

    WinBaseXWindowImpl::ShutDown();
}

ToyUtility::SPtr<IAdapter> WinDX11XWindowImpl::GetDefaultAdapter()
{
    // TODOH
    return ToyUtility::SPtr<IAdapter>();
}

const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& WinDX11XWindowImpl::GetAdapters() const
{
    // TODOH
    static ToyUtility::List<ToyUtility::SPtr<IAdapter>> a;
    return a;
}

ToyUtility::SPtr<IWindow> WinDX11XWindowImpl::CreateWindow(const WINDOW_DESC & desc)
{
    ToyUtility::SPtr<WinDX11Window> window = std::make_shared<WinDX11Window>(m_Context);

    window->SetClassName(WindowsWindowClassName);
    window->SetProgramInstance(m_Context.ProgramInstance);

    if (window->Create(desc) == ToyXResult::Success)
    {
        if(m_MainWindow == nullptr)
            SetMainWindow(window);

        return window;
    }

    // TODOH: how about error code?

    return ToyUtility::SPtr<WinDX11Window>();
}

void WinDX11XWindowImpl::SetMainWindow(ToyUtility::SPtr<IWindow> window)
{
    WinBaseXWindowImpl::SetMainWindow(window);

    m_MainWindow = std::dynamic_pointer_cast<WinDX11Window>(window);

    m_MainWindow->_MakeCurrentContext();
}


} // end of namespace ToyXWindow
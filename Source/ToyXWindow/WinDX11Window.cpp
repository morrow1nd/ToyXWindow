#include "ToyXWindow/details/WinDX11Window.h"


namespace ToyXWindow
{


void WinDX11Window::PresentBackBuffer(uint32 syncInterval)
{
    m_SwapChain->Present(syncInterval, 0);
}

ToyXResult WinDX11Window::InitContext()
{
    // Create SwapChain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = m_Desc.BufferCount;
    sd.BufferDesc.Width = m_Desc.BufferDesc.Width;
    sd.BufferDesc.Height = m_Desc.BufferDesc.Height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = m_Desc.BufferDesc.RefreshRate.Numerator();
    sd.BufferDesc.RefreshRate.Denominator = m_Desc.BufferDesc.RefreshRate.Denominator();
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_HWND;
    sd.SampleDesc.Count = m_Desc.SampleDesc.Count;
    sd.SampleDesc.Quality = m_Desc.SampleDesc.Quality;
    sd.Windowed = m_Desc.Windowed ? TRUE : FALSE;

    m_SwapChain = NULL;
    HRESULT hr = c_Context.Factory->CreateSwapChain(c_Context.Device, &sd, &m_SwapChain);
    if (FAILED(hr))
    {
        return ToyXResult::Error_WinDX11_Factory_CreateSwapChain_Fail;
    }

    // Create a render target view
    ID3D11Texture2D* backBuffer = NULL;
    hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if (FAILED(hr))
        return ToyXResult::Error_WinDX11_SwapChain_GetBuffer_Fail;

    m_RenderTargetView = NULL;
    hr = c_Context.Device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
    backBuffer->Release();
    if (FAILED(hr))
        return ToyXResult::Error_WinDX11_Device_CreateRenderTargetView_Fail;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = m_Desc.BufferDesc.Width;
    descDepth.Height = m_Desc.BufferDesc.Height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = c_Context.Device->CreateTexture2D(&descDepth, NULL, &m_DepthStencil);
    if (FAILED(hr))
        return ToyXResult::Error_WinDX11_Device_CreateTexture2D_Fail;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = c_Context.Device->CreateDepthStencilView(m_DepthStencil, &descDSV, &m_DepthStencilView);
    if (FAILED(hr))
        return ToyXResult::Error_WinDX11_Device_CreateDepthStencilView_Fail;

    c_Context.DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)m_Desc.BufferDesc.Width;
    vp.Height = (FLOAT)m_Desc.BufferDesc.Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    c_Context.DeviceContext->RSSetViewports(1, &vp);

    return ToyXResult::Success;
}

void WinDX11Window::DeInitContext()
{
    if (m_DepthStencil)
    {
        m_DepthStencil->Release();
        m_DepthStencil = nullptr;
    }
    if (m_DepthStencilView)
    {
        m_DepthStencilView->Release();
        m_DepthStencilView = nullptr;
    }
    if (m_RenderTargetView)
    {
        m_RenderTargetView->Release();
        m_RenderTargetView = nullptr;
    }
    if (m_SwapChain)
    {
        m_SwapChain->Release();
        m_SwapChain = nullptr;
    }
}


} // end of namespace ToyXWindow
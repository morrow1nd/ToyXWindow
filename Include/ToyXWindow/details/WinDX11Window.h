#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyXWindow/details/WinWindow.h"
#include "ToyXWindow/details/WinDX11CommonType.h"


namespace ToyXWindow
{


class WinDX11Window : public WinWindow
{
public:
    WinDX11Window(const WinDX11Context& context)
        :
        c_Context(context),
        m_SwapChain(nullptr),
        m_RenderTargetView(nullptr),
        m_DepthStencil(nullptr),
        m_DepthStencilView(nullptr)
    {}


public:
    virtual void PresentBackBuffer(uint32 syncInterval) override;


protected:
    virtual ToyXResult InitContext() override;
    virtual void DeInitContext() override;


private:
    const WinDX11Context& c_Context;

    IDXGISwapChain*                     m_SwapChain;
    ID3D11RenderTargetView*             m_RenderTargetView;
    ID3D11Texture2D*                    m_DepthStencil;
    ID3D11DepthStencilView*             m_DepthStencilView;
};


} // end of namespace ToyXWindow
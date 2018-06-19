#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "ToyUtility/Prerequisites/PreDefine.h"


#ifdef CreateWindow
# undef CreateWindow
#endif


namespace ToyXWindow
{


struct WinDX11Context
{
    WinDX11Context()
        :
        Factory(nullptr),
        Device(nullptr),
        DeviceContext(nullptr),
        ProgramInstance(0),
        FeatureLevel(D3D_FEATURE_LEVEL_11_0)
    {}


    IDXGIFactory* Factory;
    ID3D11Device* Device;
    ID3D11DeviceContext* DeviceContext;

    D3D_FEATURE_LEVEL FeatureLevel;

    HINSTANCE ProgramInstance;
};


} // end of namespace ToyXWindow
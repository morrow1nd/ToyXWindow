#pragma once

#include <functional>
#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyUtility/String/String.h"
#include "ToyUtility/Math/Rect2I.h"
#include "ToyUtility/Math/Rational.h"
#include "ToyXWindowConfig.h"


namespace ToyXWindow
{





//
//
//struct SampleDesc
//{
//    static SampleDesc NoMultiSampling;
//
//    SampleDesc()
//        :
//        Count(1),
//        Quality(0)
//    {
//    }
//
//    SampleDesc(ToyUtility::uint32 count, ToyUtility::uint32 quality)
//        :
//        Count(count),
//        Quality(quality)
//    {
//    }
//
//
//    ToyUtility::uint32 Count; // The number of multisamples per pixel
//    ToyUtility::uint32 Quality; // The image quality level. The higher the quality, the lower the performance
//};
//
//struct ModeDesc
//{
//    ModeDesc()
//        :
//        Width(0),
//        Height(0)
//    {
//    }
//
//
//    ToyUtility::uint32 Width; // resolution width
//    ToyUtility::uint32 Height; // resolution height
//    ToyUtility::Rational RefreshRate; // refresh rate in hertz
//                                      // TODOH: format, scanlineOrdering, scaling
//};
//
//enum class MonitorRotationType
//{
//    Unspecified,
//    Identity,
//    Rotate90,
//    Rotate180,
//    Rotate270,
//};
//
//struct MonitorDesc
//{
//    MonitorDesc()
//        :
//        DeviceName("-Unknown-"),
//        AttachedToDesktop(true),
//        Rotation(MonitorRotationType::Unspecified)
//    {
//    }
//
//
//    ToyUtility::String DeviceName;
//    ToyUtility::Rect2I DesktopCoordinates; // containing the bounds of the output in desktop coordinates
//    bool AttachedToDesktop;
//    MonitorRotationType Rotation;
//};
//
//struct AdapterDesc
//{
//    AdapterDesc()
//        :
//        VendorId(0),
//        DeviceId(0),
//        SubSysId(0),
//        Revision(0),
//        DedicatedVideoMemory(0),
//        DedicatedSystemMemory(0),
//        SharedSystemMemory(0),
//        AdapterLUID(0)
//    {
//    }
//
//
//    ToyUtility::String Description; // A string that contains the adapter description
//    ToyUtility::uint32 VendorId; // The PCI ID of the hardware vendor. PCI: http://pci-ids.ucw.cz/
//    ToyUtility::uint32 DeviceId; // The PCI ID of the hardware device
//    ToyUtility::uint32 SubSysId; // The PCI ID of the sub system
//    ToyUtility::uint32 Revision; // The PCI ID of the revision number of the adapter
//    ToyUtility::uint64 DedicatedVideoMemory; // The number of bytes of dedicated video memory that are not shared with the CPU
//    ToyUtility::uint64 DedicatedSystemMemory; // The number of bytes of dedicated system memory that are not shared with the GPU. This memory is allocated from available system memory at boot time
//    ToyUtility::uint64 SharedSystemMemory; // The number of bytes of shared system memory. This is the maximum value of system memory that may be consumed by the adapter during operation. Any incidental memory consumed by the driver as it manages and uses video memory is additional
//    ToyUtility::uint64 AdapterLUID; // A unique value that identifies the adapter
//};







enum class ToyXResult
{
    Success = 0,

    Error_Others,

    Error_XWindowImpl_FactoryCreate_Fail,

    Error_Win_RegisterWindowsWindowClass_Fail,
    Error_Win_CreateWindowA_Fail,
    Error_Win_CreateHelperWindow_Fail,

    Error_WinDX11_CreateDXGIFactory_Fail,
    Error_WinDX11_D3D11CreateDevice_Fail,
    Error_WinDX11_Factory_CreateSwapChain_Fail,
    Error_WinDX11_SwapChain_GetBuffer_Fail,
    Error_WinDX11_Device_CreateRenderTargetView_Fail,
    Error_WinDX11_Device_CreateTexture2D_Fail,
    Error_WinDX11_Device_CreateDepthStencilView_Fail,

    Error_WinWGL_LoadLibrary_opengl32_dll_Fail,
    Error_WinWGL_SetPixelFormatForDummyContext_Fail,
    Error_WinWGL_CreateDummyContext_Fail,
    Error_WinWGL_MakeDummyContextCurrent_Fail,
    Error_WinWGL_GetDC_Fail,
    Error_WinWGL_ChoosePixelFormat_Fail,
    Error_WinWGL_DescribePixelFormat_Fail,
    Error_WinWGL_SetPixelFormat_WhileInitContext_Fail,
};

#define TOYXRESULT_RETURN_IF_FAIL(value)                \
{                                                       \
    ToyXResult result = (value); /* call only once */   \
    if ((result) != ToyXResult::Success)                \
        return result;                                  \
}


using ToyUtility::uint8;
using ToyUtility::int8;
using ToyUtility::uint16;
using ToyUtility::int16;
using ToyUtility::uint32;
using ToyUtility::int32;
using ToyUtility::uint64;
using ToyUtility::int64;


enum class KeyType
{
    __Invalid = -2,
    __Unknown = -1,
    
    __Begin = 0,

    // Printable key
    Space                   = 32,
    Apostrophe              = 39, // '
    Comma                   = 44, // ,
    Minus                   = 45, // -
    Period                  = 46, // .
    Slash                   = 47, // /
    _0                      = 48,
    _1                      = 49,
    _2                      = 50,
    _3                      = 51,
    _4                      = 52,
    _5                      = 53,
    _6                      = 54,
    _7                      = 55,
    _8                      = 56,
    _9                      = 57,
    Semicolon               = 59, // ;
    Equal                   = 61, // =
    A                       = 65,
    B                       = 66,
    C                       = 67,
    D                       = 68,
    E                       = 69,
    F                       = 70,
    G                       = 71,
    H                       = 72,
    I                       = 73,
    J                       = 74,
    K                       = 75,
    L                       = 76,
    M                       = 77,
    N                       = 78,
    O                       = 79,
    P                       = 80,
    Q                       = 81,
    R                       = 82,
    S                       = 83,
    T                       = 84,
    U                       = 85,
    V                       = 86,
    W                       = 87,
    X                       = 88,
    Y                       = 89,
    Z                       = 90,
    LeftBracket             = 91, // [
    BackSlash               = 92, // \ 
    RightBracket            = 93, // ]
    GraveAccent             = 96, // ` 

    // Function keys
    Escape                  = 256,
    Enter,
    Tab,
    Backspace,
    Insert,
    Delete,
    Right,
    Left,
    Down,
    Up,
    PageUp,
    PageDown,
    Home,
    End,
    CapsLock,
    ScrollLock,
    NumberLock,
    PrintScreen,
    Pause,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    F25,
    LeftShift,
    LeftControl,
    LeftAlt,
    LeftSuper,
    RightShift,
    RightControl,
    RightAlt,
    RightSuper,
    Menu,

    // Keypad
    Keypad_0,
    Keypad_1,
    Keypad_2,
    Keypad_3,
    Keypad_4,
    Keypad_5,
    Keypad_6,
    Keypad_7,
    Keypad_8,
    Keypad_9,
    Keypad_Decimal,
    Keypad_Divide,
    Keypad_Multiply,
    Keypad_Subtact,
    Keypad_Add,
    Keypad_Enter,
    Keypad_Equal,

    __End,
};

enum class ModifierKeyType
{
    Shift       = 1 << 0,
    Control     = 1 << 1,
    Alt         = 1 << 2,
    Super       = 1 << 3,
};

enum class KeyAction
{
    Press,
    Release,
    R // TODOH
};

enum class CursorMode
{
    Normal,
    Disabled,
    Hidden,
};

enum class ButtonState
{
    Press,
    Release,
    R // TODOH
};

enum class MouseButtonType
{
    __Begin,

    Left,
    Right,
    Middle,
    _4,
    _5,

    __End,
};

enum class WindowContextNativeApiType
{
    Win_DX11,
    Win_WGL,

    MacOS_CGL,

    Linux_EGL,
    
    Android_EGL,
};

struct XWINDOW_API_STARTUP_DESC
{
    WindowContextNativeApiType WindowContextNativeApi;
};

struct ScrollDesc
{
    ScrollDesc()
        :
        IsMouse(true)
    {}


    bool IsMouse;
    union
    {
        struct
        {
            float XOffset, YOffset;
        } NormalScroll;
        struct
        {
            ToyUtility::int32 XOffset, YOffset;
        } MouseScroll;
    } Scroll;
};

struct SampleDesc
{
    static SampleDesc NoMultiSampling;

    SampleDesc()
        :
        Count(1),
        Quality(0)
    {}

    SampleDesc(ToyUtility::uint32 count, ToyUtility::uint32 quality)
        :
        Count(count),
        Quality(quality)
    {}


    ToyUtility::uint32 Count; // The number of multisamples per pixel
    ToyUtility::uint32 Quality; // The image quality level. The higher the quality, the lower the performance
};

struct ModeDesc
{
    ModeDesc()
        :
        Width(0),
        Height(0)
    {}


    ToyUtility::uint32 Width; // resolution width
    ToyUtility::uint32 Height; // resolution height
    ToyUtility::Rational RefreshRate; // refresh rate in hertz
    // TODOH: format, scanlineOrdering, scaling
};

enum class ProfileType
{
    Any = 0,
    OpenGL_Core = 0x00032001,
    OpenGL_Compat = 0x00032002,
};

enum class ReleaseType
{
    Any = 0,
    OpenGL_Flush = 0x00035001,
    OpenGL_None = 0x00035002,
};

enum class RobustnessType
{
    None,
    OpenGL_NO_RESET_NOTIFICATION = 0x00031001,
    OpenGL_LOSE_CONTEXT_ON_RESET = 0x00031002,
};

struct ContextConfig
{
    static ContextConfig WinWGLDefault;


    ContextConfig()
        :
        client(0),
        source(0),
        major(2),
        minor(0),
        forward(0),
        debug(0),
        noerror(0),
        profile(ProfileType::Any),
        robustness(RobustnessType::None),
        release(ReleaseType::Any)
    {}

    int             client;
    int             source;
    int             major;
    int             minor;
    bool            forward;
    bool            debug;
    bool            noerror;
    ProfileType     profile;
    RobustnessType  robustness;
    ReleaseType     release;
    //_GLFWwindow*  share;
};

struct FrameBufferConfig
{
    static FrameBufferConfig Default;


    FrameBufferConfig()
        :
        RedBits(8),
        GreenBits(8),
        BlueBits(8),
        AlphaBits(8),
        DepthBits(24),
        StencilBits(8),
        AccumRedBits(0),
        AccumGreenBits(0),
        AccumBlueBits(0),
        AccumAlphaBits(0),
        AuxBuffers(0),
        Stereo(0),
        Samples(0),
        sRGB(0),
        Doublebuffer(1),
        Handle(0)
    {}


    int         RedBits;
    int         GreenBits;
    int         BlueBits;
    int         AlphaBits;
    int         DepthBits;
    int         StencilBits;
    int         AccumRedBits;
    int         AccumGreenBits;
    int         AccumBlueBits;
    int         AccumAlphaBits;
    int         AuxBuffers;
    bool        Stereo;
    int         Samples;
    bool        sRGB;
    bool        Doublebuffer;
    uintptr_t   Handle;
};

struct WINDOW_DESC
{
    WINDOW_DESC()
        :
        Title(""),
        Windowed(true),
        BufferCount(1)
    {}


    ToyUtility::String              Title;
    ToyUtility::Rect2I              WindowRect;

    bool                            Windowed; // Windowed mode
    ModeDesc                        BufferDesc; // Describing the backbuffer display mode
    SampleDesc                      SampleDesc; // describing multi-sampling 
    ToyUtility::uint32              BufferCount; // Back buffer count
};

const int XWINDOW_DONT_CARE = -1;


} // end of namespace ToyXWindow
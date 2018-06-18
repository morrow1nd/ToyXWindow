#pragma once

#include <functional>
#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyUtility/String/String.h"
#include "ToyUtility/Math/Rect2I.h"
#include "ToyUtility/Math/Rational.h"
#include "ToyXWindowConfig.h"


namespace ToyXWindow
{


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
    Unknown = -1,

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
    Left,
    Right,
    Middle
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

struct WINDOW_DESC
{
    WINDOW_DESC()
        :
        Title(""),
        Windowed(true),
        BufferCount(1)
    {
    }


    ToyUtility::String              Title;
    ToyUtility::Rect2I              WindowRect;

    bool                            Windowed; // Windowed mode
    ModeDesc                        BufferDesc; // Describing the backbuffer display mode
    SampleDesc                      SampleDesc; // describing multi-sampling 
    ToyUtility::uint32              BufferCount; // Back buffer count
};


} // end of namespace ToyXWindow